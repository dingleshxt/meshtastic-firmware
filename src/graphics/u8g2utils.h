#ifndef U8G2UTILS_H
#define U8G2UTILS_H

#include <U8g2lib.h>

#define CHGR_WIDTH 128
#define CHGR_HEIGHT 64
extern bool ch_bitmap[CHGR_HEIGHT][CHGR_WIDTH];

uint8_t bit_set_to(uint8_t number, uint8_t n, bool x) {
    return (number & ~((uint8_t)1 << n)) | ((uint8_t)x << n);
}
uint8_t bit_set(uint8_t number, uint8_t n) {
    return number | ((uint8_t)1 << n);
}

#endif