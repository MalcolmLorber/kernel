#ifndef _PIC_INC
#define _PIC_INC

#include <stdint.h>

//PIC 1
#define PIC_IRQ_TIMER 0
#define PIC_IRQ_KEYBD 1
#define PIC_IRQ_SERL2 3
#define PIC_IRQ_SERL1 4
#define PIC_IRQ_PARL2 5
#define PIC_IRQ_DISKT 6
#define PIC_IRQ_PARL1 7
//PIC 2
#define PIC_IRQ_CMOST 0
#define PIC_IRQ_CGART 1
#define PIC_IRQ_AUXIL 4
#define PIC_IRQ_FPU   5
#define PIC_IRQ_HDC   6

//OCW2 masks
#define	PIC_OCW2_L1   0x01	        
#define	PIC_OCW2_L2   0x02
#define	PIC_OCW2_L3   0x04		
#define	PIC_OCW2_EOI  0x20
#define	PIC_OCW2_SL   0x40
#define	PIC_OCW2_ROT  0x80   
//OCW3 masks
#define	PIC_OCW3_RIS  0x01		
#define	PIC_OCW3_RIR  0x02		
#define	PIC_OCW3_MODE 0x04	
#define	PIC_OCW3_SMM  0x20
#define	PIC_OCW3_ESMM 0x40	
#define	PIC_OCW3_D7   0x80

//PIC1 reg ports
#define PIC1_REG_COMD 0x20
#define PIC1_REG_STAT 0x20
#define PIC1_REG_DATA 0x21
#define PIC1_REG_IMR  0x21
//PIC2 reg ports
#define PIC2_REG_COMD 0xA0
#define PIC2_REG_STAT 0xA0
#define PIC2_REG_DATA 0xA1
#define PIC2_REG_IMR  0xA1

//ICW1 masks
#define PIC_ICW1_IC4  0x01
#define PIC_ICW1_SNGL 0x02
#define PIC_ICW1_ADI  0x04
#define PIC_ICW1_LRIM 0x08
#define PIC_ICW1_INIT 0x10
//ICW4 masks
#define PIC_ICW4_UPM  0x01
#define PIC_ICW4_AEOI 0x02
#define PIC_ICW4_MS   0x04
#define PIC_ICW4_BUF  0x08
#define PIC_ICW4_SFNM 0x10

//ICW1 control bits
#define PIC_ICW1_IC4_EXPECT 0x01
#define PIC_ICW1_IC4_NO	    0x00
#define PIC_ICW1_SNGL_YES   0x02
#define PIC_ICW1_SNGL_NO    0x00
#define PIC_ICW1_ADI_INTVL4 0x04
#define PIC_ICW1_ADI_INTVL8 0x00	
#define PIC_ICW1_LTIM_LEVEL 0x08	
#define PIC_ICW1_LTIM_EDGE  0x00	
#define PIC_ICW1_INIT_YES   0x10
#define PIC_ICW1_INIT_NO    0x00
//ICW4 control bits
#define PIC_ICW4_UPM_86	    0x01
#define PIC_ICW4_UPM_MCS    0x00
#define PIC_ICW4_AEOI_AUTO  0x02
#define PIC_ICW4_AEOI_NO    0x00
#define PIC_ICW4_MS_MASTER  0x04
#define PIC_ICW4_MS_SLAVE   0x00
#define PIC_ICW4_BUF_YES    0x08
#define PIC_ICW4_BUF_NO	    0x00
#define PIC_ICW4_SFNM_NEST  0x10
#define PIC_ICW4_SFNM_NOT   0x00


uint8_t pic_read(uint16_t pic);
void pic_write(uint16_t pic, uint8_t data);
void pic_command(uint16_t pic, uint8_t cmd);
void pic_mask_irq(uint16_t pic, uint8_t irqmask);
void pic_init(uint8_t base0, uint8_t base1);

#endif
