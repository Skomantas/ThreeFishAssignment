
#include "threefish.h"

/* Under key at k and tweak at t, encrypt 32 bytes of plaintext at p and store it at c. */




/* rot64 */

#define RotateLeft64(input, rotC) (((input) << (rotC)) | ((input) >> (64 - (rotC))))

/* ----- */



/* mix functions */

#define Mix(input1,input2,rotC) do {                \
    input1 += input2;                               \
    input2 = RotateLeft64(input2, rotC) ^ input1;   \
}while(0)



#define SubKeyMix(input1,input2,key1,key2,rotC) do {        \
    input1 += input2 + key1;                                \
    input2 += key2;                                         \
    input2 = RotateLeft64(input2, rotC) ^ input1;           \
}while(0)

/*
void SubKeyMix(unsigned char input1,unsigned char input2,unsigned char key1,unsigned char key2,int rotC) {
    input1 += input2 + key1;
    input2 += key2;
    input2 = RotateLeft64(input2, rotC) ^ input1;
}
*/

/* ------------ */




/*

    p -  32bytes of plaintext

    t - tweak

    k - key

    c - output


*/
void threefish(unsigned char *c, const unsigned char *p, const unsigned char *k, const unsigned char *t) {

/* Nezinau ka cia reiktu padaryti, kaip susieti k - key t - tweak. */
    uint64_t txt[4];
    uint64_t keyC[5];
    uint64_t tweakC[3];
    uint64_t newV[4];
    uint64_t keySC = 0x1BD11BDAA9FC1A22; // ensures that the extended key cannot be all zeroes

    memcpy(txt, p, 4 * sizeof(uint64_t));
    memcpy(keyC, k, 4 * sizeof(uint64_t));
    memcpy(tweakC, t, 2 * sizeof(uint64_t));


    int ip;
    for (ip = 0; ip < 4; ip++) {
        keySC ^= keyC[ip];
    }
    keyC[4] = keySC;

    tweakC[2]=tweakC[0] ^ tweakC[1]; // key schedule : key[4] and tweak[2]

    printf( "K %" PRIx64 "\n", keyC[0]);
    printf( "K %" PRIx64 "\n", keyC[1]);
    printf( "K %" PRIx64 "\n", keyC[2]);
    printf( "K %" PRIx64 "\n", keyC[3]);
    printf( "K %" PRIx64 "\n", keyC[4]);





    printf("T %" PRIx64 "\n", tweakC[0]);
    printf("T %" PRIx64 "\n", tweakC[1]);
    printf("T %" PRIx64 "\n", tweakC[2]);


    int i;
    for(i=0; i<9; i++) {

        /*
            mod 5 for key
            mod 3 for tweak
        */

        SubKeyMix(txt[0],txt[1],keyC[i % 5],keyC[(i+1) % 5]+tweakC[i % 3],RotConst_0_1);
        SubKeyMix(txt[2],txt[3],keyC[(i+2) % 5]+tweakC[(i+1) % 3],keyC[(i+3) % 5]+(2*i),RotConst_0_2);


        Mix(txt[0],txt[3],RotConst_1_1);
        Mix(txt[2],txt[1],RotConst_1_2);

        Mix(txt[0],txt[1],RotConst_2_1);
        Mix(txt[2],txt[3],RotConst_2_2);

        Mix(txt[0],txt[3],RotConst_3_1);
        Mix(txt[2],txt[1],RotConst_3_2);

        SubKeyMix(txt[0],txt[1],keyC[(i+1) % 5],keyC[(i+2) % 5]+tweakC[(i+1) % 3],RotConst_4_1);
        SubKeyMix(txt[2],txt[3],keyC[(i+3) % 5]+tweakC[(i+2) % 3],keyC[(i+4) % 5]+((2*i)+1),RotConst_4_2);

        Mix(txt[0],txt[3],RotConst_5_1);
        Mix(txt[2],txt[1],RotConst_5_2);

        Mix(txt[0],txt[1],RotConst_6_1);
        Mix(txt[2],txt[3],RotConst_6_2);

        Mix(txt[0],txt[3],RotConst_7_1);
        Mix(txt[2],txt[1],RotConst_7_2);

    }

    newV[0] = txt[0]+keyC[3];
    newV[1] = txt[1]+keyC[4] + t[0];
    newV[2] = txt[2]+keyC[0] + t[1];
    newV[3] = txt[3]+keyC[1] + 18;
    /* 18 from last round of iteration */


    memcpy(c, newV, 4 * sizeof(uint64_t)); //
}
