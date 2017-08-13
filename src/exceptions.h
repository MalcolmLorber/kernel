#ifndef _EXCEPTIONS_INC
#define _EXCEPTIONS_INC

#include "idt.h"

//error functions for each possible x86 error code
void int_div_by_zero(uint32_t error);
void int_debug(uint32_t error);
void int_non_maskable_interrupt(uint32_t error);
void int_breakpoint(uint32_t error);
void int_overflow(uint32_t error);
void int_bound_range_exceeded(uint32_t error);
void int_invalid_opcode(uint32_t error);
void int_device_not_available(uint32_t error);
void int_double_fault(uint32_t error);
void int_coprocessor_segment_overrun(uint32_t error);
void int_invalid_tss(uint32_t error);
void int_segment_not_present(uint32_t error);
void int_stack_segment_fault(uint32_t error);
void int_general_protection_fault(uint32_t error);
void int_page_fault(uint32_t error);
void int_reserved(uint32_t error);
void int_x87_floating_point(uint32_t error);
void int_alignment_check(uint32_t error);
void int_machine_check(uint32_t error);
void int_simd_floating_point(uint32_t error);
void int_virtualization(uint32_t error);
void int_security(uint32_t error);

#endif
