// ../../include/atm/auth.h

#ifndef AUTH_H
#define AUTH_H

#include "storage.h" 

// Função para autenticar usuário
int autenticarUsuario(const char* id, const char* senhaDigitada, Conta contas[], int tamanho);

// Função para gerar hash da senha digitada
void gerarHash(const char* senhaOriginal, char* senhaHashDestino);

int iniciarAutenticacao(Conta contas[], int tamanho, char* idAutenticado);

// NOVA função para encontrar índice da conta pelo ID
int encontrarIndiceConta(const char* id, Conta contas[], int tamanho);

#endif

