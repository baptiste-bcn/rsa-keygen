# Projet de Cryptographie en C

Ce projet implémente des opérations de base de la cryptographie RSA en C. Il permet de générer des clés RSA, de chiffrer et déchiffrer à la fois des entiers en hexadécimal et des fichiers textes.

## Prérequis

Pour compiler et exécuter ce projet, assurez-vous d'avoir un compilateur C installé sur votre machine.

## Installation

Clonez ce dépôt et naviguez dans le dossier du projet :

```
git clone <https://grond.iut-fbleau.fr/blanchonb/R4.B.10.git>
cd R4.B.10
```

Pour compiler le projet, exécutez :

```
make
```

Pour supprimer les fichiers liés à la compilation, exécutez :

```
make clean
```

## Utilisation

### Génération de clés RSA

Pour générer une paire de clés RSA (publique et privée), exécutez :

```
./key_gen
```

Les fichiers `key.pub` (clé publique) et `key.priv` (clé privée) seront générés dans le répertoire courant.


### Chiffrement et Déchiffrement d'un Entier en Hexadécimal

Le fichier nécessaire pour tester le chiffrement et le déchiffrement d'un entier en hexadécimal est fourni (`hexfile.in`). Vous avez également la possibilité de modifier son contenu.

Pour chiffrer l'entier, exécutez :

```
./crypt_rsa -e key.pub hexfile.in hexfile.out
```

Pour déchiffrer l'entier, exécutez :

```
./crypt_rsa -d key.priv hexfile.out hexfile.decrypted
```

Le fichier `hexfile.decrypted` contiendra l'entier original en hexadécimal.

### Chiffrement et Déchiffrement d'un Fichier Texte

Le fichier nécessaire pour tester le chiffrement et le déchiffrement d'un fichier texte est  fourni (`textfile.in`). Vous avez également la possibilité de modifier son contenu.

Pour chiffrer le fichier texte, exécutez :

```
./crypt_rsa_file -e key.pub textfile.in textfile.out
```

Pour déchiffrer le fichier texte, exécutez :

```
./crypt_rsa_file -d key.priv textfile.out textfile.decrypted
```

Le fichier `textfile.decrypted` contiendra le message original.


---

© Baptiste BLANCHON, Emilien LEDUN 
- [Sujet disponible ici](https://grond.iut-fbleau.fr/monnerat/R4.B.10_2024/src/branch/master/miniprojet/p1).