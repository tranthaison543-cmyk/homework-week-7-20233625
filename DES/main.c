
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <ctype.h>
typedef uint64_t u64;
typedef uint32_t u32;


// Initial Permutation
static const int IP[64] = {
    57,49,41,33,25,17,9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7,
    56,48,40,32,24,16,8,0,
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6
};

// Final Permutation
static const int FP[64] = {
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41,9,49,17,57,25,
    32,0,40,8,48,16,56,24
};

// Expansion: 32 bit -> 48 bit
static const int E[48] = {
    31,0,1,2,3,4,
    3,4,5,6,7,8,
    7,8,9,10,11,12,
    11,12,13,14,15,16,
    15,16,17,18,19,20,
    19,20,21,22,23,24,
    23,24,25,26,27,28,
    27,28,29,30,31,0
};

// Permutation P
static const int P[32] = {
    15,6,19,20,28,11,27,16,
    0,14,22,25,4,17,30,9,
    1,7,23,13,31,26,2,8,
    18,12,29,5,21,10,3,24
};

// PC-1: 64 bit -> 56 bit

static const int PC1[56] = {
    56, 48, 40, 32, 24, 16,  8,
     0, 57, 49, 41, 33, 25, 17,
     9,  1, 58, 50, 42, 34, 26,
    18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
     6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28,
    20, 12,  4, 27, 19, 11,  3
};

// PC-2: 56 bit -> 48 bit
static const int PC2[48] = {
    13,16,10,23,0,4,
    2,27,14,5,20,9,
    22,18,11,3,25,7,
    15,6,26,19,12,1,
    40,51,30,36,46,54,
    29,39,50,44,32,47,
    43,48,38,55,33,52,
    45,41,49,35,28,31
};

// So bit dich vong trai trong 16 vong
static const int SHIFTS[16] = {
    1,1,2,2,2,2,2,2,
    1,2,2,2,2,2,2,1
};


static const int SBOX[8][4][16] = {
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};


// Vi tri bit trong bang duoc danh so tu 0, tinh tu trai sang phai.
static u64 permute(u64 input, int input_bits,
                   const int table[], int output_bits)
{
    u64 output = 0;

    for (int i = 0; i < output_bits; i++) {
        int position = table[i];
        u64 bit = (input >> (input_bits - 1 - position)) & 1ULL;

        output = (output << 1) | bit;
    }

    return output;
}


static u32 left_shift_28(u32 value, int shift)
{
    return ((value << shift) |
            (value >> (28 - shift))) & 0x0FFFFFFF;
}

static void generate_subkeys(u64 key, u64 subkeys[16])
{
    // PC-1: 64 bit -> 56 bit
    u64 key56 = permute(key, 64, PC1, 56);

    // Chia thanh C0 va D0
    u32 C = (u32)((key56 >> 28) & 0x0FFFFFFF);
    u32 D = (u32)(key56 & 0x0FFFFFFF);

    printf("\n===== KEY SCHEDULE =====\n");
    printf("C0 = %07X\n", C);
    printf("D0 = %07X\n", D);

    for (int i = 0; i < 16; i++) {
        // Dich vong trai
        C = left_shift_28(C, SHIFTS[i]);
        D = left_shift_28(D, SHIFTS[i]);

        // Ghep C va D
        u64 combined = ((u64)C << 28) | D;

        // PC-2: 56 bit -> 48 bit
        subkeys[i] = permute(combined, 56, PC2, 48);

        printf("K%-2d = %012llX\n",
               i + 1, (unsigned long long)subkeys[i]);
    }
}


static u32 feistel(u32 R, u64 subkey)
{
    // Expansion E: 32 bit -> 48 bit
    u64 expanded = permute(R, 32, E, 48);

    // XOR voi khoa con
    u64 x = expanded ^ subkey;

    // Chia thanh 8 nhom, moi nhom 6 bit
    u32 sbox_output = 0;

    for (int i = 0; i < 8; i++) {
        int six_bits = (int)((x >> (42 - 6 * i)) & 0x3F);

        // Bit dau va bit cuoi tao thanh hang
        int row = ((six_bits & 0x20) >> 4) |
                  (six_bits & 0x01);

        // Bon bit giua tao thanh cot
        int col = (six_bits >> 1) & 0x0F;

        int value = SBOX[i][row][col];

        sbox_output = (sbox_output << 4) | (u32)value;
    }

    // Hoan vi P
    return (u32)permute(sbox_output, 32, P, 32);
}


static u64 des_crypt(u64 block, const u64 subkeys[16], int decrypt)
{
    // Hoan vi ban dau IP
    u64 ip_result = permute(block, 64, IP, 64);

    u32 L = (u32)(ip_result >> 32);
    u32 R = (u32)ip_result;

    // 16 vong Feistel
    for (int i = 0; i < 16; i++) {
        u64 key;

        if (decrypt) {
            key = subkeys[15 - i];
        } else {
            key = subkeys[i];
        }

        u32 next_L = R;
        u32 next_R = L ^ feistel(R, key);

        L = next_L;
        R = next_R;

        printf("Round %2d: L = %08X  R = %08X\n",
               i + 1, L, R);
    }

    // Hoan doi hai nua: R16 || L16
    u64 pre_output = ((u64)R << 32) | L;

    // Hoan vi cuoi FP
    return permute(pre_output, 64, FP, 64);
}
static int input_hex64(const char *message, u64 *value)
{
    char buffer[100];
    char *end;

    printf("%s", message);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }

    char *start = buffer;

    // Bo khoang trang o dau
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // Chuyen hexadecimal sang so 64 bit
    unsigned long long result = strtoull(start, &end, 16);

    // Khong co ky tu hexadecimal
    if (start == end) {
        return 0;
    }

    // Luu do dai chuoi hex truoc khi bo khoang trang
    size_t hex_length = (size_t)(end - start);

    // Chi chap nhan toi da 16 ky tu hex
    if (hex_length > 16) {
        return 0;
    }

    // Bo khoang trang o cuoi
    while (isspace((unsigned char)*end)) {
        end++;
    }

    // Khong chap nhan ky tu khong hop le
    if (*end != '\0') {
        return 0;
    }

    *value = (u64)result;
    return 1;
}
int main(void)
{
    u64 plaintext, key;
    u64 subkeys[16];

    printf("========== DES ENCRYPTION ==========\n");

    // Nhap plaintext va key
    if (!input_hex64("Nhap Plaintext (hex): ", &plaintext)) {
        printf("Plaintext khong hop le!\n");
        return 1;
    }

    if (!input_hex64("Nhap Key (hex): ", &key)) {
        printf("Key khong hop le!\n");
        return 1;
    }

    printf("\nPlaintext = %016llX\n",
           (unsigned long long)plaintext);
    printf("Key       = %016llX\n",
           (unsigned long long)key);

    // Sinh 16 khoa con
    generate_subkeys(key, subkeys);

    // Ma hoa
    printf("\n===== 16 VONG MA HOA =====\n");
    u64 ciphertext = des_crypt(plaintext, subkeys, 0);

    printf("\nCiphertext = %016llX\n",
           (unsigned long long)ciphertext);

    // Giai ma
    printf("\n===== KIEM TRA GIAI MA =====\n");
    u64 recovered = des_crypt(ciphertext, subkeys, 1);

    printf("\nRecovered plaintext = %016llX\n",
           (unsigned long long)recovered);

    if (recovered == plaintext) {
        printf("DES decryption: PASS\n");
    } else {
        printf("DES decryption: FAIL\n");
    }

    return 0;
}
