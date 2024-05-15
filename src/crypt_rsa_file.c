#include <stdio.h>
#include <string.h>
#include "big.h"
#include "common.h"

// Fonction pour chiffrer un fichier avec une clé publique

void encrypt_file(big_n n, big_n e, FILE *input_file, FILE *output_file)
{

    // Calcul de la taille totale du fichier d'entrée
    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Écriture de la taille du fichier dans le fichier de sortie
    fwrite(&file_size, sizeof(file_size), 1, output_file);

    // Calcul des tailles pour le cryptage
    size_t bits = mssb(n);
    size_t encrypted_size = (bits + 7) / 8;
    size_t input_size = (bits - 1) / 8;

    while (1)
    {
        big_n input_data;
        zero_big(input_data);
        size_t bytes_read = fread(input_data, 1, input_size, input_file);

        if (bytes_read <= 0)
        {
            break;
        }

        big_n encrypted_data;
        encrypt_hex_bign(input_data, n, e, encrypted_data);

        fwrite(encrypted_data, 1, encrypted_size, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

// Fonction pour déchiffrer un fichier avec une clé privée

void decrypt_file(big_n n, big_n d, FILE *input_file, FILE *output_file)
{
    // Lecture de la taille totale du contenu du fichier d'entrée
    size_t total_size;
    fread(&total_size, sizeof(total_size), 1, input_file);

    // Calcul des tailles pour le décryptage
    size_t bits = mssb(n);
    size_t encrypted_size = (bits + 7) / 8;
    size_t output_size = (bits - 1) / 8;
    size_t total_written = 0;

    while (1)
    {
        big_n encrypted_data;
        zero_big(encrypted_data);
        size_t bytes_read = fread(encrypted_data, 1, encrypted_size, input_file);

        if (bytes_read <= 0)
        {
            break;
        }

        big_n decrypted_data;
        decrypt_hex_bign(encrypted_data, d, n, decrypted_data);

        // Écriture des données décryptées, ajustement de la taille si nécessaire
        fwrite(decrypted_data, 1, (total_written + output_size > total_size) ? total_size - total_written : output_size, output_file);
        total_written += output_size;
    }

    fclose(input_file);
    fclose(output_file);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s -e|-d file.key file.in file.out\n", argv[0]);
        return 1;
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

    // Ouverture du fichier d'entrée pour lecture
    FILE *file_in = fopen(inputFileName, "r");
    if (file_in == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier d'entrée %s pour lecture.\n", inputFileName);
        exit(EXIT_FAILURE);
    }

    // Ouverture du fichier de sortie pour écriture
    FILE *file_out = fopen(outputFileName, "w");
    if (file_out == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de sortie %s pour écriture.\n", outputFileName);
        exit(EXIT_FAILURE);
    }

    big_n n, e, d;

    if (encrypt)
    {
        load_public_key(keyFileName, n, e);
        big_out(4, "n = ", n, stdout, '\0', 0, 0, 80, 10, 0, 0);
        big_out(4, "e = ", e, stdout, '\0', 0, 0, 80, 10, 0, 0);
        encrypt_file(n, e, file_in, file_out);
    }
    else
    {
        load_private_key(keyFileName, n, e, d);
        big_out(4, "n = ", n, stdout, '\0', 0, 0, 80, 10, 0, 0);
        big_out(4, "e = ", e, stdout, '\0', 0, 0, 80, 10, 0, 0);
        big_out(4, "d = ", d, stdout, '\0', 0, 0, 80, 10, 0, 0);
        decrypt_file(n, d, file_in, file_out);
    }
    return 0;
}