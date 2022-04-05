/*
 *  Define about Instruction Information
 */

#pragma once
#include <string.h>
#include "Register.h"

//instruction number, if set to ERRCODE, it means can't recognize instruction
typedef enum _INSTRUCTIONNUMBER {
    ERRCODE, Mov, Addn, Subn, Muln, Divn, Add, Sub, Mul, Div, Inc, Dec, 
    Cmp, Jmp, Jg, Je, Jl, Jge, Jle, Jne
}INUM;

//if set to TYPENULL, it means can't recognize operand's type
typedef enum _OPERNAD_TYPE {
    TYPENULL, REGISTER, NUMBER
} OPERNAD_TYPE;

//operand consist of type and value
typedef struct _OPERAND {
    OPERNAD_TYPE type;
    int value;
} OPERAND, *LPOPERAND;

//instruction
typedef struct _INST {
    INUM instruction_number;
    OPERAND operand[2];
} INST, *LPSFINST;

extern REGARR registers;
extern INST inst;

//get instruction number
void GetINUM(char *str) {
    if(!strcmp(str, "MOV")) inst.instruction_number = Mov;
    else if(!strcmp(str, "ADDN")) inst.instruction_number = Addn;
    else if(!strcmp(str, "SUBN")) inst.instruction_number = Subn;
    else if(!strcmp(str, "MULN")) inst.instruction_number = Muln;
    else if(!strcmp(str, "DIVN")) inst.instruction_number = Divn;
    else if(!strcmp(str, "ADD")) inst.instruction_number = Add;
    else if(!strcmp(str, "SUB")) inst.instruction_number = Sub;
    else if(!strcmp(str, "MUL")) inst.instruction_number = Mul;
    else if(!strcmp(str, "DIV")) inst.instruction_number = Div;
    else if(!strcmp(str, "INC")) inst.instruction_number = Inc;
    else if(!strcmp(str, "DEC")) inst.instruction_number = Dec;
    else if(!strcmp(str, "CMP")) inst.instruction_number = Cmp;
    else if(!strcmp(str, "JMP")) inst.instruction_number = Jmp;
    else if(!strcmp(str, "JG")) inst.instruction_number = Jg;
    else if(!strcmp(str, "JE")) inst.instruction_number = Je;
    else if(!strcmp(str, "JL")) inst.instruction_number = Jl;
    else if(!strcmp(str, "JGE")) inst.instruction_number = Jge;
    else if(!strcmp(str, "JLE")) inst.instruction_number = Jle;
    else if(!strcmp(str, "JNE")) inst.instruction_number = Jne;

    else inst.instruction_number = ERRCODE;
}

//get operand's info
void GetOperandInfo(char *str, int opNum) {
    //check operand is TYPENULL
    opNum--;
    int index = 0;
    if(str != NULL) {
        //case register
        if(str[0] == 'r' || str[0] == 'R') {
            if(str[2] != 0) inst.operand[opNum].type = TYPENULL;
            else inst.operand[opNum].type = REGISTER;
        }

        //case hex number
        else if(str[0] == '0' && (str[1] == 'x' || str[2] == 'X')) {
            index = 2;
            inst.operand[opNum].type = NUMBER;
            while(str[index] != 0) {
                if('0' > str[index] || str[index] > '9')
                    if('a' > str[index] || str[index] > 'f')
                        if('A' > str[index] || str[index] > 'F') {
                            inst.operand[opNum].type = TYPENULL;
                            break;
                        }
                index++;
            }

            //if input more than 4byte, don't calculate
            if(index > 10) inst.operand[opNum].type = TYPENULL;
        }

        //case decimal number
        else {
            inst.operand[opNum].type = NUMBER;
            if(str[index] == '-' || str[index] == '+') index++;
            while(str[index] != 0) {
                if('0' > str[index] || str[index] > '9') {
                    inst.operand[opNum].type = TYPENULL;
                    break;
                }
                index++;
            }

            //if input more than 4byte, don't calculate
            int temp = strtoll(str, NULL, 10) >> 32;
            if(temp != 0) inst.operand[opNum].type = TYPENULL;
        }

        //get operand's value
        if(inst.operand[opNum].type != TYPENULL) {
            switch(str[0]) {
                case 'r':
                case 'R':
                    inst.operand[opNum].type = REGISTER;
                    inst.operand[opNum].value = strtoll(&str[1], NULL, 16);
                    break;

                case '0':
                    inst.operand[opNum].type = NUMBER;
                    if(str[1] == 'x' || str[1] == 'X')
                        inst.operand[opNum].value = strtoll(str, NULL, 16);
                    else
                        inst.operand[opNum].value = strtoll(str, NULL, 10);
                    break;

                default :
                    inst.operand[opNum].type = NUMBER;
                    inst.operand[opNum].value = strtoll(str, NULL, 10);
                    break;
            }
        }
    }
}

void InitializeIns() {
    inst.instruction_number = ERRCODE;
    for(int x = 0 ; x < 2 ; x++) {
        inst.operand[x].type = TYPENULL;
        inst.operand[x].value = 0;
    }
}

int GetOperandValue(int num) {
    int op_value = 0;
    switch(inst.operand[num].type) {
        case REGISTER:
            op_value = registers.R[inst.operand[num].value];
            break;

        case NUMBER:
            op_value = inst.operand[num].value;
            break;
    }

    return op_value;
}

void SetFlag(C_R_TYPE result) {
    //set zero flag
    if((int)result == 0) registers.Flag.ZF = 1;
    else registers.Flag.ZF = 0;

    //set sign flag
    unsigned long long temp = 0x80000000 & (unsigned)result;
    registers.Flag.SF = temp >> 31;

    //set overflow flag
    int op1_value = GetOperandValue(0);
    int op2_value = GetOperandValue(1);

    //check op1's sign bit is same to op2's sign bit
    int temp2 = !(((unsigned)op1_value >> 31) ^ ((unsigned)op2_value >> 31));

    //if sign bits are different not set OF
    if(temp2 == 0) registers.Flag.OF = 0;

    else {
        //op1 is negative int
        if((unsigned)op1_value >> 31) {
            if(registers.Flag.SF == 0) registers.Flag.OF = 1;
            else registers.Flag.OF = 0;
        }

        //op1 is not negative int
        else {
            if(registers.Flag.SF == 1) registers.Flag.OF = 1;
            else registers.Flag.OF = 0;
        }
    }

    //set carry flag
    temp = result >> 32;
    if(temp != 0) registers.Flag.CF = 1;
    else registers.Flag.CF = 0;
}