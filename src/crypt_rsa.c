#include <stdio.h>
#include <string.h>
#include "big.h"
#include "common.h"

// Fonction pour lire un nombre big_n à partir d'un fichier au format hexadécimal

void read_hex_bign_from_file(const char *filepath, big_n number)
{
    FILE *file;

    file = fopen(filepath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s en lecture.", filepath);
        exit(EXIT_FAILURE);
    }

    read_hex(file, number);
    fclose(file);
}

// Fonction pour écrire un nombre big_n dans un fichier au format hexadécimal

void write_hex_bign_to_file(const char *filepath, big_n number)
{
    FILE *file;

    file = fopen(filepath, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s en écriture.", filepath);
        exit(EXIT_FAILURE);
    }

    write_hex(file, number, '\n');
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s -e|-d file.key file.in file.out\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int encrypt = 0;

    if (strcmp(argv[1], "-e") == 0)
    {
        encrypt = 1;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        encrypt = 0;
    }
    else
    {
        fprintf(stderr, "Option invalide: %s\n", argv[1]);
        fprintf(stderr, "Utilisation: %s -e|-d file.key file.in file.out\n", argv[0]);
        return 1;
    }

    char *keyFileName = argv[2];
    char *inputFileName = argv[3];
    char *outputFileName = argv[4];

    printf("Mode: %s\n", encrypt ? "Chiffrement" : "Déchiffrement");
    printf("Fichier de clé: %s\n", keyFileName);
    printf("Fichier d'entrée: %s\n", inputFileName);
    printf("Fichier de sortie: %s\n", outputFileName);

    big_n n, e, d, result, int_to_decrypt;

    if (encrypt)
    {
        load_public_key(keyFileName, n, e);
        big_out(4, "n = ", n, stdout, '\0', 0, 0, 80, 10, 0, 0);
        big_out(4, "e = ", e, stdout, '\0', 0, 0, 80, 10, 0, 0);
        big_n int_to_encrypt;
        read_hex_bign_from_file(inputFileName, int_to_encrypt);
        big_out(6, "int = ", int_to_encrypt, stdout, '\0', 0, 0, 80, 10, 0, 0);
        encrypt_hex_bign(int_to_encrypt, n, e, result);
        printf("Integer has been encrypted.\n");
        write_hex_bign_to_file(outputFileName, result);
    }
    else
    {
        load_private_key(keyFileName, n, e, d);
        big_out(4, "n = ", n, stdout, '\0', 0, 0, 80, 10, 0, 0);
        big_out(4, "e = ", e, stdout, '\0', 0, 0, 80, 10, 0, 0);
        big_out(4, "d = ", d, stdout, '\0', 0, 0, 80, 10, 0, 0);
        read_hex_bign_from_file(inputFileName, int_to_decrypt);
        big_out(6, "int = ", int_to_decrypt, stdout, '\0', 0, 0, 80, 10, 0, 0);
        modpow(int_to_decrypt, d, n, result);
        printf("Integer has been decrypted.\n");
        write_hex_bign_to_file(outputFileName, result);
    }
    return 0;
}