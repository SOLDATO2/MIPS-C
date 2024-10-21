#include <stdio.h>
#include <string.h>

#define REGISTRADORES 32 //mips por padrao tem 32 registradores

//comandos
//R ( ADD (nao feito), SUB (nao feito), AND (nao feito) )



struct PC {
    int vetor_registradores[REGISTRADORES];
};

struct Instrucao {
    //todos os campos para as instrucoes que estamos usando
    char instrucao[11];
    int rs;
    int rt;
    int rd;
    int imediato;
};

void criarRegistradores(struct PC *pc){
    //inicia todos os registradores com 0, vamos armazenar os valores dos registradores aqui
    for(int i = 0; i < REGISTRADORES; i++){
        pc->vetor_registradores[i] = 1;
    }
}

void lerInstrucao(char *buffer, struct Instrucao *instrucao){
    //https://melted-ray-4c3.notion.site/Aula-04-Aprofundamento-em-Assembly-MIPS-2c1510d6c9754cbea48e2b236ab4aeaf
    char rt[10], rs[10], rd[10];

    
    //scaneia o buffer no formato "%s %[^,], %[^,], %[^,]" e armazena os valores ----instrucao->instrucao, rd, rs, rt----, retorna 4 se for um tipo R
    if (sscanf(buffer, "%s %[^,], %[^,], %s", instrucao->instrucao, rd, rs, rt) == 4){

        //trabalhando apenas com 3 instrucoes, alimenta o indicie dos registradores para rd, rs e rt
        instrucao->rd = (strcmp(rd, "$t0") == 0) ? 8 : 
                        (strcmp(rd, "$t1") == 0) ? 9 : 
                        (strcmp(rd, "$t2") == 0) ? 10 : -1;

        instrucao->rs = (strcmp(rs, "$t0") == 0) ? 8 : 
                        (strcmp(rs, "$t1") == 0) ? 9 : 
                        (strcmp(rs, "$t2") == 0) ? 10 : -1;

        instrucao->rt = (strcmp(rt, "$t0") == 0) ? 8 : 
                        (strcmp(rt, "$t1") == 0) ? 9 : 
                        (strcmp(rt, "$t2") == 0) ? 10 : -1;
        return;

    }
    printf("Instrucao nao reconhecida\n");

}

void mostrarRegistradores(struct PC *pc){
    for(int i = 0; i < REGISTRADORES; i++){
        printf("R%d: %d\n", i, pc->vetor_registradores[i]);
    }
}

//INSTRUCOES TIPO R
void ADD(struct Instrucao *instrucao, struct PC *pc) {
    pc->vetor_registradores[instrucao->rd] = pc->vetor_registradores[instrucao->rs] + pc->vetor_registradores[instrucao->rt];
}

//--------------------------------------

void executarInstrucao(struct Instrucao *instrucao, struct PC *pc){
    if (strcmp(instrucao->instrucao, "ADD") == 0){
        ADD(instrucao, pc);
    }
}


int main(){
    
    struct PC pc;
    char buffer[50];
    criarRegistradores(&pc);

    printf("Digite um comando: ");
    
    fgets(buffer, sizeof(buffer), stdin); //le e armazena comando mips no buffer

    struct Instrucao instrucao;
    lerInstrucao(buffer, &instrucao);

    executarInstrucao(&instrucao, &pc);
    

    
    
    mostrarRegistradores(&pc);
    return 0;
}