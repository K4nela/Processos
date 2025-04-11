#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_CLASSES 10
#define MAX_ASSUNTOS 10
#define MAX_PROCESSOS 20000

typedef struct {
    int id;
    char numero[30];
    char data_ajuizamento[25];
    int id_classe[MAX_CLASSES];
    int qtd_classes;
    int id_assunto[MAX_ASSUNTOS];
    int qtd_assuntos;
    int ano_eleicao;
} Processo;
Processo* ler_processos_csv(const char* nome_arquivo, int* quantidade);

//  Menu
void menu();

void ordenar_id(Processo processos[], int n); //Funcoes de ordenacao do codigo
void ordenar_data(Processo processos[], int n);//Funcoes de ordenacao do codigo


int contar_classe(Processo processos[], int n, int classe_busca);// Funcoes para contagem
int contar_assuntos_distintos(Processo processos[], int n);// Funcoes para contagem


void listar_processos(Processo processos[], int n);//Funcionalidades de listar e exportar
void listar_varios_assuntos(Processo processos[], int n);//Funcionalidades de listar e exportar
void salvar(const char *nome_arquivo, Processo processos[], int n);//Funcionalidades de listar e exportar

int dias_em_tramite(Processo processos[], int total_processos, int id_busca);//calculo do tempo de tramitacao

#endif
