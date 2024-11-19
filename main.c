#include <stdio.h>
#include <string.h>

#define REGISTRADORES 32 // MIPS por padrao tem 32 registradores

// Comandos
// ADD, SUB, AND, 
// ADDI, ORI, ANDI
// J, JAL, JR

struct PC {
    int vetor_registradores[REGISTRADORES];
    int pc_valor;
};

struct Instrucao {
    // campos para armazenar informações da instrução
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
    pc->pc_valor = 0;
}

int regIndice(char *registrador) {
    //retorna o índice do registrador mapeado
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
    // campos para armazenar informações da instrução
    char rt[10], rs[10], rd[10];
    int imediato;

    //inicia campos com 0
    instrucao->rs = instrucao->rt = instrucao->rd = instrucao->imediato = 0;
    sscanf(buffer, "%s", instrucao->instrucao);

    //ADDI, ORI, ANDI
    if (sscanf(buffer, "%s %[^,], %[^,], %d", instrucao->instrucao, rt, rs, &imediato) == 4) {
        instrucao->rt = regIndice(rt);
        instrucao->rs = regIndice(rs);
        instrucao->imediato = imediato;
        return;
    }
    //ADD, SUB, AND
    else if (sscanf(buffer, "%s %[^,], %[^,], %[^,]", instrucao->instrucao, rd, rs, rt) >= 4) {
        instrucao->rd = regIndice(rd);
        instrucao->rs = regIndice(rs);
        instrucao->rt = regIndice(rt);
        return;
    }
    //J, JAL
    else if (sscanf(buffer, "%s %d", instrucao->instrucao, &imediato) == 2) {
        instrucao->imediato = imediato;
        return;
    }
    //JR
    else if (sscanf(buffer, "%s %s", instrucao->instrucao, rs) == 2) {
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

// INSTRUCOES TIPO R
void ADD(struct Instrucao *instrucao, struct PC *pc) {
    pc->vetor_registradores[instrucao->rd] = pc->vetor_registradores[instrucao->rs] + pc->vetor_registradores[instrucao->rt];
}

void SUB(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rd] = pc->vetor_registradores[instrucao->rs] - pc->vetor_registradores[instrucao->rt];
}

void AND(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rd] = pc->vetor_registradores[instrucao->rs] & pc->vetor_registradores[instrucao->rt];
}

void JR(struct Instrucao *instrucao, struct PC *pc){
    pc->pc_valor = pc->vetor_registradores[instrucao->rs];
}

// INSTRUCOES TIPO I
void ADDI(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rt] = pc->vetor_registradores[instrucao->rs] + instrucao->imediato;
}

void ORI(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rt] = pc->vetor_registradores[instrucao->rs] | instrucao->imediato;
}

void ANDI(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[instrucao->rt] = pc->vetor_registradores[instrucao->rs] & instrucao->imediato;
}

// INSTRUCOES TIPO J
void J(struct Instrucao *instrucao, struct PC *pc){
    pc->pc_valor = instrucao->imediato;
}

void JAL(struct Instrucao *instrucao, struct PC *pc){
    pc->vetor_registradores[31] = pc->pc_valor; // $ra
    pc->pc_valor = instrucao->imediato;
}

void executarInstrucao(struct Instrucao *instrucao, struct PC *pc){
    //TIPOS DE INSTRUÇÕES
    //Tipo R
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
    if(strcmp(instrucao->instrucao, "JR") == 0){
        JR(instrucao, pc);
        return;
    }
    //--------------------------------------

    //Tipo I
    if(strcmp(instrucao->instrucao, "ADDI") == 0){
        ADDI(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }

    if(strcmp(instrucao->instrucao, "ORI") == 0){
        ORI(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }

    if(strcmp(instrucao->instrucao, "ANDI") == 0){
        ANDI(instrucao, pc);
        pc->pc_valor += 4;
        return;
    }
    //--------------------------------------

    //Tipo J
    if(strcmp(instrucao->instrucao, "J") == 0){
        J(instrucao, pc);
        return;
    }

    if(strcmp(instrucao->instrucao, "JAL") == 0){
        JAL(instrucao, pc);
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

    if(strcmp(instrucao->instrucao, "ADDI") == 0 || strcmp(instrucao->instrucao, "ORI") == 0 || strcmp(instrucao->instrucao, "ANDI") == 0) {

        printf("Tipo I: Opcode: ");
        
        if(strcmp(instrucao->instrucao, "ADDI") == 0)
            printf("8 ");
        else if(strcmp(instrucao->instrucao, "ORI") == 0)
            printf("13 ");
        else if(strcmp(instrucao->instrucao, "ANDI") == 0)
            printf("12 ");
        
        printf("rs: %d, rt: %d, imediato: %d\n", instrucao->rs, instrucao->rt, instrucao->imediato);
        return;
    }

    if(strcmp(instrucao->instrucao, "J") == 0 || strcmp(instrucao->instrucao, "JAL") == 0) {
        
        printf("Tipo J: Opcode: ");
        
        if(strcmp(instrucao->instrucao, "J") == 0)
            printf("2 ");
        else if(strcmp(instrucao->instrucao, "JAL") == 0)
            printf("3 ");
        
        printf("imediato: %d\n", instrucao->imediato);
        return;
    }

    if(strcmp(instrucao->instrucao, "JR") == 0){
        printf("Tipo R: Opcode: 0, rs: %d, rt: 0, rd: 0, shamt: 0, funct: 8\n", instrucao->rs); // Funct de JR é 8
        return;
    }

    printf("Instrução não reconhecida\n");
}

int main(){
    
    struct PC pc;
    struct Instrucao instrucao;
    char buffer[50];
    criarRegistradores(&pc);

    printf("Instruções suportadas: ADD, SUB, AND, ADDI, ORI, ANDI, J, JAL, JR\n\n");

    while(1){
        printf("Digite um comando: ");
        
        if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            printf("\nFim de entrada.\n");
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        //ignora linha vazia
        if(strlen(buffer) == 0){
            continue;
        }

        lerInstrucao(buffer, &instrucao);

        mostrarEstruturaInstrucao(&instrucao);

        executarInstrucao(&instrucao, &pc);
        
        mostrarRegistradores(&pc);
    }

    return 0;
}
