/*
 *  Module for JMP Instruction
 *  JMP to Operand1's value line
 *  check condition by flags
 */

#include <Windows.h>
#include "Register.h"
#include "InstructionInformation.h"

extern REGARR registers;
extern INST inst;

typedef int (*JUMP)(FILE *);

int JMP(FILE *fp) {
    int line = 0;
    char buffer[32] = {0, };

    if(inst.operand[0].type == REGISTER)
        line = registers.R[inst.operand[0].value];
    else
        line = inst.operand[0].value;
    line--;
    fseek(fp, 0, SEEK_SET);
    for(int x = 0 ; x < line ; x++)
        if(fgets(buffer, 32, fp) == NULL)
            break;

    if(feof(fp)) printf("Wrong Acces Memory, Access Denied");
    else printf("JMP to %d", line + 1);

    return line;
}

//ZF == 0 && (SF == OF)
int JG(FILE *fp) {
    int line = 0;
    if(registers.Flag.ZF == 0 && (registers.Flag.SF == registers.Flag.OF))
        line = JMP(fp);

    return line;
}

//ZF == 1
int JE(FILE *fp) {
    int line = 0;
    if(registers.Flag.ZF == 1)
        line = JMP(fp);

    return line;
}

//SF ^ OF
int JL(FILE *fp) {
    int line = 0;
    if(registers.Flag.SF ^ registers.Flag.OF)
        line = JMP(fp);

    return line;
}

//SF == OF
int JGE(FILE *fp) {
    int line = 0;
    if(registers.Flag.SF == registers.Flag.OF)
        line = JMP(fp);

    return line;
}

//ZF == 1 || (SF ^ OF)
int JLE(FILE *fp) {
    int line = 0;
    if(registers.Flag.ZF == 1 || (registers.Flag.SF ^ registers.Flag.OF))
        line = JMP(fp);

    return line;
}

//ZF == 0
int JNE(FILE *fp) {
    int line = 0;
    if(registers.Flag.ZF == 0) 
        line = JMP(fp);

    return line;
}

BOOL CheckCanJmp() {
    if(inst.operand[1].type == TYPENULL)
        if((inst.operand[0].type != TYPENULL) && (inst.operand[0].value >= 0))
            return TRUE;
    
    return FALSE;
}

BOOL DoJmp(FILE *fp, int *line) {
    JUMP LPJUMP = NULL;
    if(CheckCanJmp()) {
        switch(inst.instruction_number) {
            case Jmp:
                LPJUMP = JMP;
                break;  
            case Jg:
                LPJUMP = JG;
                break;
            case Je:
                LPJUMP = JE;
                break;
            case Jl:
                LPJUMP = JL;
                break;
            case Jge:
                LPJUMP = JGE;
                break;
            case Jle:
                LPJUMP = JLE;
                break;
            case Jne:
                LPJUMP = JNE;
                break;
        }

        if(LPJUMP != NULL)  {
            int temp_line = LPJUMP(fp);
            if(inst.operand[0].type == NUMBER)
                if(inst.operand[0].value == (temp_line + 1))
                    *line = temp_line;

                else
                    printf("Can't JMP");

            else
                if(registers.R[inst.operand[0].value] == (temp_line + 1))
                    *line = temp_line;

                else
                    printf("Can't JMP");

            return TRUE; //success
        }
    }

    return FALSE; //fail
}