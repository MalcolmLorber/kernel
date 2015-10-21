#include "exceptions.h"
#include "serial.h"

void int_div_by_zero(uint32_t __attribute__((unused))error)
{
    serial_writestring("DIV BY ZERO ERRO");
}
void int_debug(uint32_t __attribute__((unused))error)
{
    serial_writestring("DEBUG ERROR");
}
void int_non_maskable_interrupt(uint32_t __attribute__((unused))error)
{
    serial_writestring("NON MASKABLE INTERRUPT ERROR");
}
void int_breakpoint(uint32_t __attribute__((unused))error)
{
    serial_writestring("BREAKPOINT ERROR");
}
void int_overflow(uint32_t __attribute__((unused))error)
{
    serial_writestring("OVERFLOW ERROR");
}
void int_bound_range_exceeded(uint32_t __attribute__((unused))error)
{
    serial_writestring("BOUND RANGE EXCEEDED ERROR");
}
void int_invalid_opcode(uint32_t __attribute__((unused))error)
{
    serial_writestring("INVALID OPCODE ERROR");
}
void int_device_not_available(uint32_t __attribute__((unused))error)
{
    serial_writestring("DEVICE NOT AVAILABLE ERROR");
}
void int_double_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("DOUBLE FAULT ERROR");
}
void int_coprocessor_segment_overrun(uint32_t __attribute__((unused))error)
{
    serial_writestring("COPROCESSOR SEGMENT OVERRUN ERROR");
}
void int_invalid_tss(uint32_t __attribute__((unused))error)
{
    serial_writestring("INVALID TSS ERROR");
}
void int_segment_not_present(uint32_t __attribute__((unused))error)
{
    serial_writestring("SEGMENT NOT PRESENT ERROR");
}
void int_stack_segment_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("STACK SEGMENT ERROR");
}
void int_general_protection_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("GENERAL PROTECTION FAULT ERROR");
}
void int_page_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("PAGE FAULT ERROR");
}
void int_reserved(uint32_t __attribute__((unused))error)
{
    serial_writestring("RESERVED ERROR");
}
void int_x87_floating_point(uint32_t __attribute__((unused))error)
{
    serial_writestring("X87 FLOATING POINT ERROR");
}
void int_alignment_check(uint32_t __attribute__((unused))error)
{
    serial_writestring("ALIGNMENT ERROR");
}
void int_machine_check(uint32_t __attribute__((unused))error)
{
    serial_writestring("MACHINE CHECK ERROR");
}
void int_simd_floating_point(uint32_t __attribute__((unused))error)
{
    serial_writestring("SIMD FLOATING POINT ERROR");
}
void int_virtualization(uint32_t __attribute__((unused))error)
{
    serial_writestring("VITUALIZATION ERROR");
}
void int_security(uint32_t __attribute__((unused))error)
{
    serial_writestring("SECURITY ERROR");
}
