#pragma once

#include <common/Object.h>
#include <minsk/CodeAnalysis/Diagnostic.h>
#include <minsk/CodeAnalysis/Syntax/SyntaxKind.h>

struct DiagnosticBag
{
  struct DiagnosticList* diagnostics;
};

struct DiagnosticBag* diagnostic_bag_new(void);

void diagnostic_bag_report_invalid_number(
    struct DiagnosticBag* bag,
    struct TextSpan* span,
    sds text,
    enum ObjectKind kind);
void diagnostic_bag_report_bad_character(
    struct DiagnosticBag* bag,
    int position,
    char character);
void diagnostic_bag_report_unexpected_token(
    struct DiagnosticBag* bag,
    struct TextSpan* span,
    enum SyntaxKind actual_kind,
    enum SyntaxKind expected_kind);
void diagnostic_bag_report_undefined_binary_operator(
    struct DiagnosticBag* bag,
    struct TextSpan* span,
    sds operator_text,
    enum ObjectKind left_type,
    enum ObjectKind right_type);
void diagnostic_bag_report_undefined_unary_operator(
    struct DiagnosticBag* bag,
    struct TextSpan* span,
    sds operator_text,
    enum ObjectKind operand_type);
void diagnostic_bag_report_undefined_name(
    struct DiagnosticBag* bag,
    struct TextSpan* span,
    sds name);
void diagnostic_bag_report_cannot_convert(
    struct DiagnosticBag* bag,
    struct TextSpan* span,
    enum ObjectKind from_type,
    enum ObjectKind to_type);

void diagnostic_bag_add_range(
    struct DiagnosticBag* bag,
    const struct DiagnosticBag* other);
