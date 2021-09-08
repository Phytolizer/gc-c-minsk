#include "minsk/CodeAnalysis/DiagnosticBag.h"

static void report(struct DiagnosticBag* bag, struct TextSpan* span, sds message);

struct DiagnosticBag* diagnostic_bag_new(void)
{
    struct DiagnosticBag* bag = mc_malloc(sizeof(struct DiagnosticBag));
    bag->diagnostics = mc_malloc(sizeof(struct DiagnosticList));
    LIST_INIT(bag->diagnostics);
    return bag;
}

void diagnostic_bag_report_invalid_number(
    struct DiagnosticBag* bag, struct TextSpan* span, sds text, enum ObjectKind kind)
{
    sds message = sdscatfmt(sdsempty(), "The number %S isn't valid %s.", text, OBJECT_KINDS[kind]);
    report(bag, span, message);
}

void diagnostic_bag_report_bad_character(struct DiagnosticBag* bag, int position, char character)
{
    sds message = sdscatprintf(sdsempty(), "bad character input: '%c'.", character);
    report(bag, text_span_new(position, 1), message);
}

void diagnostic_bag_report_unexpected_token(
    struct DiagnosticBag* bag, struct TextSpan* span, enum SyntaxKind actual_kind, enum SyntaxKind expected_kind)
{
    sds message = sdscatprintf(
        sdsempty(), "Unexpected token <%s>, expected <%s>.", SYNTAX_KINDS[actual_kind], SYNTAX_KINDS[expected_kind]);
    report(bag, span, message);
}

void diagnostic_bag_report_undefined_binary_operator(
    struct DiagnosticBag* bag,
    struct TextSpan* span,
    sds operator_text,
    enum ObjectKind left_type,
    enum ObjectKind right_type)
{
    sds message = sdscatfmt(
        sdsempty(),
        "The binary operator %S is not defined for types '%s' and '%s'.",
        operator_text,
        OBJECT_KINDS[left_type],
        OBJECT_KINDS[right_type]);
    report(bag, span, message);
}

void diagnostic_bag_report_undefined_unary_operator(
    struct DiagnosticBag* bag, struct TextSpan* span, sds operator_text, enum ObjectKind operand_type)
{
    sds message = sdscatfmt(
        sdsempty(), "The unary operator %S is not defined for type '%s'.", operator_text, OBJECT_KINDS[operand_type]);
    report(bag, span, message);
}

void diagnostic_bag_report_undefined_name(struct DiagnosticBag* bag, struct TextSpan* span, sds name)
{
    sds message = sdscatfmt(sdsempty(), "Variable '%S' doesn't exist.", name);
    report(bag, span, message);
}

void diagnostic_bag_report_cannot_convert(
    struct DiagnosticBag* bag, struct TextSpan* span, enum ObjectKind from_type, enum ObjectKind to_type)
{
    sds message =
        sdscatfmt(sdsempty(), "Cannot convert type '%s' to '%s'.", OBJECT_KINDS[from_type], OBJECT_KINDS[to_type]);
    report(bag, span, message);
}

void diagnostic_bag_report_variable_already_declared(struct DiagnosticBag* bag, struct TextSpan* span, sds name)
{
    sds message = sdscatfmt(sdsempty(), "Variable '%S' is already declared.", name);
    report(bag, span, message);
}

void diagnostic_bag_report_cannot_assign(struct DiagnosticBag* bag, struct TextSpan* span, sds name)
{
    sds message = sdscatfmt(sdsempty(), "Variable '%S' is read-only and cannot be assigned to.", name);
    report(bag, span, message);
}

void diagnostic_bag_add_range(struct DiagnosticBag* bag, const struct DiagnosticBag* other)
{
    for (long i = 0; i < other->diagnostics->length; i++)
    {
        LIST_PUSH(bag->diagnostics, other->diagnostics->data[i]);
    }
}

static void report(struct DiagnosticBag* bag, struct TextSpan* span, sds message)
{
    struct Diagnostic* diagnostic = diagnostic_new(span, message);
    LIST_PUSH(bag->diagnostics, diagnostic);
}
