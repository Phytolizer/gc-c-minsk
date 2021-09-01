#pragma once

#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>

int unary_operator_precedence(enum SyntaxKind kind);
int binary_operator_precedence(enum SyntaxKind kind);
