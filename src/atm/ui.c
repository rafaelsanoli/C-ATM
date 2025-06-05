#include <stdio.h>
#include <stdlib.h>
#include "atm/account.h"
#include "atm/auth.h"
#include "atm/security.h"
#include "atm/storage.h
#include "atm/transaction.h"
#include "atm/ui.h"

int () {
	int opcao;
	int usuario_logado = 0;
	
	do{
	system("clear || cls");
	printf("1. login\n");
	printf("2. Ver saldo\n");
	printf("3. Sacar\n");
	printf("4. Depositar\n");
	printf("5. Ver extrato\n");
	printf("6. Sair\n");
	printf("Escolha uma opcao: ");
	scanf("%d", &opcao);
		
	switch (opcao) {
		case 1:
			usuario_logado = autenticar_usuario();
			break;
			case 2:
			if(usuario_logado);
				mostrar_saldo();
				else
				printf("Voce precisa fazer login primeiro!\n");
				break;
				case 3:
					if (usuario_logado)
					realizar_deposito();
					else
					printf("Voce precisa fazer login primeiro!\n");
					case 4:
					if(usuario_logado)
						realizar_deposito();
						else
						printf("Voce precisa fazer login primeiro!\n");
						else
						printf("Voce precisa fazer login primeiro!\n");
						break;
						case 5:
							if(usuario_logado)
							mostrar_extrato();
							else
							printf("Voce precisa fazer login primeiro!\n")
							break;
							case 6:
								printf("Saindo...\n");
								break;
								default:
									printf("Opcao invalida!\n");
								}
									
							printf("\nPressione Enter para continuar...");
							getchar();
							getchat();
						}while (opcao !=6);
							
			return 0;
						}
						
				
					
		}
	}
