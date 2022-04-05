/*
 *  Module for CMP Instruction
 *  for using SetFlag function, set flags
 */

#include <stdio.h>
#include <Windows.h>
#include "Register.h"
#include "InstructionInformation.h"

extern REGARR registers;
extern INST inst;

void CMP() {
    int op_value[2];
    for(int x = 0 ; x < 2 ; x++) {
        if(inst.operand[x].type == REGISTER) op_value[x] = registers.R[inst.operand[x].value];
        else op_value[x] = inst.operand[x].value;
    }

    long long temp = (unsigned)op_value[0];
    C_R_TYPE result = temp - (unsigned)op_value[1];
    SetFlag(result);

    printf("ZF : %d, SF : %d", registers.Flag.ZF, registers.Flag.SF);
}

BOOL CheckCanCmp() {
    if((inst.operand[0].type != TYPENULL) && (inst.operand[1].type != TYPENULL))
        return TRUE;
    return FALSE;
}

BOOL DoCmp() {
    if(CheckCanCmp()) {
        CMP();
        return TRUE;
    }

    return FALSE;
}