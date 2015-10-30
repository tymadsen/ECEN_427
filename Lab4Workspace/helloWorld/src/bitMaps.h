/*
 * graphics.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef BITMAPS_H_
#define BITMAPS_H_

#include <stdint.h>

#define packword32(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b31 << 31) | (b30 << 30) | (b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) |    \
 (b25 << 25) | (b24 << 24) | (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) |    \
 (b17 << 17) | (b16 << 16) | (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) |    \
 (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) |    \
 (b1  << 1 ) | (b0  << 0 ))

#define packword30(b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) |    \
 (b25 << 25) | (b24 << 24) | (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) |    \
 (b17 << 17) | (b16 << 16) | (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) |    \
 (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) |    \
 (b1  << 1 ) | (b0  << 0 ))

#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword10(b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b9  << 9 ) | (b8  << 8) | (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword8(b7,b6,b5,b4,b3,b2,b1,b0) \
((b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword6(b5,b4,b3,b2,b1,b0) \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword5(b4,b3,b2,b1,b0) \
((b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword3(b2,b1,b0) \
((b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword2(b1,b0) \
((b1  << 1 ) | (b0  << 0 ) )

extern static const uint32_t saucer_16x7[];

extern static const uint32_t alien_explosion_12x10[];

extern static const uint32_t alien_top_in_12x8[];

extern static const uint32_t alien_top_out_12x8[];

extern static const uint32_t alien_middle_in_12x8[];

extern static const uint32_t alien_middle_out_12x8[];

extern static const uint32_t alien_bottom_in_12x8[];

extern static const uint32_t alien_bottom_out_12x8[];

extern static const uint32_t tank_15x8[];

// Shape of the entire bunker.
extern static const uint32_t bunker_24x18[];

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erod the block as you sequence through
// these patterns.
extern static const uint32_t bunkerDamage0_6x6[];

extern static const uint32_t bunkerDamage1_6x6[];

extern static const uint32_t bunkerDamage2_6x6[];

extern static const uint32_t bunkerDamage3_6x6[];

extern static const uint32_t word_score_30x5[];

extern static const uint32_t word_lives_24x5[];

extern static const uint32_t word_ga_24x10[];

extern static const uint32_t word_me_24x10[];

extern static const uint32_t word_ov_24x10[];

extern static const uint32_t word_er_24x10[];

extern static const uint32_t number_0_5x5[];

extern static const uint32_t number_1_5x5[];

extern static const uint32_t number_2_5x5[];

extern static const uint32_t number_3_5x5[];

extern static const uint32_t number_4_5x5[];

extern static const uint32_t number_5_5x5[];

extern static const uint32_t number_6_5x5[];

extern static const uint32_t number_7_5x5[];

extern static const uint32_t number_8_5x5[];

extern static const uint32_t number_9_5x5[];

extern static const uint32_t alien_bullet_11_3x7[];

extern static const uint32_t alien_bullet_12_3x7[];

extern static const uint32_t alien_bullet_13_3x7[];

extern static const uint32_t alien_bullet_21_3x7[];

extern static const uint32_t alien_bullet_22_3x7[];

extern static const uint32_t alien_bullet_23_3x7[];

extern static const uint32_t tank_bullet_1x3[];

extern static const uint32_t tank_death_1_32x16[];

extern static const uint32_t tank_death_2_32x16[];

extern static const uint32_t tank_death_1_16x8[];

extern static const uint32_t tank_death_2_16x8[];

#endif /* BITMAPS_H_ */
