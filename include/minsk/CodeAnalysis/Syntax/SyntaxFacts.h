#pragma once

#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>
#include <sds.h>

int unary_operator_precedence(enum SyntaxKind kind);
int binary_operator_precedence(enum SyntaxKind kind);
enum SyntaxKind keyword_kind(sds text);
