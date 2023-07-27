#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PRIME_MODULUS 2147483647
#define BASE_GENERATOR 16807

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

int main() {
    unsigned long alice_private_key = 0x50db7349; // Alice的私钥
    unsigned long bob_private_key = 0x28fea38d; // Bob的私钥
    
    unsigned long alice_public_key = perform_dh_key_exchange(alice_private_key, BASE_GENERATOR);
    unsigned long bob_public_key = perform_dh_key_exchange(bob_private_key, BASE_GENERATOR);

    // Alice和Bob交换公钥后计算共享密钥
    unsigned long alice_shared_secret = mod_exp(bob_public_key, alice_private_key, PRIME_MODULUS);
    unsigned long bob_shared_secret = mod_exp(alice_public_key, bob_private_key, PRIME_MODULUS);

    printf("Alice的共享密钥:%lx\n", alice_shared_secret);
    printf("Bob  的共享密钥:%lx\n", bob_shared_secret);

    return 0;
}
