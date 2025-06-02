#!/bin/bash

echo "Compilando..."

gcc ../src/main.c ../src/atm/auth.c ../src/atm/account.c ../src/atm/storage.c \
-I../include \
-I$(brew --prefix openssl)/include \
-I$(brew --prefix json-c)/include \
-L$(brew --prefix openssl)/lib \
-L$(brew --prefix json-c)/lib \
-lssl -lcrypto -ljson-c \
-o main && echo "Compilado com sucesso!" && ./main