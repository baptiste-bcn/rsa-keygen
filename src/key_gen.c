#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "big.h"
#include "common.h"

// Vérifie si un nombre big_n est pair

int is_bign_even(big_n number)
{
    big_n zero, two, result;
    init_bign(zero, 0);
    init_bign(two, 2);
    init_bign(result, 0);

    mod_big(two, number, result);
    return cp_abs(result, zero) == 0;
}

// Génère un nombre big_n aléatoire de la taille spécifiée en bits

void generate_random_bign(int bits, big_n result)
{
    zero_big(result); // Initialise le résultat à zéro.

    for (int i = 0; i < bits;)
    {
        int byte = rand() % 256; // Génère un octet aléatoire.
        int size = 8;            // Initialise la taille à 8 bits.
        // Ajuste la taille si le nombre restant de bits à générer est inférieur à 8.
        if (bits - i < 8)
        {
            size = bits - i;
        }

        byte &= (1 << size) - 1; // Applique un masque pour ne conserver que les 'size' bits inférieurs.

        int index = i / 32;                   // Calcule l'indice du tableau 'result' où écrire.
        int offset = i % 32;                  // Calcule le décalage nécessaire dans l'élément du tableau.
        result[index] |= (int)byte << offset; // Positionne les bits dans le résultat.

        i += size; // Incrémente 'i' par la taille ajoutée.
    }
}

// Génère un nombre big_n aléatoire dans la plage spécifiée

void generate_random_bign_in_range(big_n min, big_n max, big_n result)
{
    big_n range;
    // Calcul de l'étendue de l'intervalle.
    sub_big(max, min, range);
    add_int(1, range, range); // Ajustement pour inclure max.

    // Génération d'un nombre aléatoire adapté à l'intervalle.
    generate_random_bign(mssb(range), result);
    // Réduction pour assurer que le résultat soit dans [0, range-1].
    mod_big(result, range, result);
    // Ajustement pour décaler l'intervalle à [min, max].
    add_big(min, result, result);
}

// Calcule le PGCD de deux nombres big_n

void pgcd_big(big_n bn1, big_n bn2, big_n result)
{
    big_n t, zero;
    init_bign(t, 0);
    init_bign(zero, 0);

    big_n a, b;
    copy_big(bn1, a);
    copy_big(bn2, b);

    // Algorithme d'Euclide pour le PGCD
    while (cp_abs(b, zero) != 0)
    {
        copy_big(b, t);   // t = b
        mod_big(b, a, b); // b = a % b
        copy_big(t, a);   // a = t
    }

    copy_big(a, result);
}

// Vérifie si un nombre est un témoin de Miller-Rabin

int miller_witness(big_n a, big_n n, big_n n_minus_one, big_n d, int s)
{
    big_n x;
    modpow(a, d, n, x); // Calcule a^d mod n

    big_n one;
    init_bign(one, 1);

    big_n minus_one;
    copy_big(n_minus_one, minus_one);

    if (cp_abs(x, one) == 0 || cp_abs(x, minus_one) == 0)
    {
        return 0;
    }

    for (int j = 0; j < s - 1; j++)
    {
        mult_big(x, x, x); // x = x^2 mod n
        mod_big(n, x, x);

        if (cp_abs(x, one) == 0)
        {
            return 1; // n est composé
        }

        if (cp_abs(x, minus_one) == 0)
        {
            return 0; // n passe le test pour ce témoin
        }
    }

    return 1; // n est composé si on sort de la boucle sans retourner 0
}

// Test de primalité de Miller-Rabin

int miller_test(big_n n, int k)
{
    big_n two;
    init_bign(two, 2);

    if (is_bign_even(n) || cp_abs(n, two) == 1)
    {
        return 0; // n est pair ou inférieur à 2, donc pas premier
    }

    big_n n_minus_one;
    copy_big(n, n_minus_one);
    sub_int(1, n_minus_one, n_minus_one); // n_minus_one = n - 1

    big_n d;
    copy_big(n_minus_one, d);

    int s = 0;
    while (is_bign_even(d))
    { // Trouver d tel que n - 1 = 2^s * d avec d impair
        s++;
        big_shift(-1, d, d);
    }

    for (int i = 0; i < k; i++)
    {
        big_n a;
        big_n min, max;
        zero_big(min);
        min[0] = 2; // min = 2
        copy_big(n_minus_one, max);
        sub_int(1, max, max);                       // max = n - 2
        generate_random_bign_in_range(min, max, a); // a aléatoire dans [2, n - 2]

        if (miller_witness(a, n, n_minus_one, d, s))
        {
            return 0; // n est composé
        }
    }

    return 1; // n est probablement premier
}

// Génère un nombre premier big_n de la taille spécifiée en bits

void get_prime_bign(int bits, int k, big_n result)
{
    generate_random_bign(bits, result);

    result[(bits - 1) / 32] |= 1 << ((bits - 1) % 32);

    result[0] |= 1;

    while (!miller_test(result, k))
    {
        add_int(2, result, result);
    }
}

// Vérifie si deux nombres sont premiers entre eux

int are_coprime_big(big_n a, big_n b)
{
    big_n pgcd, one;
    init_bign(one, 1);
    init_bign(pgcd, 0);
    pgcd_big(a, b, pgcd);
    return cp_abs(pgcd, one) == 0;
}

// Calcule l'algorithme étendu d'Euclide

void extended_euclid_big(big_n a, big_n b, big_n r, big_n u, big_n v)
{
    big_n r_prime, u_prime, v_prime, q, rs, us, vs, zero;

    init_bign(q, 0);
    init_bign(rs, 0);
    init_bign(us, 0);
    init_bign(vs, 0);
    init_bign(zero, 0);

    copy_big(a, r);        // r = a
    copy_big(b, r_prime);  // r' = b
    init_bign(u, 1);       // u = 1
    init_bign(v, 0);       // v = 0
    init_bign(u_prime, 0); // u' = 0
    init_bign(v_prime, 1); // v' = 1

    // Tant que r' != 0
    while (cp_abs(r_prime, zero) != 0)
    {
        div_big(r_prime, r, q); // q = r / r'

        // Mise à jour de rs, us, vs
        copy_big(r, rs); // rs = r
        copy_big(u, us); // us = u
        copy_big(v, vs); // vs = v

        // Mise à jour de r, u, v
        copy_big(r_prime, r); // r = r'
        copy_big(u_prime, u); // u = u'
        copy_big(v_prime, v); // v = v'

        // Mise à jour de r', u', v'
        mult_big(q, r_prime, r_prime);
        sub_big(r_prime, rs, r_prime); // r' = rs - q * r'

        mult_big(q, u_prime, u_prime);
        sub_big(u_prime, us, u_prime); // u' = us - q * u'

        mult_big(q, v_prime, v_prime);
        sub_big(v_prime, vs, v_prime); // v' = vs - q * v'
    }
}

// Génère une paire de clés RSA

void generate_rsa_key(big_n n_res, big_n e_res, big_n d_res)
{
    // 1 - Choisir p et q, deux nombres premiers distincts
    big_n p;
    big_n q;
    init_bign(p, 0);
    init_bign(q, 0);
    get_prime_bign(512, 100, p);
    get_prime_bign(512, 100, q);
    // 2 - Calculer leur produit n = pq, appelé module de chiffrement
    big_n n;
    mult_big(p, q, n);
    printf("n = ");
    // 3 - Calculer φ(n) = (p - 1)(q - 1) (c'est la valeur de l'indicatrice d'Euler en n) ;
    big_n phi, p_minus_one, q_minus_one;
    init_bign(phi, 0);
    init_bign(p_minus_one, 0);
    init_bign(q_minus_one, 0);
    sub_int(1, p, p_minus_one);
    sub_int(1, q, q_minus_one);
    mult_big(q_minus_one, p_minus_one, phi);
    // 4 - choisir un entier naturel e premier avec φ(n) et strictement inférieur à φ(n), appelé exposant de chiffrement ;
    big_n e;
    init_bign(e, 65537);

    while (!are_coprime_big(phi, e))
    {
        add_int(1, e, e);
    }
    // 5 - calculer l'entier naturel d, inverse modulaire de e pour la multiplication modulo φ(n) et strictement inférieur à φ(n), appelé exposant de déchiffrement
    big_n r, u, v, d, zero;
    init_bign(r, 0);
    init_bign(u, 0);
    init_bign(v, 0);
    init_bign(d, 0);
    init_bign(zero, 0);
    extended_euclid_big(e, phi, r, u, v);
    while (u[PREC] != 0)
    {
        add_big(phi, u, u);
    }
    copy_big(u, d);
    // Copie des résultats
    copy_big(e, e_res);
    copy_big(d, d_res);
    copy_big(n, n_res);
}

// Enregistrer les clés générées dans des fichiers

void save_keys(big_n n, big_n e, big_n d)
{
    FILE *pubFile, *privFile;

    // Écriture de la clé publique
    pubFile = fopen("key.pub", "w");
    if (!pubFile)
    {
        perror("Erreur à l'ouverture de key.pub");
        return;
    }
    write_hex(pubFile, n, '\n');
    write_hex(pubFile, e, '\n');
    fclose(pubFile);

    // Écriture de la clé privée
    privFile = fopen("key.priv", "w");
    if (!privFile)
    {
        perror("Erreur à l'ouverture de key.priv");
        return;
    }
    write_hex(pubFile, n, '\n');
    write_hex(pubFile, e, '\n');
    write_hex(pubFile, d, '\n');
    fclose(privFile);

    printf("Les clés ont été sauvegardées avec succès.\n");
}

int main()
{
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    big_n n, e, d;
    generate_rsa_key(n, e, d);
    save_keys(n, e, d);
    big_out(4, "n = ", n, stdout, '\0', 0, 0, 80, 10, 0, 0);
    big_out(4, "e = ", e, stdout, '\0', 0, 0, 80, 10, 0, 0);
    big_out(4, "d = ", d, stdout, '\0', 0, 0, 80, 10, 0, 0);
}