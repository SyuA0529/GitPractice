/*
 *  Define about register
 */

#pragma once
#include <Windows.h>

typedef int REG;
typedef char FLAG;
typedef unsigned long long C_R_TYPE; //Calculate Return Type

//Flag Register array
typedef struct _FLAGARR {
    FLAG SF;
    FLAG ZF;
    FLAG OF;
    FLAG CF;
} FLAGS, *LPFLAGS;

//whole of Registers
typedef struct _REGARR {
    REG R[10];
    FLAGS Flag;
} REGARR, *LPREGARR;