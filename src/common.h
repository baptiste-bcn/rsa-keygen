#ifndef COMMON_H
#define COMMON_H

#include "big.h"

void init_bign(big_n result, int value);

void modpow(big_n base, big_n exponent, big_n modulo, big_n result);

void load_private_key(const char *filename, big_n n, big_n e, big_n d);

void load_public_key(const char *filename, big_n n, big_n e);

void encrypt_hex_bign(big_n hex_to_encrypt, big_n n, big_n e, big_n result);

void decrypt_hex_bign(big_n integer_to_decrypt, big_n d, big_n n, big_n result);

#endif