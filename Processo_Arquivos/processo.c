#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

//-------------------------------------MENU-------------------------------------
void menu(){
    printf("\n========================== OPCOES ===========================\n");
    printf("[1] Organizar ID e salvar em ordem crescente\n");
    printf("[2] Organizar decrescente e salvar\n");
    printf("[3] Contar total de classe por processo\n");
    printf("[4] Contar todos os assuntos distintos\n");
    printf("[5] Listar os processos com mais de um assunto\n");
    printf("[6] Ver os dias em que estao em tramite por ID do processo\n"); 
    printf("[7] Listar processos do arquivo\n");
    printf("[0] Sair");
    printf("\n=============================================================\n");
    printf("Selecione\n:");
}

//----------------------------LER LINHAS DO PROCESSO----------------------------
Processo* ler_processos_csv(const char* nome_arquivo, int* quantidade) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        *quantidade = 0;
        return NULL;
    }

    Processo* processos = malloc(MAX_PROCESSOS * sizeof(Processo));
    if (!processos) {
        fclose(arquivo);
        perror("!ERROR! Nao foi possivel alocar a memoria.");
        *quantidade = 0;
        return NULL;
    }

    char linha[1024];
    int i = 0;

    fgets(linha, sizeof(linha), arquivo); //Essa etapa desconsidera o cabecalho
    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_PROCESSOS) {
        Processo p = {0};
        char numero[64], data[64], classe_str[128], assunto_str[128];
        int id;

        int lidos = sscanf(linha, "%d,\"%[^\"]\",%[^,],\"%[^\"]\",%[^,],%d",
                           &p.id, numero, data, classe_str, assunto_str, &p.ano_eleicao);

        if (lidos < 6) {
            // tenta versao sem aspas (linhas com apenas um valor em classe/assunto)
            lidos = sscanf(linha, "%d,\"%[^\"]\",%[^,],%[^,],%[^,],%d",
                           &p.id, numero, data, classe_str, assunto_str, &p.ano_eleicao);
        }

        if (lidos == 6) {
            strcpy(p.numero, numero);
            strncpy(p.data_ajuizamento, data, 19);
            p.data_ajuizamento[19] = '\0';

            // classes
            p.qtd_classes = 0;
            char* token = strtok(classe_str, "{},");
            while (token && p.qtd_classes < 2) {
                p.id_classe[p.qtd_classes++] = atoi(token);
                token = strtok(NULL, "{},");
            }

            // assuntos
            p.qtd_assuntos = 0;
            token = strtok(assunto_str, "{},");
            while (token && p.qtd_assuntos < MAX_ASSUNTOS) {
                p.id_assunto[p.qtd_assuntos++] = atoi(token);
                token = strtok(NULL, "{},");
            }

            processos[i++] = p;
        }
    }

    fclose(arquivo);
    *quantidade = i;
    return processos;
}

//-----------------------------ORGANIZAR ID-------------------------------------
void ordenar_id(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processos[j].id > processos[j + 1].id) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

//----------------------------ORDANIZAR DATA------------------------------------
void ordenar_data(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(processos[j].data_ajuizamento, processos[j + 1].data_ajuizamento) < 0) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

//---------------------------SALVAR ARQUIVO-------------------------------------
void salvar(const char *nome_arquivo, Processo processos[], int n) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (fp == NULL) {
        printf("Nao ta abrindo  %s\n", nome_arquivo);
        return;
    }

    fprintf(fp, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,\"%s\",\"%s\",{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            fprintf(fp, "%d", processos[i].id_classe[j]);
            if (j < processos[i].qtd_classes - 1) fprintf(fp, ",");
        }

        fprintf(fp, "},{");
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            fprintf(fp, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].qtd_assuntos - 1) fprintf(fp, ",");
        }
        fprintf(fp, "},%d\n", processos[i].ano_eleicao);
    }

    fclose(fp);
    printf("Arquivo '%s' salvo com SUCESSO!.\n", nome_arquivo);
}

//---------------------------CONTAR CLASSE--------------------------------------
int contar_classe(Processo processos[], int total_processos, int classe_busca) {
    int count = 0;
    for (int i = 0; i < total_processos; i++) {
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            if (processos[i].id_classe[j] == classe_busca) {
                count++;
                break;
            }
        }
    }
    return count;
}

//-----------------------CONTAR ASSUNTOS DIFERENTES-----------------------------
int contar_assuntos_distintos(Processo processos[], int n) {
    int assuntos[1000];
    int total_distintos = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int atual = processos[i].id_assunto[j];
            int encontrado = 0;

            for (int k = 0; k < total_distintos; k++) {
                if (assuntos[k] == atual) {
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                assuntos[total_distintos++] = atual;
            }
        }
    }

    return total_distintos;
}

//--------------------LISTAR PROCESSOS COM MAIS DE UM ASSUNTO-------------------
void listar_varios_assuntos(Processo processos[], int n) {
    printf("\nProcessos com mais de um id_assunto:\n");
    printf("\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

    for (int i = 0; i < n; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("%d,\"%s\",\"%s\",{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
            
            for (int j = 0; j < processos[i].qtd_classes; j++) {
                printf("%d", processos[i].id_classe[j]);
                if (j < processos[i].qtd_classes - 1) printf(",");
            }

            printf("},{");
            for (int j = 0; j < processos[i].qtd_assuntos; j++) {
                printf("%d", processos[i].id_assunto[j]);
                if (j < processos[i].qtd_assuntos - 1) printf(",");
            }
            printf("},%d\n", processos[i].ano_eleicao);
        }
    }
}

//----------------------------DIAS EM TRAMITE-----------------------------------
int dias_em_tramite(Processo processos[], int total_processos, int id_busca) {
    for (int i = 0; i < total_processos; i++) {
        if (processos[i].id == id_busca) {
            struct tm data_ajuizamento = {0};

            int ano, mes, dia, hora, min, seg; // = "2016-04-20 15:03:40.000" to perdida, assim q tem q ficar (esperamos), ajuda!
            if (sscanf(processos[i].data_ajuizamento, "%d-%d-%d %d:%d:%d",
                       &ano, &mes, &dia, &hora, &min, &seg) == 6) {
                data_ajuizamento.tm_year = ano - 1900;
                data_ajuizamento.tm_mon = mes - 1;
                data_ajuizamento.tm_mday = dia;
                data_ajuizamento.tm_hour = hora;
                data_ajuizamento.tm_min = min;
                data_ajuizamento.tm_sec = seg;

                time_t t_ajuizamento = mktime(&data_ajuizamento);
                time_t t_atual = time(NULL);

                double segundos = difftime(t_atual, t_ajuizamento);
                int dias = (int)(segundos / (60 * 60 * 24));

                printf("Processo ID: %d\n", processos[i].id);
                printf("Data de ajuizamento: %s\n", processos[i].data_ajuizamento);
                printf("Dias em tramitacao: %d dias\n", dias);
                return dias;
            } else {
                printf("!ERROR! Nao foi possivel ler a data do processo.\n");
                return -1;
            }
        }
    }
    printf("!ERROR! Nao foi possivel encontrar o ID %d.\n", id_busca);
    return -1;
}

//não foi possivel mesmo mds...

//----------------------------LISTANDO PROCESSOS--------------------------------
void listar_processos(Processo processos[], int n) {
    for (int i = 0; i < n; i++) {
        printf("ID: %d\n", processos[i].id);
        printf("Numero: %s\n", processos[i].numero);
        printf("Data do processo ajuizamento: %s\n", processos[i].data_ajuizamento);

        printf("Classe(s): ");
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            printf("%d", processos[i].id_classe[j]);
            if (j < processos[i].qtd_classes - 1) printf(", ");
        }
        printf("\n");

        
        printf("Ano: %d\n", processos[i].ano_eleicao);

        
        printf("Assuntos: ");
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            printf("%d", processos[i].id_assunto[j]);
            if (j < processos[i].qtd_assuntos - 1) printf(", ");
        }
        printf("\n\n");
    }
}
//Finalizamos, ufa nao aguentava mais!! Merecemos 10 prof <3


//AEEEEEEEEEEEEEEEEEEEEEEEEE CARACA Q SOFRIMENTO, VOU COMER DEPOIS DE 12 HORAS CODANDO
//mereço um lanche

//depois de quase me m4t4r, CONSEGUIMOS!!!!!!