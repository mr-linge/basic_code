#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PRIME_MODULUS 2147483647
#define BASE_GENERATOR 16807

// 计算模运算
unsigned long mod_exp(unsigned long base, unsigned long exponent, unsigned long modulus);
// 执行Diffie-Hellman密钥协商
unsigned long perform_dh_key_exchange(unsigned long private_key, unsigned long base);
