#include "Parser.h"

#include <stdbool.h>

#include <IncludeMe.h>
#include <common/List.h>
#include <minsk/CodeAnalysis/Syntax/AssignmentExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/BinaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/BlockStatementSyntax.h>
#include <minsk/CodeAnalysis/Syntax/CompilationUnitSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ElseClauseSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ExpressionStatementSyntax.h>
#include <minsk/CodeAnalysis/Syntax/IfStatementSyntax.h>
#include <minsk/CodeAnalysis/Syntax/LiteralExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/NameExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/ParenthesizedExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxFacts.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxTree.h>
#include <minsk/CodeAnalysis/Syntax/UnaryExpressionSyntax.h>
#include <minsk/CodeAnalysis/Syntax/VariableDeclarationSyntax.h>
#include <minsk/CodeAnalysis/Syntax/WhileStatementSyntax.h>
#include <sds.h>

#include "Lexer.h"

static struct SyntaxToken* peek(struct Parser* parser, int offset) __attribute__((const));
static struct SyntaxToken* current(struct Parser* parser) __attribute__((const));
static struct SyntaxToken* next_token(struct Parser* parser);
static struct SyntaxToken* match_token(struct Parser* parser, enum SyntaxKind kind);

static struct StatementSyntax* parse_statement(struct Parser* parser);
static struct StatementSyntax* parse_block_statement(struct Parser* parser);
static struct ElseClauseSyntax* parse_else_clause(struct Parser* parser);
static struct StatementSyntax* parse_expression_statement(struct Parser* parser);
static struct StatementSyntax* parse_if_statement(struct Parser* parser);
static struct StatementSyntax* parse_variable_declaration(struct Parser* parser);
static struct StatementSyntax* parse_while_statement(struct Parser* parser);

static struct ExpressionSyntax* parse_expression(struct Parser* parser);
static struct ExpressionSyntax* parse_assignment_expression(struct Parser* parser);
static struct ExpressionSyntax* parse_binary_expression(struct Parser* parser, int parent_precedence);
static struct ExpressionSyntax* parse_primary_expression(struct Parser* parser);
static struct ExpressionSyntax* parse_parenthesized_expression(struct Parser* parser);
static struct ExpressionSyntax* parse_boolean_literal(struct Parser* parser);
static struct ExpressionSyntax* parse_name_expression(struct Parser* parser);
static struct ExpressionSyntax* parse_number_literal(struct Parser* parser);

struct Parser* parser_new(struct SourceText* source_text)
{
    struct Parser* parser = mc_malloc(sizeof(struct Parser));
    parser->source_text = source_text;
    parser->tokens = mc_malloc(sizeof(struct SyntaxTokenList));
    LIST_INIT(parser->tokens);
    parser->position = 0;
    struct Lexer* lexer = lexer_new(source_text);
    while (true)
    {
        struct SyntaxToken* token = lexer_next_token(lexer);
        if (token->kind != SYNTAX_KIND_WHITESPACE_TOKEN && token->kind != SYNTAX_KIND_BAD_TOKEN)
        {
            LIST_PUSH(parser->tokens, token);
        }
        if (token->kind == SYNTAX_KIND_END_OF_FILE_TOKEN)
        {
            break;
        }
    }
    parser->diagnostics = diagnostic_bag_new();
    diagnostic_bag_add_range(parser->diagnostics, lexer->diagnostics);
    return parser;
}

void parser_free(struct Parser* parser)
{
    LIST_DEINIT(parser->tokens);
    mc_free(parser->tokens);
    mc_free(parser);
}

struct CompilationUnitSyntax* parser_parse_compilation_unit(struct Parser* parser)
{
    struct StatementSyntax* statement = parse_statement(parser);
    struct SyntaxToken* end_of_file_token = match_token(parser, SYNTAX_KIND_END_OF_FILE_TOKEN);
    return compilation_unit_syntax_new(statement, end_of_file_token);
}

static struct SyntaxToken* peek(struct Parser* parser, int offset)
{
    int index = parser->position + offset;
    if (index >= parser->tokens->length)
    {
        return parser->tokens->data[parser->tokens->length - 1];
    }
    return parser->tokens->data[index];
}

static struct SyntaxToken* current(struct Parser* parser)
{
    return peek(parser, 0);
}

static struct SyntaxToken* next_token(struct Parser* parser)
{
    struct SyntaxToken* curr = current(parser);
    ++parser->position;
    return curr;
}

static struct SyntaxToken* match_token(struct Parser* parser, enum SyntaxKind kind)
{
    if (current(parser)->kind == kind)
    {
        return next_token(parser);
    }

    diagnostic_bag_report_unexpected_token(parser->diagnostics, syntax_token_get_span(current(parser)),
                                           current(parser)->kind, kind);
    return syntax_token_new(kind, current(parser)->position, sdsempty(), OBJECT_NULL());
}

static struct StatementSyntax* parse_statement(struct Parser* parser)
{
    switch (current(parser)->kind)
    {
    case SYNTAX_KIND_OPEN_BRACE_TOKEN:
        return parse_block_statement(parser);
    case SYNTAX_KIND_LET_KEYWORD:
    case SYNTAX_KIND_VAR_KEYWORD:
        return parse_variable_declaration(parser);
    case SYNTAX_KIND_IF_KEYWORD:
        return parse_if_statement(parser);
    case SYNTAX_KIND_WHILE_KEYWORD:
        return parse_while_statement(parser);
    default:
        return parse_expression_statement(parser);
    }
}

static struct StatementSyntax* parse_block_statement(struct Parser* parser)
{
    struct SyntaxToken* open_brace_token = match_token(parser, SYNTAX_KIND_OPEN_BRACE_TOKEN);
    struct StatementSyntaxList* statements = mc_malloc(sizeof(struct StatementSyntaxList));
    LIST_INIT(statements);
    while (current(parser)->kind != SYNTAX_KIND_CLOSE_BRACE_TOKEN &&
           current(parser)->kind != SYNTAX_KIND_END_OF_FILE_TOKEN)
    {
        LIST_PUSH(statements, parse_statement(parser));
    }
    struct SyntaxToken* close_brace_token = match_token(parser, SYNTAX_KIND_CLOSE_BRACE_TOKEN);
    return (struct StatementSyntax*)block_statement_syntax_new(open_brace_token, statements, close_brace_token);
}

static struct ElseClauseSyntax* parse_else_clause(struct Parser* parser)
{
    if (current(parser)->kind != SYNTAX_KIND_ELSE_KEYWORD)
    {
        return NULL;
    }

    struct SyntaxToken* keyword = next_token(parser);
    struct StatementSyntax* statement = parse_statement(parser);
    return else_clause_syntax_new(keyword, statement);
}

static struct StatementSyntax* parse_expression_statement(struct Parser* parser)
{
    struct ExpressionSyntax* expression = parse_expression(parser);
    return (struct StatementSyntax*)expression_statement_syntax_new(expression);
}

static struct StatementSyntax* parse_if_statement(struct Parser* parser)
{
    struct SyntaxToken* keyword = match_token(parser, SYNTAX_KIND_IF_KEYWORD);
    struct ExpressionSyntax* condition = parse_expression(parser);
    struct StatementSyntax* statement = parse_statement(parser);
    struct ElseClauseSyntax* else_clause = parse_else_clause(parser);
    return (struct StatementSyntax*)if_statement_syntax_new(keyword, condition, statement, else_clause);
}

static struct StatementSyntax* parse_variable_declaration(struct Parser* parser)
{
    enum SyntaxKind expected =
        current(parser)->kind == SYNTAX_KIND_LET_KEYWORD ? SYNTAX_KIND_LET_KEYWORD : SYNTAX_KIND_VAR_KEYWORD;
    struct SyntaxToken* keyword_token = match_token(parser, expected);
    struct SyntaxToken* identifier_token = match_token(parser, SYNTAX_KIND_IDENTIFIER_TOKEN);
    struct SyntaxToken* equals_token = match_token(parser, SYNTAX_KIND_EQUALS_TOKEN);
    struct ExpressionSyntax* initializer = parse_expression(parser);
    return (struct StatementSyntax*)variable_declaration_syntax_new(keyword_token, identifier_token, equals_token,
                                                                    initializer);
}

static struct StatementSyntax* parse_while_statement(struct Parser* parser)
{
    struct SyntaxToken* keyword = match_token(parser, SYNTAX_KIND_WHILE_KEYWORD);
    struct ExpressionSyntax* condition = parse_expression(parser);
    struct StatementSyntax* body = parse_statement(parser);
    return (struct StatementSyntax*)while_statement_syntax_new(keyword, condition, body);
}

static struct ExpressionSyntax* parse_expression(struct Parser* parser)
{
    return parse_assignment_expression(parser);
}

static struct ExpressionSyntax* parse_assignment_expression(struct Parser* parser)
{
    if (peek(parser, 0)->kind == SYNTAX_KIND_IDENTIFIER_TOKEN && peek(parser, 1)->kind == SYNTAX_KIND_EQUALS_TOKEN)
    {
        struct SyntaxToken* identifier_token = next_token(parser);
        struct SyntaxToken* equals_token = next_token(parser);
        struct ExpressionSyntax* right = parse_assignment_expression(parser);
        return (struct ExpressionSyntax*)assignment_expression_new(identifier_token, equals_token, right);
    }

    return parse_binary_expression(parser, 0);
}

static struct ExpressionSyntax* parse_binary_expression(struct Parser* parser, int parent_precedence)
{
    struct ExpressionSyntax* left;
    int unary_operator_prec = unary_operator_precedence(current(parser)->kind);
    if (unary_operator_prec != 0 && unary_operator_prec >= parent_precedence)
    {
        struct SyntaxToken* operator_token = next_token(parser);
        struct ExpressionSyntax* operand = parse_binary_expression(parser, unary_operator_prec);
        left = (struct ExpressionSyntax*)unary_expression_syntax_new(operator_token, operand);
    }
    else
    {
        left = parse_primary_expression(parser);
    }

    while (true)
    {
        int precedence = binary_operator_precedence(current(parser)->kind);
        if (precedence == 0 || precedence <= parent_precedence)
        {
            break;
        }

        struct SyntaxToken* operator_token = next_token(parser);
        struct ExpressionSyntax* right = parse_binary_expression(parser, precedence);
        left = (struct ExpressionSyntax*)binary_expression_syntax_new(left, operator_token, right);
    }

    return left;
}

static struct ExpressionSyntax* parse_primary_expression(struct Parser* parser)
{
    switch (current(parser)->kind)
    {
    case SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN:
        return parse_parenthesized_expression(parser);
    case SYNTAX_KIND_TRUE_KEYWORD:
    case SYNTAX_KIND_FALSE_KEYWORD:
        return parse_boolean_literal(parser);
    case SYNTAX_KIND_NUMBER_TOKEN:
        return parse_number_literal(parser);
    default:
        return parse_name_expression(parser);
    }
}

static struct ExpressionSyntax* parse_parenthesized_expression(struct Parser* parser)
{
    struct SyntaxToken* open_parenthesis_token = match_token(parser, SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN);
    struct ExpressionSyntax* expression = parse_expression(parser);
    struct SyntaxToken* close_parenthesis_token = match_token(parser, SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN);
    return (struct ExpressionSyntax*)parenthesized_expression_syntax_new(open_parenthesis_token, expression,
                                                                         close_parenthesis_token);
}

static struct ExpressionSyntax* parse_boolean_literal(struct Parser* parser)
{
    bool is_true = current(parser)->kind == SYNTAX_KIND_TRUE_KEYWORD;
    struct SyntaxToken* keyword_token =
        match_token(parser, is_true ? SYNTAX_KIND_TRUE_KEYWORD : SYNTAX_KIND_FALSE_KEYWORD);
    return (struct ExpressionSyntax*)literal_expression_syntax_new(keyword_token, OBJECT_BOOLEAN(is_true));
}

static struct ExpressionSyntax* parse_name_expression(struct Parser* parser)
{
    struct SyntaxToken* identifier_token = match_token(parser, SYNTAX_KIND_IDENTIFIER_TOKEN);
    return (struct ExpressionSyntax*)name_expression_syntax_new(identifier_token);
}

static struct ExpressionSyntax* parse_number_literal(struct Parser* parser)
{
    struct SyntaxToken* number_token = match_token(parser, SYNTAX_KIND_NUMBER_TOKEN);
    return (struct ExpressionSyntax*)literal_expression_syntax_new(number_token, number_token->value);
}
