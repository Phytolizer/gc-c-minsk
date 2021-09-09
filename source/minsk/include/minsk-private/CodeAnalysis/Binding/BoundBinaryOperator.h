#pragma once

#include <common/Object.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>

#include "BoundBinaryOperatorKind.h"

struct BoundBinaryOperator
{
    enum SyntaxKind syntax_kind;
    enum BoundBinaryOperatorKind kind;
    enum ObjectKind left_type;
    enum ObjectKind right_type;
    enum ObjectKind result_type;
};

extern struct BoundBinaryOperator* bind_binary_operator(
    enum SyntaxKind syntax_kind, enum ObjectKind left_type, enum ObjectKind right_type);
