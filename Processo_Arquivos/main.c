#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    int total = 0;
    Processo* processos = ler_processos_csv("tabela.csv", &total);

    if (!processos || total == 0) {
        printf("Nenhum processo lido.\n");
        return 1;
    }

    printf("Total de processos lidos: %d\n", total);

    int opcao;
    do {
        menu();
        scanf("%d", &opcao);


        switch (opcao) {
            case 1:
                ordenar_id(processos, total);
                salvar("Id_Ordenado.csv", processos, total);
                break;

            case 2:
                ordenar_data(processos, total);
                salvar("Data_Ordenado.csv", processos, total);
                break;

            case 3: {
                int classe_desejada;
                printf("Digite o ID da classe que deseja buscar: ");
                scanf("%d", &classe_desejada);
                int total_encontrado = contar_classe(processos, total, classe_desejada);
                printf("Total de processos com a classe %d: %d\n", classe_desejada, total_encontrado);
                break;
            }

            case 4: {
                int distintos = contar_assuntos_distintos(processos, total);
                printf("Assuntos distintos: %d\n", distintos);
                break;
            }

            case 5:
                listar_varios_assuntos(processos, total);
                break;

            case 6: {
                int id_procurado;
                printf("Digite o ID do processo para verificar dias de tramitacao: ");
                scanf("%d", &id_procurado);
                dias_em_tramite(processos, total, id_procurado);
                break;
            }

            case 7:
                listar_processos(processos, total);
                break;

            case 0:
                printf("Obrigado por usar nosso programa <3\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    //ta apontando muito erro mds...

    free(processos);
    return 0;
}
