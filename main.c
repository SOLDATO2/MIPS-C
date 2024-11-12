#include <stdio.h>
#include <string.h>

#define REGISTRADORES 32 //mips por padrao tem 32 registradores

//comandos
//R ( ADD (OK), SUB (OK), AND (OK) )
//I (ADDI (OK), SUBI (ok), LI (ok) )
//J (J (OK), JR (OK), JAL (OK) )


struct PC {
    int vetor_registradores[REGISTRADORES];
    int pc_valor;
};

struct Instrucao {
    //criando campos para armazenar informacoes da instrucao
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

int regIndice(char *registrador) {
    //https://melted-ray-4c3.notion.site/Aula-04-Aprofundamento-em-Assembly-MIPS-2c1510d6c9754cbea48e2b236ab4aeaf
    //retorna o indice do registrador mapeado
    const char *registradores[] = {
        "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
        "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
        "$s6", "$s7", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
    };

    for (int i = 0; i < REGISTRADORES; i++) {
        if (strcmp(registrador, registradores[i]) == 0) return i;
    }
    return -1;
}

void lerInstrucao(char *buffer, struct Instrucao *instrucao) {
    //https://melted-ray-4c3.notion.site/Aula-04-Aprofundamento-em-Assembly-MIPS-2c1510d6c9754cbea48e2b236ab4aeaf
    char rt[10], rs[10], rd[10];
    int imediato;
    
    sscanf(buffer, "%s", instrucao->instrucao);

    //ADDI e SUBI
    if (sscanf(buffer, "%s %[^,], %[^,], %d", instrucao->instrucao, rd, rs, &imediato) == 4) {
        instrucao->rd = regIndice(rd);
        instrucao->rs = regIndice(rs);
        instrucao->imediato = imediato;
        return;
    //LI
    } else if (sscanf(buffer, "%s %[^,], %d", instrucao->instrucao, rs, &imediato) == 3) {
        instrucao->rs = regIndice(rs);
        instrucao->imediato = imediato;
        return;
    //todos tipo R
    } else if (sscanf(buffer, "%s %[^,], %[^,], %s", instrucao->instrucao, rd, rs, rt) == 4) {
        instrucao->rd = regIndice(rd);
        instrucao->rs = regIndice(rs);
        instrucao->rt = regIndice(rt);
        return;
    //J e JAL
    } else if (sscanf(buffer, "%s %d", instrucao->instrucao, &imediato) == 2) {
        instrucao->imediato = imediato;
        return;
    //JR
    } else if (sscanf(buffer, "%s %s", instrucao->instrucao, rs) == 2) {
        instrucao->rs = regIndice(rs);
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
//INSTRUCOES TIPO J

void J(struct Instrucao *instrucao, struct PC *pc){
    pc->pc_valor = instrucao->imediato;
}

void JAL(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[31] = pc->pc_valor; //acessando o index direto aqui pq $ra é um registrador "escondido"
    pc->pc_valor = instrucao->imediato;
}

void JR(struct Instrucao *instrucao, struct PC *pc){
    pc->pc_valor = pc->vetor_registradores[instrucao->rs];
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

    //TIPO J
    if(strcmp(instrucao->instrucao, "J") == 0){
        J(instrucao, pc);
        return;
    }

    if(strcmp(instrucao->instrucao, "JAL") == 0){
        JAL(instrucao, pc);
        return;
    }

    if(strcmp(instrucao->instrucao, "JR") == 0){
        JR(instrucao, pc);
        return;
    }
    //--------------------------------------

    printf("Nenhuma instrucao executada pois nao foi possivel identificar o comando\n");
}

void mostrarEstruturaInstrucao(struct Instrucao *instrucao){
    if(strcmp(instrucao->instrucao, "ADD") == 0 || strcmp(instrucao->instrucao, "SUB") == 0 || strcmp(instrucao->instrucao, "AND") == 0) {

        printf("Tipo R: Opcode: 0, rs: %d, rt: %d, rd: %d, shamt: 0, funct: ", instrucao->rs, instrucao->rt, instrucao->rd);
        
        if(strcmp(instrucao->instrucao, "ADD") == 0)
            printf("32\n");
        else if(strcmp(instrucao->instrucao, "SUB") == 0)
            printf("34\n");
        else if(strcmp(instrucao->instrucao, "AND") == 0)
            printf("36\n");
        return;
    }

    if(strcmp(instrucao->instrucao, "ADDI") == 0 || strcmp(instrucao->instrucao, "SUBI") == 0 || strcmp(instrucao->instrucao, "LI") == 0) {

        printf("Tipo I: Opcode: ");
        
        if(strcmp(instrucao->instrucao, "ADDI") == 0)
            printf("8 ");
        else if(strcmp(instrucao->instrucao, "SUBI") == 0)
            printf("8 ");
        else if(strcmp(instrucao->instrucao, "LI") == 0)
            printf("8 ");
        
        printf("rs: %d, rt: %d, imediato: %d\n", instrucao->rs, instrucao->rd, instrucao->imediato);
        return;
    }

    if(strcmp(instrucao->instrucao, "J") == 0 || strcmp(instrucao->instrucao, "JAL") == 0 || strcmp(instrucao->instrucao, "JR") == 0) {
        
        printf("Tipo J: Opcode: ");
        
        if(strcmp(instrucao->instrucao, "J") == 0)
            printf("2 ");
        else if(strcmp(instrucao->instrucao, "JAL") == 0)
            printf("3 ");
        else if(strcmp(instrucao->instrucao, "JR") == 0)
            printf("0 ");
        
        if(strcmp(instrucao->instrucao, "JR") == 0)
            printf("rs: %d\n", instrucao->rs);
        else
            printf("imediato: %d\n", instrucao->imediato);
        return;
    }

    printf("Instrução não reconhecida\n");
}



int main(){
    
    struct PC pc;
    struct Instrucao instrucao;
    char buffer[50];
    criarRegistradores(&pc);

    while(1){
        printf("Digite um comando: ");
        
        fgets(buffer, sizeof(buffer), stdin); //le e armazena comando mips no buffer

        lerInstrucao(buffer, &instrucao);

        mostrarEstruturaInstrucao(&instrucao);

        executarInstrucao(&instrucao, &pc);
        
        
        
        mostrarRegistradores(&pc);
    }

    return 0;
}