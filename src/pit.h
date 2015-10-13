#ifndef _PIT_INC
#define _PIT_INC

#include <stdint.h>

//OCW masks
#define PIT_OCW_BINC 0x01
#define PIT_OCW_MODE 0x0E
#define PIT_OCW_RL   0x30
#define PIT_OCW_CONT 0xC0

//OCW control bits
#define PIT_OCW_BINC_BIN 0x0
#define PIT_OCW_BINC_BCD 0x1

#define PIT_OCW_MODE_TERMCNT 0x0
#define PIT_OCW_MODE_ONESHOT 0x2
#define PIT_OCW_MODE_RATEGEN 0x4
#define PIT_OCW_MODE_SQWVGEN 0x6
#define PIT_OCW_MODE_SOFTTRG 0x8
#define PIT_OCW_MODE_HARDTRG 0xA

#define PIT_OCW_RL_LATH 0x00
#define PIT_OCW_RL_LSB  0x10
#define PIT_OCW_RL_MSB  0x20
#define PIT_OCW_RL_DATA 0x30

#define PIT_OCW_CONT_0 0x00
#define PIT_OCW_CONT_1 0x40
#define PIT_OCW_CONT_2 0x80

#define PIT_REG_CONT_0 0x40
#define PIT_REG_CONT_1 0x41
#define PIT_REG_CONT_2 0x42
#define PIT_REG_COMD 0x43

void pit_command(uint8_t cmd);
void pit_write(uint16_t data, uint8_t counter);
uint8_t pit_read(uint16_t counter);
uint32_t pit_set_tick(uint32_t i);
uint32_t pit_get_tick();
void pit_start_counter(uint32_t frequency, uint8_t counter, uint8_t mode);
void pit_init();
bool pit_is_init();

#endif
