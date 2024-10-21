#include <stdio.h>
#include <string.h>

#define REGISTRADORES 32 //mips por padrao tem 32 registradores

//comandos
//R ( ADD (OK), SUB (OK), AND (OK) )
//I (ADDI (nao OK), SUBI (nao ok), LI (nao ok) )



struct PC {
    int vetor_registradores[REGISTRADORES];
    int pc_valor;
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
    pc->pc_valor = 0; //inicia o pc em 0

}

void lerInstrucao(char *buffer, struct Instrucao *instrucao){
    //https://melted-ray-4c3.notion.site/Aula-04-Aprofundamento-em-Assembly-MIPS-2c1510d6c9754cbea48e2b236ab4aeaf
    char rt[10], rs[10], rd[10];

    
    //scaneia o buffer no formato "%s %[^,], %[^,], %s" e armazena os valores ----instrucao->instrucao, rd, rs, rt----, retorna 4 se for um tipo R
    if (sscanf(buffer, "%s %[^,], %[^,], %s", instrucao->instrucao, rd, rs, rt) == 4){

        //mapeando todas as instrucoes
        instrucao->rd = (strcmp(rd, "$t0") == 0) ? 8 : 
                        (strcmp(rd, "$t1") == 0) ? 9 : 
                        (strcmp(rd, "$t2") == 0) ? 10 :
                        (strcmp(rd, "$t3") == 0) ? 11 : 
                        (strcmp(rd, "$t4") == 0) ? 12 : 
                        (strcmp(rd, "$t5") == 0) ? 13 :
                        (strcmp(rd, "$t6") == 0) ? 14 : 
                        (strcmp(rd, "$t7") == 0) ? 15 : 
                        (strcmp(rd, "$t8") == 0) ? 16 :
                        (strcmp(rd, "$t9") == 0) ? 17 :
                        (strcmp(rd, "$s0") == 0) ? 18 : 
                        (strcmp(rd, "$s1") == 0) ? 19 : 
                        (strcmp(rd, "$s2") == 0) ? 20 :
                        (strcmp(rd, "$s3") == 0) ? 21 : 
                        (strcmp(rd, "$s4") == 0) ? 22 : 
                        (strcmp(rd, "$s5") == 0) ? 23 :
                        (strcmp(rd, "$s6") == 0) ? 24 : 
                        (strcmp(rd, "$s7") == 0) ? 25 : -1;

        instrucao->rs = (strcmp(rd, "$t0") == 0) ? 8 : 
                        (strcmp(rd, "$t1") == 0) ? 9 : 
                        (strcmp(rd, "$t2") == 0) ? 10 :
                        (strcmp(rd, "$t3") == 0) ? 11 : 
                        (strcmp(rd, "$t4") == 0) ? 12 : 
                        (strcmp(rd, "$t5") == 0) ? 13 :
                        (strcmp(rd, "$t6") == 0) ? 14 : 
                        (strcmp(rd, "$t7") == 0) ? 15 : 
                        (strcmp(rd, "$t8") == 0) ? 16 :
                        (strcmp(rd, "$t9") == 0) ? 17 :
                        (strcmp(rd, "$s0") == 0) ? 18 : 
                        (strcmp(rd, "$s1") == 0) ? 19 : 
                        (strcmp(rd, "$s2") == 0) ? 20 :
                        (strcmp(rd, "$s3") == 0) ? 21 : 
                        (strcmp(rd, "$s4") == 0) ? 22 : 
                        (strcmp(rd, "$s5") == 0) ? 23 :
                        (strcmp(rd, "$s6") == 0) ? 24 : 
                        (strcmp(rd, "$s7") == 0) ? 25 : -1;

        instrucao->rt = (strcmp(rd, "$t0") == 0) ? 8 : 
                        (strcmp(rd, "$t1") == 0) ? 9 : 
                        (strcmp(rd, "$t2") == 0) ? 10 :
                        (strcmp(rd, "$t3") == 0) ? 11 : 
                        (strcmp(rd, "$t4") == 0) ? 12 : 
                        (strcmp(rd, "$t5") == 0) ? 13 :
                        (strcmp(rd, "$t6") == 0) ? 14 : 
                        (strcmp(rd, "$t7") == 0) ? 15 : 
                        (strcmp(rd, "$t8") == 0) ? 16 :
                        (strcmp(rd, "$t9") == 0) ? 17 :
                        (strcmp(rd, "$s0") == 0) ? 18 : 
                        (strcmp(rd, "$s1") == 0) ? 19 : 
                        (strcmp(rd, "$s2") == 0) ? 20 :
                        (strcmp(rd, "$s3") == 0) ? 21 : 
                        (strcmp(rd, "$s4") == 0) ? 22 : 
                        (strcmp(rd, "$s5") == 0) ? 23 :
                        (strcmp(rd, "$s6") == 0) ? 24 : 
                        (strcmp(rd, "$s7") == 0) ? 25 : -1;
        return;

    }


    printf("Instrucao nao reconhecida\n");

}

void mostrarRegistradores(struct PC *pc){
    for(int i = 0; i < REGISTRADORES; i++){
        printf("R%d: %d\n", i, pc->vetor_registradores[i]);
    }
    printf("PC: %d\n", pc->pc_valor);
}

//INSTRUCOES TIPO R
void ADD(struct Instrucao *instrucao, struct PC *pc) {
    pc->vetor_registradores[instrucao->rd] = pc->vetor_registradores[instrucao->rs] + pc->vetor_registradores[instrucao->rt];
}

void SUB(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rd] = pc->vetor_registradores[instrucao->rs] - pc->vetor_registradores[instrucao->rt];
}

void AND(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rd] = pc->vetor_registradores[instrucao->rs] & pc->vetor_registradores[instrucao->rt];
}

//--------------------------------------

void executarInstrucao(struct Instrucao *instrucao, struct PC *pc){
    if(strcmp(instrucao->instrucao, "ADD") == 0){
        ADD(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }
    if(strcmp(instrucao->instrucao, "SUB") == 0){
        SUB(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }
    if(strcmp(instrucao->instrucao, "AND") == 0){
        SUB(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }

    printf("Nenhuma instrucao executada pois nao foi possivel identificar o comando\n");
}


int main(){
    
    struct PC pc;
    char buffer[50];
    criarRegistradores(&pc);

    while(1){
        printf("Digite um comando: ");
        
        fgets(buffer, sizeof(buffer), stdin); //le e armazena comando mips no buffer

        struct Instrucao instrucao;
        lerInstrucao(buffer, &instrucao);

        executarInstrucao(&instrucao, &pc);
        

        
        
        mostrarRegistradores(&pc);
    }

    return 0;
}