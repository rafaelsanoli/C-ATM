#ifndef STORAGE_H
#define STORAGE_H

#define MAX_CONTAS 10 

typedef struct {
    char nome[50]; 
    char id[10];
    char senha_hash[65]; 
    double saldo;
} Conta;

int carregar_contas(Conta contas[], int max_contas_parametro); 

#endif // STORAGE_H