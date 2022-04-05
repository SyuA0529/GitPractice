#include <stdio.h>
#include <string.h>

#include "MyString.h"
#include "Register.h"
#include "InstructionInformation.h"
#include "Calculate.h"
#include "Mov.h"
#include "Compare.h"
#include "Jump.h"

//global value
REGARR registers;
INST inst;

void DoInstruction(FILE *fp, int *line) {
    BOOL success;
    switch(inst.instruction_number) {
        case Mov:
            success = DoMov();
            break;
        
        case Addn:
        case Subn:
        case Muln:
        case Divn:
        case Add:
        case Sub:
        case Mul:
        case Div:
        case Inc:
        case Dec:
            success = DoCalculate();
            break;

        case Cmp:
            success = DoCmp();
            break;

        case Jmp:
        case Jg:
        case Je:
        case Jl:
        case Jge:
        case Jle:
        case Jne:
            success = DoJmp(fp, line);
            break;

        default :
            printf("Wrong Instruction Name");
    }

    //error
    if(!success)
        printf("Wrong Instruction Usage");
}

int main() {
    //initializing
    FILE *fp = NULL;
    memset(&registers, 0x00, sizeof(REGARR));
    memset(&inst, 0x00, sizeof(inst));

    //get .txt path
    printf("Input Path : ");
    char *PATH = GetString();
    char line[32] = {0, };

    //file open
    if((fp = fopen(PATH, "r")) == NULL)
        perror("Can't Open File");

    int inst_line = 0; //instruction line
    printf("\n");

    //finish when read whole file
    while(TRUE) {
        //initializing ins, inc instructin line
        inst_line++;
        InitializeIns();

        //read line
        if(fgets(line, 32, fp) == NULL) break;
        DeleteCharFromStr(line, '\n');
        char org_line[32] = {0, };
        strcpy(org_line, line);

        //seperate line by token
        char *opcode = strtok(line, " ");
        char *op1 = strtok(NULL, " ");
        char *op2 = strtok(NULL, " ");
        DeleteCharFromStr(op1, ',');

        //set instruction info
        GetINUM(opcode);
        GetOperandInfo(op1, 1);
        GetOperandInfo(op2, 2);

        //do instruction
        printf("%d line : %s >> ", inst_line, org_line);
        DoInstruction(fp, &inst_line);

        if(!feof(fp)) {
            printf("\ncur flag state = ZF : %d, SF : %d, OF : %d, CF : %d\n\n", 
            registers.Flag.ZF, registers.Flag.SF, registers.Flag.OF, registers.Flag.CF);
        }

        else break;
    }

    printf("\n\nProgram End\n");
    return 0;
}