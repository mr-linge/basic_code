#include "dh_exchange.h"

// 计算模运算
unsigned long mod_exp(unsigned long base, unsigned long exponent, unsigned long modulus) {
    unsigned long result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent = exponent / 2;
    }
    return result;
}

// 执行Diffie-Hellman密钥协商
unsigned long perform_dh_key_exchange(unsigned long private_key, unsigned long base) {
    unsigned long public_key = mod_exp(base, private_key, PRIME_MODULUS);
    return public_key;
}
