#include "random_prime.h"

// 检查一个数是否为质数
bool is_prime(unsigned long  number) {
    if (number <= 1) {
        return false;
    }

    for (unsigned long  i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

// 生成一个随机数
unsigned long  generate_random_number(unsigned long  min, unsigned long  max) {
    return min + rand() % (max - min + 1);
}

// 生成一个随机的大质数
unsigned long  generate_random_prime(unsigned long min, unsigned long max) {
    srand(time(NULL));
    unsigned long  random_number;
    do {
        random_number = generate_random_number(min, max);
    } while (!is_prime(random_number));

    return random_number;
}
