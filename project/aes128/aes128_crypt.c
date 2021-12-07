#include "aes128_crypt.h"

// 矩阵 转 文本
void matrix_to_text(uint8_t text[16], uint8_t origin_matrix[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            text[4 * i + j] = origin_matrix[j][i];
        }
    }
}

void print_matrix(uint8_t target[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02X ", target[i][j]);
        }
        puts("");
    }
    puts("");
}

void split_block(Message *msg) {
    unsigned long block_num = msg->len / 16;
    short remainder = (short) (msg->len % 16);
//    printf("block_num = %lu\t remainder = %u\n", block_num, remainder);
    if (remainder != 0) {
        block_num++;
        uint8_t *new_p = (uint8_t *) realloc(msg->message, block_num * 16);
        if (!new_p) {
            printf("Not Enough Memory!\n");
            exit(-1);
        }
        // 尾部填充 0， 最后一字节是填充个数
        memset((new_p + (block_num * 16) - (16 - remainder)), '\0', (16 - remainder));
        *(new_p + (block_num * 16) - 1) = 16 - remainder;
        msg->message = new_p;
        msg->len = block_num * 16;
    }
}

// 判断是否有填充，以及填充个数
void handle_filling(Message *msg) {
    uint8_t *ptr = msg->message + msg->len - 1;
    uint8_t filling_num = *ptr;
    // 确定填充的真实性
    uint8_t is_filling = 0;
    if (filling_num != 0) {
        for (int i = 1; i < filling_num; ++i) {
            if (*(ptr - i) != '\0') {
                is_filling++;
            }
        }
    }
    // 修正真实数据个数
    if (is_filling == 0) {
        msg->len -= filling_num;
    }
}

void AES_ECB_encrypt(uint8_t key[16], Message *msg) {
    // 分隔成 块，不足16 字节 进行填充
    split_block(msg);

    uint8_t *ptr = msg->message;
    key_expansion(key);
    uint8_t state_matrix[4][4];
    for (int i = 0; i < msg->len / 16; ++i) {
        block_encrypt(ptr, state_matrix);
/*        puts("state_matrix:");
        print_matrix(state_matrix);
        puts("");*/
        matrix_to_text(ptr, state_matrix);
        ptr += 16;
    }
}

void AES_ECB_decrypt(uint8_t key[16], Message *msg) {
    uint8_t *ptr = msg->message;
    key_expansion(key);
    uint8_t state_matrix[4][4];
    for (int i = 0; i < msg->len / 16; ++i) {
        block_decrypt(ptr, state_matrix);
        matrix_to_text(ptr, state_matrix);
        ptr += 16;
    }
    handle_filling(msg);
}

uint8_t iv[16] = "abcdef0123456789";

// 两个数组里的数据依次 异或
void xor_arr(uint8_t msg[16], const uint8_t object[16]) {
    for (int i = 0; i < 16; ++i) {
        msg[i] = msg[i] ^ object[i];
    }
}

void AES_CBC_encrypt(uint8_t key[16], Message *msg) {
    // 分隔成 块，不足16 字节 进行填充
    split_block(msg);

    uint8_t *ptr = msg->message;
    key_expansion(key);
    uint8_t state_matrix[4][4];

    // 需要每次与明文异或的 数据
    uint8_t *obj_ptr = iv;
    for (int i = 0; i < msg->len / 16; ++i) {
        xor_arr(ptr, obj_ptr);
        block_encrypt(ptr, state_matrix);
/*        puts("state_matrix:");
        print_matrix(state_matrix);
        puts("");*/
        matrix_to_text(ptr, state_matrix);
        obj_ptr = ptr;
        ptr += 16;
    }
}

void AES_CBC_decrypt(uint8_t key[16], Message *msg) {
    uint8_t *ptr = msg->message;
    key_expansion(key);
    uint8_t state_matrix[4][4];

    // 需要每次与明文异或的 数据
    uint8_t *obj_ptr = (uint8_t *) calloc(msg->len, 1);
    strcat(obj_ptr, iv);
    strncat(obj_ptr, msg->message, msg->len - 16);
    for (int i = 0; i < msg->len / 16; ++i) {
        block_decrypt(ptr, state_matrix);
        matrix_to_text(ptr, state_matrix);
        xor_arr(ptr, obj_ptr);
        ptr += 16;
        obj_ptr += 16;
    }
    handle_filling(msg);
}
