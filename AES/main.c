#include <stdio.h>
#include <stdint.h>

typedef uint8_t byte;

// In ma trận State theo thứ tự hàng và cột.
void print_state(const char *label, const byte state[16])
{
    printf("%s\n", label);

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            printf("%02X ", state[col * 4 + row]);
        }

        printf("\n");
    }

    printf("\n");
}

int main(void)
{
    // Ban ro 128 bit cua bai tap tuan 11.
    byte plaintext[16] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x11
    };

    // Khoa AES-128: 128 bit.
    byte key[16] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
// Thuc hien phep XOR State voi khoa con.
void add_round_key(byte state[16], const byte round_key[16])
{
    for (int i = 0; i < 16; i++)
    {
        state[i] ^= round_key[i];
    }
}
    print_state("Trang thai ban ro:", plaintext);
    print_state("Khoa ban dau:", key);
    add_round_key(plaintext, key);

    print_state("Trang thai sau AddRoundKey:", plaintext);
    return 0;
}
