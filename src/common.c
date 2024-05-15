#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "big.h"

// Initialise un nombre big_n avec une valeur entière

void init_bign(big_n result, int value)
{
    zero_big(result);
    add_int(value, result, result);
}

// Calcule la puissance modulaire (base^exponent % modulo)

void modpow(big_n base, big_n exponent, big_n modulo, big_n result)
{
    // https://www.dcode.fr/modular-exponentiation
    big_n b, e, m, zero, one, two;

    init_bign(result, 1);
    init_bign(zero, 0);
    init_bign(one, 1);
    init_bign(two, 2);

    copy_big(base, b);
    copy_big(exponent, e);
    copy_big(modulo, m);

    // b = b % m
    mod_big(m, b, b);

    if (cp_abs(b, zero) == 0)
    {
        // return zero
        init_bign(result, 0);
        return;
    }
    // e > zero
    while (cp_abs(e, zero) == -1)
    {
        big_n e_is_even;
        init_bign(e_is_even, 0);

        // e % 2
        mod_big(two, e, e_is_even);

        // e % 2 == 1
        if (cp_abs(e_is_even, one) == 0)
        {
            big_n mult_res;
            init_bign(mult_res, 0);
            // r * b
            mult_big(result, b, mult_res);
            // (r * b) % m
            mod_big(m, mult_res, result);
        }

        // e >> 1
        div_int(2, e, e);

        // b ** 2
        big_n pow_res;
        mult_big(b, b, pow_res);
        // (b ** 2) % m
        mod_big(m, pow_res, b);
    }
}

// Charge la clé privée à partir d'un fichier

void load_private_key(const char *filename, big_n n, big_n e, big_n d)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    read_hex(file, n);
    read_hex(file, e);
    read_hex(file, d);

    fclose(file);
}

// Charge la clé publique à partir d'un fichier

void load_public_key(const char *filename, big_n n, big_n e)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    read_hex(file, n);
    read_hex(file, e);

    fclose(file);
}

// Chiffre un nombre hexadécimal avec la clé publique

void encrypt_hex_bign(big_n hex_to_encrypt, big_n n, big_n e, big_n result)
{
    big_n zero;
    init_bign(zero, 0);
    // Vérification que 0 <= m < n
    if (cp_abs(zero, hex_to_encrypt) == -1 || cp_abs(hex_to_encrypt, n) <= 0)
    {
        fprintf(stderr, "L'entier m ne peux pas être chiffré. 0 <= entier < n");
        exit(EXIT_FAILURE);
    }
    modpow(hex_to_encrypt, e, n, result);
}

// Déchiffre un nombre hexadécimal avec la clé privée

void decrypt_hex_bign(big_n integer_to_decrypt, big_n d, big_n n, big_n result)
{
    big_n zero;
    init_bign(zero, 0);
    // Vérification que 0 <= m < n
    if (cp_abs(zero, integer_to_decrypt) == -1 || cp_abs(integer_to_decrypt, n) <= 0)
    {
        fprintf(stderr, "L'entier m ne peux pas être déchiffré. 0 <= entier < n");
        exit(EXIT_FAILURE);
    }
    modpow(integer_to_decrypt, d, n, result);
}