#include <stdio.h>
#include <string.h>

#define REGISTRADORES 32 //mips por padrao tem 32 registradores

//comandos
//R ( ADD (OK), SUB (OK), AND (OK) )
//I (ADDI (OK), SUBI (ok), LI (ok) )
//J (J (n ok), JR (n ok), JAL(n ok) )


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
        pc->vetor_registradores[i] = 0;
    }
    pc->pc_valor = 0; //inicia o pc em 0
}
int regIndice(char *registrador) {
    //retorna o indice do registrador mapeado

    return (strcmp(registrador, "$zero") == 0) ? 0 :
           (strcmp(registrador, "$at") == 0) ? 1 :
           (strcmp(registrador, "$v0") == 0) ? 2 :
           (strcmp(registrador, "$v1") == 0) ? 3 :
           (strcmp(registrador, "$a0") == 0) ? 4 :
           (strcmp(registrador, "$a1") == 0) ? 5 :
           (strcmp(registrador, "$a2") == 0) ? 6 :
           (strcmp(registrador, "$a3") == 0) ? 7 :
           (strcmp(registrador, "$t0") == 0) ? 8 :
           (strcmp(registrador, "$t1") == 0) ? 9 :
           (strcmp(registrador, "$t2") == 0) ? 10 :
           (strcmp(registrador, "$t3") == 0) ? 11 :
           (strcmp(registrador, "$t4") == 0) ? 12 :
           (strcmp(registrador, "$t5") == 0) ? 13 :
           (strcmp(registrador, "$t6") == 0) ? 14 :
           (strcmp(registrador, "$t7") == 0) ? 15 :
           (strcmp(registrador, "$t8") == 0) ? 16 :
           (strcmp(registrador, "$t9") == 0) ? 17 :
           (strcmp(registrador, "$s0") == 0) ? 18 :
           (strcmp(registrador, "$s1") == 0) ? 19 :
           (strcmp(registrador, "$s2") == 0) ? 20 :
           (strcmp(registrador, "$s3") == 0) ? 21 :
           (strcmp(registrador, "$s4") == 0) ? 22 :
           (strcmp(registrador, "$s5") == 0) ? 23 :
           (strcmp(registrador, "$s6") == 0) ? 24 :
           (strcmp(registrador, "$s7") == 0) ? 25 :
           (strcmp(registrador, "$k0") == 0) ? 26 :
           (strcmp(registrador, "$k1") == 0) ? 27 :
           (strcmp(registrador, "$gp") == 0) ? 28 :
           (strcmp(registrador, "$sp") == 0) ? 29 :
           (strcmp(registrador, "$fp") == 0) ? 30 :
           (strcmp(registrador, "$ra") == 0) ? 31 : -1;
}

void lerInstrucao(char *buffer, struct Instrucao *instrucao){
    char rt[10], rs[10], rd[10];
    int imediato;

    //ADDI e SUBI
    sscanf(buffer, "%s", instrucao->instrucao);

    if (strncmp(instrucao->instrucao, "ADDI", 4) == 0 || strncmp(instrucao->instrucao, "SUBI", 4) == 0) {
        if (sscanf(buffer, "%s %[^,], %[^,], %d", instrucao->instrucao, rd, rs, &imediato) == 4) {
            instrucao->rd = regIndice(rd);
            instrucao->rs = regIndice(rs);
            instrucao->imediato = imediato;
            return;
        }
    }

    //LI
    if (sscanf(buffer, "%s %[^,], %d", instrucao->instrucao, rs, &imediato) == 3) {
        instrucao->rs = regIndice(rs);
        instrucao->imediato = imediato;
        return;
    }

    //todos tipo R
    if (sscanf(buffer, "%s %[^,], %[^,], %s", instrucao->instrucao, rd, rs, rt) == 4) {
        instrucao->rd = regIndice(rd);
        instrucao->rs = regIndice(rs);
        instrucao->rt = regIndice(rt);
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
//INSTRUCOES TIPO I

void ADDI(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rs] = pc->vetor_registradores[instrucao->rd] + instrucao->imediato;
}

void LI(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rs] = instrucao->imediato;
}

void SUBI(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rs] = pc->vetor_registradores[instrucao->rd] - instrucao->imediato;
}

//--------------------------------------

void executarInstrucao(struct Instrucao *instrucao, struct PC *pc){
    //TIPO R
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
        AND(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }
    //--------------------------------------

    //TIPO I
    if(strcmp(instrucao->instrucao, "LI") == 0){
        LI(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }

    if(strcmp(instrucao->instrucao, "ADDI") == 0 || strcmp(instrucao->instrucao, "SUBI") == 0){
        ADDI(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }
    //--------------------------------------

    printf("Nenhuma instrucao executada pois nao foi possivel identificar o comando\n");
}

void mostrarEstruturaInstrucao(struct Instrucao *instrucao){
    // Verifica se é uma instrução do tipo R
    if(strcmp(instrucao->instrucao, "ADD") == 0 || strcmp(instrucao->instrucao, "SUB") == 0 || strcmp(instrucao->instrucao, "AND") == 0) {

        printf("Tipo R: Opcode: 0, rs: %d, rt: %d, rd: %d, shamt: 0, funct: ", instrucao->rs, instrucao->rt, instrucao->rd);
        
        if(strcmp(instrucao->instrucao, "ADD") == 0)
            printf("32");
        else if(strcmp(instrucao->instrucao, "SUB") == 0)
            printf("34");
        else if(strcmp(instrucao->instrucao, "AND") == 0)
            printf("36");
        return;
    }

    // Verifica se é uma instrução do tipo I
    if(strcmp(instrucao->instrucao, "ADDI") == 0 || strcmp(instrucao->instrucao, "SUBI") == 0 || strcmp(instrucao->instrucao, "LI") == 0) {
        printf("Tipo I: Opcode: ");
        if(strcmp(instrucao->instrucao, "ADDI") == 0)
            printf("8");
        else if(strcmp(instrucao->instrucao, "SUBI") == 0)
            printf("10");
        else if(strcmp(instrucao->instrucao, "LI") == 0)
            printf("11");

        printf(", rs: %d, rd: %d, imediato: %d", instrucao->rs, instrucao->rd, instrucao->imediato);
        return;
    }

    printf("Instrucao nao reconhecida.\n");
}

int main(){
    char instrucao[60];
    struct PC pc;
    struct Instrucao inst;

    criarRegistradores(&pc);

    while(1){
        printf("\nDigite a instrucao: ");
        fgets(instrucao, sizeof(instrucao), stdin);
        instrucao[strcspn(instrucao, "\n")] = 0; // Remove o \n do final da string

        lerInstrucao(instrucao, &inst);
        executarInstrucao(&inst, &pc);
        mostrarRegistradores(&pc);
    }

    return 0;
}