#pragma once

#include <minsk-private/CodeAnalysis/Binding/BoundExpression.h>
#include <minsk-private/CodeAnalysis/Binding/BoundNode.h>
#include <minsk-private/CodeAnalysis/Binding/BoundStatement.h>

#define BOUND_TREE_REWRITER_KINDS_ X(LOWERER)

enum BoundTreeRewriterKind
{
#define X(x) BOUND_TREE_REWRITER_KIND_##x,
    BOUND_TREE_REWRITER_KINDS_
#undef X
};

struct BoundTreeRewriter
{
    enum BoundTreeRewriterKind kind;
};

struct BoundStatement* bound_tree_rewriter_rewrite_statement(
    struct BoundTreeRewriter* rewriter, struct BoundStatement* node);
struct BoundExpression* bound_tree_rewriter_rewrite_expression(
    struct BoundTreeRewriter* rewriter, struct BoundExpression* node);
