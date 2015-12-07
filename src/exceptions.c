#include "exceptions.h"
#include "serial.h"

void int_div_by_zero(uint32_t __attribute__((unused))error)
{
    serial_writestring("DIV BY ZERO ERROR\n");
}
void int_debug(uint32_t __attribute__((unused))error)
{
    serial_writestring("DEBUG ERROR\n");
}
void int_non_maskable_interrupt(uint32_t __attribute__((unused))error)
{
    serial_writestring("NON MASKABLE INTERRUPT ERROR\n");
}
void int_breakpoint(uint32_t __attribute__((unused))error)
{
    serial_writestring("BREAKPOINT ERROR\n");
}
void int_overflow(uint32_t __attribute__((unused))error)
{
    serial_writestring("OVERFLOW ERROR\n");
}
void int_bound_range_exceeded(uint32_t __attribute__((unused))error)
{
    serial_writestring("BOUND RANGE EXCEEDED ERROR\n");
}
void int_invalid_opcode(uint32_t __attribute__((unused))error)
{
    serial_writestring("INVALID OPCODE ERROR\n");
}
void int_device_not_available(uint32_t __attribute__((unused))error)
{
    serial_writestring("DEVICE NOT AVAILABLE ERROR\n");
}
void int_double_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("DOUBLE FAULT ERROR\n");
}
void int_coprocessor_segment_overrun(uint32_t __attribute__((unused))error)
{
    serial_writestring("COPROCESSOR SEGMENT OVERRUN ERROR\n");
}
void int_invalid_tss(uint32_t __attribute__((unused))error)
{
    serial_writestring("INVALID TSS ERROR\n");
}
void int_segment_not_present(uint32_t __attribute__((unused))error)
{
    serial_writestring("SEGMENT NOT PRESENT ERROR\n");
}
void int_stack_segment_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("STACK SEGMENT ERROR\n");
}
void int_general_protection_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("GENERAL PROTECTION FAULT ERROR\n");
}
void int_page_fault(uint32_t __attribute__((unused))error)
{
    serial_writestring("PAGE FAULT ERROR ");
    serial_hexword(error);
    uint32_t cr2;
    asm volatile("movl %%cr2, %0" : "=r" (cr2) : );
    //register uint32_t cr2 asm("%%cr2");
    serial_writestring(" ");
    serial_hexword(cr2);
    serial_writestring("\n");
}
void int_reserved(uint32_t __attribute__((unused))error)
{
    serial_writestring("RESERVED ERROR\n");
}
void int_x87_floating_point(uint32_t __attribute__((unused))error)
{
    serial_writestring("X87 FLOATING POINT ERROR\n");
}
void int_alignment_check(uint32_t __attribute__((unused))error)
{
    serial_writestring("ALIGNMENT ERROR\n");
}
void int_machine_check(uint32_t __attribute__((unused))error)
{
    serial_writestring("MACHINE CHECK ERROR\n");
}
void int_simd_floating_point(uint32_t __attribute__((unused))error)
{
    serial_writestring("SIMD FLOATING POINT ERROR\n");
}
void int_virtualization(uint32_t __attribute__((unused))error)
{
    serial_writestring("VITUALIZATION ERROR\n");
}
void int_security(uint32_t __attribute__((unused))error)
{
    serial_writestring("SECURITY ERROR\n");
}
