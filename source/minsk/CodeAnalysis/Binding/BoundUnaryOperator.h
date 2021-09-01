#pragma once

#include <common/Object.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>

#include "BoundUnaryOperatorKind.h"

struct BoundUnaryOperator
{
  enum SyntaxKind syntax_kind;
  enum BoundUnaryOperatorKind kind;
  enum ObjectKind operand_type;
  enum ObjectKind result_type;
};

struct BoundUnaryOperator* bind_unary_operator(
    enum SyntaxKind syntax_kind,
    enum ObjectKind operand_type);
