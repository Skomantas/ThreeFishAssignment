#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>


/* rotation constants */

#define RotConst_0_1  14
#define RotConst_0_2  16

#define RotConst_1_1  52
#define RotConst_1_2  57

#define RotConst_2_1  23
#define RotConst_2_2  40

#define RotConst_3_1  5
#define RotConst_3_2  37

#define RotConst_4_1  25
#define RotConst_4_2  33

#define RotConst_5_1  46
#define RotConst_5_2  12

#define RotConst_6_1  58
#define RotConst_6_2  22

#define RotConst_7_1  32
#define RotConst_7_2  32

/* ------------------ */



/* Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */

/* Under key at k and tweak at t, encrypt 32 bytes of plaintext at p and store it at c. */
void threefish(unsigned char *c, const unsigned char *p, const unsigned char *k, const unsigned char *t);
