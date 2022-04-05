/*
 *  Module for Calculate Instruction
 *  Calculate using by Operand1, Operand2's value
 *  if function with N, save result to R0 register
 *  the other functions, save result to Operand1's register
 *  set Flag in consideration reusult
 */

#pragma once
#include <string.h>
#include <Windows.h>
#include "Register.h"
#include "InstructionInformation.h"

typedef C_R_TYPE (*CALCULATE)();
extern REGARR registers;
extern INST inst;

//add set ZF and OF

//operand[0] is number
C_R_TYPE ADDN () {
    C_R_TYPE op2_value = (unsigned)GetOperandValue(1);
    C_R_TYPE temp = (unsigned)inst.operand[0].value;

    C_R_TYPE result = temp + op2_value;
    registers.R[0] = (REG)result;

    printf("R0 : %d", registers.R[0]);
    return result;
}

C_R_TYPE SUBN() {
    C_R_TYPE op2_value = (unsigned)GetOperandValue(1);
    C_R_TYPE temp = (unsigned)inst.operand[0].value;

    C_R_TYPE result = temp - op2_value;
    registers.R[0] = (REG)result;
    
    printf("R0 : %d", registers.R[0]);
    return result;
}

C_R_TYPE MULN() {
    C_R_TYPE op2_value = (unsigned)GetOperandValue(1);
    C_R_TYPE temp = (unsigned)inst.operand[0].value;

    C_R_TYPE result = temp * op2_value;
    registers.R[0] = (REG)result;
    
    printf("R0 : %d", registers.R[0]);
    return result;
}

C_R_TYPE DIVN() {
    C_R_TYPE op2_value = (unsigned)GetOperandValue(1);
    C_R_TYPE temp = (unsigned)inst.operand[0].value;

    C_R_TYPE result = temp / op2_value;
    registers.R[0] = (REG)result;
    
    printf("R0 : %d", registers.R[0]);
    return result;
}

//operand[0] is register
C_R_TYPE ADD() {
    C_R_TYPE op2_value = GetOperandValue(1);
    C_R_TYPE temp = (unsigned)registers.R[inst.operand[0].value];

    C_R_TYPE result = temp + op2_value;
    registers.R[inst.operand[0].value] = (REG)result;

    printf("R%d : %d", inst.operand[0].value, registers.R[inst.operand[0].value]);
    return result;
}

C_R_TYPE SUB() {
    C_R_TYPE op2_value = GetOperandValue(1);
    C_R_TYPE temp = (unsigned)registers.R[inst.operand[0].value];

    C_R_TYPE result = temp - op2_value;
    registers.R[inst.operand[0].value] = (REG)result;
    
    printf("R%d : %d", inst.operand[0].value, registers.R[inst.operand[0].value]);
    return result;
}

C_R_TYPE MUL() {
    C_R_TYPE op2_value = GetOperandValue(1);
    C_R_TYPE temp = (unsigned)registers.R[inst.operand[0].value];

    C_R_TYPE result = temp * op2_value;
    registers.R[inst.operand[0].value] = (REG)result;
    
    printf("R%d : %d", inst.operand[0].value, registers.R[inst.operand[0].value]);
    return result;
}

C_R_TYPE DIV() {
    C_R_TYPE op2_value = GetOperandValue(1);
    C_R_TYPE temp = (unsigned)registers.R[inst.operand[0].value];

    C_R_TYPE result = temp / op2_value;
    registers.R[inst.operand[0].value] = (REG)result;
    
    printf("R%d : %d", inst.operand[0].value, registers.R[inst.operand[0].value]);
    return result;
}

C_R_TYPE INC() {
    C_R_TYPE temp = (unsigned)registers.R[inst.operand[0].value];
    C_R_TYPE result = temp + (unsigned)1;
    registers.R[inst.operand[0].value] = result;
    printf("R%d : %d", inst.operand[0].value, registers.R[inst.operand[0].value]);
    return result;
}

C_R_TYPE DEC() {
    C_R_TYPE temp = (unsigned)registers.R[inst.operand[0].value];
    C_R_TYPE result = temp - (unsigned)1;
    registers.R[inst.operand[0].value] = result;
    printf("R%d : %d", inst.operand[0].value, registers.R[inst.operand[0].value]);
    return result;
}

//check can caculate instruction
BOOL CheckCanCalculate() {
    switch(inst.instruction_number) {
        case Addn:
        case Subn:
        case Muln:
        case Divn:
            if((inst.operand[0].type == NUMBER) && (inst.operand[1].type != TYPENULL)) {\
                if(inst.instruction_number == Divn) {
                    if(inst.operand[1].type == REGISTER) {
                        if(registers.R[inst.operand[1].value] != 0) return TRUE;
                        else return FALSE;
                    }

                    else if(inst.operand[1].type == NUMBER) {
                        if(inst.operand[1].value != 0) return TRUE;
                        else return FALSE;
                    }

                    else return FALSE;
                }

                return TRUE;
            }
            else return FALSE;

        case Add:
        case Sub:
        case Mul:
        case Div:
            if((inst.operand[0].type == REGISTER) && (inst.operand[1].type != TYPENULL)) {
                if(inst.instruction_number == Div) {
                    if(inst.operand[1].type == REGISTER) {
                        if(registers.R[inst.operand[1].value] != 0) return TRUE;
                        else return FALSE;
                    }

                    else if(inst.operand[1].type == NUMBER) {
                        if(inst.operand[1].value != 0) return TRUE;
                        else return FALSE;
                    }

                    else return FALSE;
                }

                return TRUE;
            }
            else return FALSE;

        case Inc:
        case Dec:
            if((inst.operand[0].type == REGISTER) && (inst.operand[1].type == TYPENULL)) return TRUE;
        
        default:
            return FALSE;
    }
}

//do calculate instruction
BOOL DoCalculate() {
    CALCULATE LPCALCULATE = NULL;
    if(CheckCanCalculate()) {
        switch(inst.instruction_number) {
            case Addn:
                LPCALCULATE = ADDN;
                break;  
            case Subn:
                LPCALCULATE = SUBN;
                break;
            case Muln:
                LPCALCULATE = MULN;
                break;
            case Divn:
                LPCALCULATE = DIVN;
                break;
            case Add:
                LPCALCULATE = ADD;
                break;
            case Sub:
                LPCALCULATE = SUB;
                break;
            case Mul:
                LPCALCULATE = MUL;
                break;
            case Div:
                LPCALCULATE = DIV;
                break;
            case Inc:
                LPCALCULATE = INC;
                break;
            case Dec:
                LPCALCULATE = DEC;
                break;
        }

        if(LPCALCULATE != NULL)  {
            C_R_TYPE result = LPCALCULATE();
            SetFlag(result);
            return TRUE; //success
        }
    }

    return FALSE;
}