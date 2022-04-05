/*
 *  Module for MOV Instruction
 *  mov Operand's value to Operand1 register
 */

#include <Windows.h>
#include "Register.h"
#include "InstructionInformation.h"

extern REGARR registers;
extern INST inst;

void MOV() {
    int op2_value = 0;
    switch(inst.operand[1].type) {
        case REGISTER:
            op2_value = registers.R[inst.operand[1].value];
            break;

        case NUMBER:
            op2_value = inst.operand[1].value;
            break;
    }

    registers.R[inst.operand[0].value] = op2_value;
    printf("R%d : %d", inst.operand[0].value, op2_value);
}

BOOL CheckCanMov() {
    if(inst.operand[1].type == TYPENULL) return FALSE;
    if(inst.operand[0].type == REGISTER) return TRUE;
    return FALSE;
}

BOOL DoMov() {
    if(CheckCanMov()) {
        MOV();
        return TRUE;
    }

    return FALSE;
}