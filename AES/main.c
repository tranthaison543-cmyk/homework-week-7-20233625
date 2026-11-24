
#include <stdio.h>
#include <stdint.h>

typedef uint8_t byte;

// 1. BANG S-BOX VA RCON

static const byte sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const byte rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1B, 0x36
};

// In ma tran State theo thu tu hang va cot
void print_state(const char *label, const byte state[16])
{
    printf("%s\n", label);

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            printf("%02X ", state[col * 4 + row]);
        }
        printf("\n");
    }

    printf("\n");
}

// In 11 khoa vong
void print_round_keys(const byte round_keys[176])
{
    for (int round = 0; round <= 10; round++) {
        printf("Round Key %d:\n", round);
        print_state("", &round_keys[round * 16]);
    }
}


// AddRoundKey
void add_round_key(byte state[16], const byte round_key[16])
{
    for (int i = 0; i < 16; i++) {
        state[i] ^= round_key[i];
    }
}

// SubBytes
void sub_bytes(byte state[16])
{
    for (int i = 0; i < 16; i++) {
        state[i] = sbox[state[i]];
    }
}

// ShiftRows
void shift_rows(byte state[16])
{
    byte temp[16];

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            temp[col * 4 + row] =
                state[((col + row) % 4) * 4 + row];
        }
    }

    for (int i = 0; i < 16; i++) {
        state[i] = temp[i];
    }
}

// Nhan mot byte voi 02 trong GF(2^8)
byte xtime(byte x)
{
    if (x & 0x80) {
        return (byte)((x << 1) ^ 0x1B);
    }

    return (byte)(x << 1);
}

// Nhan mot byte voi 03 trong GF(2^8)
byte multiply_by_03(byte x)
{
    return xtime(x) ^ x;
}

// MixColumns
void mix_columns(byte state[16])
{
    for (int col = 0; col < 4; col++) {
        int i = col * 4;

        byte a0 = state[i];
        byte a1 = state[i + 1];
        byte a2 = state[i + 2];
        byte a3 = state[i + 3];

        state[i]     = xtime(a0) ^ multiply_by_03(a1) ^ a2 ^ a3;
        state[i + 1] = a0 ^ xtime(a1) ^ multiply_by_03(a2) ^ a3;
        state[i + 2] = a0 ^ a1 ^ xtime(a2) ^ multiply_by_03(a3);
        state[i + 3] = multiply_by_03(a0) ^ a1 ^ a2 ^ xtime(a3);
    }
}

//  KEY EXPANSION

// RotWord: xoay trai mot tu 4 byte
void rot_word(byte word[4])
{
    byte temp = word[0];

    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

// SubWord: thay the tung byte bang S-box
void sub_word(byte word[4])
{
    for (int i = 0; i < 4; i++) {
        word[i] = sbox[word[i]];
    }
}

// Sinh 11 khoa vong AES-128
void key_expansion(const byte key[16], byte round_keys[176])
{
    for (int i = 0; i < 16; i++) {
        round_keys[i] = key[i];
    }

    byte temp[4];

    for (int i = 4; i < 44; i++) {
        for (int j = 0; j < 4; j++) {
            temp[j] = round_keys[(i - 1) * 4 + j];
        }

        if (i % 4 == 0) {
            rot_word(temp);
            sub_word(temp);
            temp[0] ^= rcon[i / 4 - 1];
        }

        for (int j = 0; j < 4; j++) {
            round_keys[i * 4 + j] =
                round_keys[(i - 4) * 4 + j] ^ temp[j];
        }
    }
}

//  CAC VONG MA HOA AES
// Vong AES thong thuong
void aes_round(byte state[16], const byte round_key[16])
{
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state);
    add_round_key(state, round_key);
}

// Vong AES cuoi cung: khong co MixColumns
void aes_final_round(byte state[16], const byte round_key[16])
{
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, round_key);
}

// Ma hoa AES-128
void aes_encrypt(byte state[16], const byte round_keys[176])
{
    // AddRoundKey ban dau
    add_round_key(state, round_keys);

    // Vong 1 den vong 9
    for (int round = 1; round < 10; round++) {
        aes_round(state, &round_keys[round * 16]);
    }

    // Vong 10
    aes_final_round(state, &round_keys[160]);
}


int main(void)
{
    // Ban ro 128 bit cua bai tap tuan 11
    byte plaintext[16] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x11
    };

    // Khoa AES-128: 128 bit
    byte key[16] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    byte round_keys[176];

    // Sinh khoa vong
    key_expansion(key, round_keys);

    // In cac khoa vong
    print_round_keys(round_keys);

    // In ban ro
    print_state("Trang thai ban ro:", plaintext);

    // Ma hoa AES-128
    aes_encrypt(plaintext, round_keys);

    // In ban ma
    print_state("Ciphertext:", plaintext);

    return 0;
}
