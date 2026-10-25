
#include <stdint.h>
#include <stdio.h>

/*
 * Multiply two elements in GF(2^m).
 * poly is the full irreducible polynomial, including x^m.
 */
static uint16_t gf_multiply(uint16_t a, uint16_t b,
                            unsigned int m, uint16_t poly)
{
    uint16_t result = 0;
    uint16_t high_bit = (uint16_t)(1u << (m - 1));
    uint16_t mask = (uint16_t)((1u << m) - 1u);
    uint16_t reduction = (uint16_t)(poly & mask);

    a &= mask;
    b &= mask;

    while (b != 0) {
        /* Add the current polynomial using XOR. */
        if (b & 1u) {
            result ^= a;
        }

        /* Multiply a by x. */
        uint16_t carry = (uint16_t)(a & high_bit);
        a = (uint16_t)((a << 1) & mask);

        /* Reduce modulo the irreducible polynomial. */
        if (carry != 0) {
            a ^= reduction;
        }

        b >>= 1;
    }

    return (uint16_t)(result & mask);
}

int main(void)
{
    unsigned int m;
    unsigned int a, b;
    uint16_t polynomial;

    printf("Polynomial multiplication over GF(2^m)\n");
    printf("1. GF(2^4), polynomial x^4 + x + 1 (0x13)\n");
    printf("2. GF(2^8), polynomial x^8 + x^4 + x^3 + x + 1 (0x11B)\n");
    printf("Choose field (1 or 2): ");

    if (scanf("%u", &m) != 1 || (m != 1 && m != 2)) {
        printf("Error: invalid field choice.\n");
        return 1;
    }

    if (m == 1) {
        m = 4;
        polynomial = 0x13;
    } else {
        m = 8;
        polynomial = 0x11B;
    }

    printf("Enter a and b in hexadecimal (e.g. 7 D): ");

    if (scanf("%x %x", &a, &b) != 2) {
        printf("Error: invalid input.\n");
        return 1;
    }

    unsigned int limit = 1u << m;

    if (a >= limit || b >= limit) {
        printf("Error: operands must be between 0 and 0x%X.\n",
               limit - 1);
        return 1;
    }

    uint16_t result = gf_multiply(
        (uint16_t)a, (uint16_t)b, m, polynomial
    );

    printf("Field: GF(2^%u)\n", m);
    printf("Polynomial: 0x%X\n", polynomial);
    printf("A = 0x%X, B = 0x%X\n", a, b);
    printf("A * B = 0x%X\n", result);

    return 0;
}
