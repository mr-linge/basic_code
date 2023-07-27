#include <stdio.h>
#include <string.h>
#include "aes128_crypt.h"

void test_aes128_EBC();
void test_aes128_CBC();

int main() {
    test_aes128_CBC();
    puts("\n*******************************");
    test_aes128_EBC();
    return 0;
}

void test_aes128_CBC() {
    uint8_t key_text[16] = {0x3C, 0xA1, 0x0B, 0x21, 0x57, 0xF0, 0x19, 0x16, 0x90, 0x2E, 0x13, 0x80, 0xAC, 0xC1, 0x07, 0xBD};
    char *plaintext_tmp = "abcdef1234567890abcdef1234567890abcdef1234567890a";
    unsigned long len = strlen(plaintext_tmp);
    printf("origin message len = %lu\n", len);
    uint8_t *plaintext_text = (uint8_t *) malloc(len);
    strcpy(plaintext_text, plaintext_tmp);
    Message *msg = (Message *) malloc(sizeof(Message));
    msg->message = plaintext_text;
    msg->len = len;
    AES_CBC_encrypt(key_text, msg);
    printf("len = %lu ciphertext:\n", msg->len);
    for (int i = 0; i < msg->len; ++i) {
        printf("%02X ", *(msg->message + i));
    }
    puts("");

    AES_CBC_decrypt(key_text, msg);
    printf("len = %lu plaintext:\n", msg->len);
    for (int i = 0; i < msg->len; ++i) {
        printf("%c", *(msg->message + i));
    }
     puts("");
}

void test_aes128_EBC() {
    uint8_t key_text[16] = {0x3C, 0xA1, 0x0B, 0x21, 0x57, 0xF0, 0x19, 0x16, 0x90, 0x2E, 0x13, 0x80, 0xAC, 0xC1, 0x07, 0xBD};
    char *plaintext_tmp = "abcdef1234567890abcdef1234567890abcdef1234567890a";
    unsigned long len = strlen(plaintext_tmp);
    printf("origin message len = %lu\n", len);
    uint8_t *plaintext_text = (uint8_t *) malloc(len);
    strcpy(plaintext_text, plaintext_tmp);
    Message *msg = (Message *) malloc(sizeof(Message));
    msg->message = plaintext_text;
    msg->len = len;
    AES_ECB_encrypt(key_text, msg);
    printf("len = %lu ciphertext:\n", msg->len);
    for (int i = 0; i < msg->len; ++i) {
        printf("%02X ", *(msg->message + i));
    }
    puts("");

    AES_ECB_decrypt(key_text, msg);
    printf("len = %lu plaintext:\n", msg->len);
    for (int i = 0; i < msg->len; ++i) {
        printf("%02X", *(msg->message + i));
    }
     puts("");
}
