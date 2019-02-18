#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"

#define CMOS_RW 0x71
#define INDEX_PORT 0x70
#define STATUS_REG_A 0x8A
#define STATUS_REG_B 0x8B
#define STATUS_REG_C 0x8C
#define IRQ8 0x08
#define IRQ2 0x02

/* interrupt rates */
#define R_2HZ 0x0F
#define R_4HZ 0x0E
#define R_8HZ 0x0D
#define R_16HZ 0x0C
#define R_32HZ 0x0B
#define R_64HZ 0x0A
#define R_128HZ 0x09
#define R_256HZ 0x08
#define R_512HZ 0x07
#define R_1024HZ 0x06

extern void rtc_init();
extern void rtc_handler();
int32_t rtc_open(const uint8_t* filename);
int32_t rtc_close(int32_t fd);
int32_t rtc_read(int32_t fd, void* buf, int32_t  nbytes);
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);

#endif /* _RTC_H */

