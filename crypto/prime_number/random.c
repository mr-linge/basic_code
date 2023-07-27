#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
    unsigned long  random_number;
    do {
        random_number = generate_random_number(min, max);
    } while (!is_prime(random_number));

    return random_number;
}

int main() {
    srand(time(NULL));

    unsigned long min_range = 0x10000000000; // 范围下限
    unsigned long max_range = 0x1000000000000000; // 范围上限

    unsigned long random_prime = generate_random_prime(min_range, max_range);
    printf("随机生成的大质数:%lx\n", random_prime);

    return 0;
}