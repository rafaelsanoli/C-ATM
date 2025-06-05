#!/bin/bash

echo "Compilando o C-ATM..."

# Lista de todos os arquivos fonte .c
# Garanta que todos os seus módulos .c estejam aqui
SOURCES=(
    "../src/main.c"
    "../src/atm/auth.c"
    "../src/atm/account.c"
    "../src/atm/storage.c"
    "../src/atm/ui.c"
    "../src/atm/transaction.c" # Adicionado para as funções mock
    "../src/atm/security.c"
    # Adicione ../src/atm/utils.c aqui se você criar esse arquivo
)

# Opções do compilador
# -I../include diz ao GCC para procurar headers na pasta ../include
# As outras opções -I e -L são para OpenSSL e json-c via Homebrew no macOS
# -lssl -lcrypto são para OpenSSL
# -ljson-c é para json-c
COMPILER_FLAGS="-Wall -Wextra -g -I../include" # Adicionei -Wall -Wextra -g para debug e mais avisos
LINKER_FLAGS="-L$(brew --prefix openssl)/lib -L$(brew --prefix json-c)/lib -lssl -lcrypto -ljson-c"
INCLUDE_PATHS="-I$(brew --prefix openssl)/include -I$(brew --prefix json-c)/include"

# Comando de compilação
gcc ${COMPILER_FLAGS} ${INCLUDE_PATHS} ${SOURCES[@]} ${LINKER_FLAGS} -o main

# Verifica se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilado com sucesso!"
    echo "Executando o C-ATM..."
    ./main # Executa o programa
else
    echo "----------------------"
    echo "Falha na compilacao."
    echo "----------------------"
fi