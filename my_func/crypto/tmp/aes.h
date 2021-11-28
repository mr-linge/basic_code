#ifndef __AES_H
#define __AES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//加密
bool EncryptDataToCipherTxt(uint8_t *orign, uint8_t *result, uint16_t length);

//解密
bool DecryptCipherTxtToData(uint8_t *orign, uint8_t *result, uint16_t length);

#endif
