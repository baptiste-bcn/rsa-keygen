CC = gcc

CFLAGS = -Wall -g -I./lib

DEPS = lib/big.h

all: key_gen crypt_rsa crypt_rsa_file

key_gen: src/key_gen.o src/common.o lib/big.o
	$(CC) $(CFLAGS) -o $@ $^

crypt_rsa: src/crypt_rsa.o src/common.o lib/big.o
	$(CC) $(CFLAGS) -o $@ $^

crypt_rsa_file: src/crypt_rsa_file.o src/common.o lib/big.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o lib/*.o key_gen crypt_rsa crypt_rsa_file hexfile.out hexfile.decrypted textfile.out textfile.decrypted key.priv key.pub

.PHONY: clean