#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "storage.h" // Para a struct Conta

int depositar(Conta *conta, double valor);
int sacar(Conta *conta, double valor);
int transferir(Conta *origem, Conta *destino, double valor);
void registrar_transacao(const char *tipo, const Conta *conta, double valor, const char *destino_id);

#endif