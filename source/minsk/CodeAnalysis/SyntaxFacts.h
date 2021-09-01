#pragma once

#include <minsk/CodeAnalysis/SyntaxKind.h>

int unary_operator_precedence(enum SyntaxKind kind);
int binary_operator_precedence(enum SyntaxKind kind);
