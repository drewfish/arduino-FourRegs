// vim: ts=4 sts=4 sw=4 et si ai
/*
FourRegs.h - Library for printing the low-level ARM SAMD51 configuration registers
Created by Drew Folta, April 20, 2020.

The MIT License (MIT)

Copyright (c) 2020 Drew Folta

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef FOUR_REGS_H
#define FOUR_REGS_H
#include <Arduino.h>


// This is a wild experiment in API design. The idea is to make it easy to add
// new configuration options in the future without a long list of arguments.
struct FourRegOptions {
    Print   &print;
    bool    showDisabled;
};


void printFourRegAC(FourRegOptions &opts);
void printFourRegADC(FourRegOptions &opts, Adc* adc, uint8_t idx);
void printFourRegAES(FourRegOptions &opts);
//FUTURE void printFourRegCAN(FourRegOptions &opts);
void printFourRegCCL(FourRegOptions &opts);
void printFourRegCMCC(FourRegOptions &opts);
void printFourRegDAC(FourRegOptions &opts);
void printFourRegDMAC(FourRegOptions &opts);
//FUTURE void printFourRegDSU(FourRegOptions &opts);
void printFourRegEIC(FourRegOptions &opts);
void printFourRegEVSYS(FourRegOptions &opts);
void printFourRegFREQM(FourRegOptions &opts);
void printFourRegGCLK(FourRegOptions &opts);
void printFourRegI2S(FourRegOptions &opts);
void printFourRegICM(FourRegOptions &opts);
void printFourRegMCLK(FourRegOptions &opts);
void printFourRegNVMCTRL(FourRegOptions &opts);
void printFourRegOSC32KCTRL(FourRegOptions &opts);
void printFourRegOSCCTRL(FourRegOptions &opts);
void printFourRegPAC(FourRegOptions &opts);
void printFourRegPCC(FourRegOptions &opts);
//FUTURE void printFourRegPDEC(FourRegOptions &opts);
void printFourRegPM(FourRegOptions &opts);
void printFourRegPORT(FourRegOptions &opts);
//FUTURE void printFourRegQSPI(FourRegOptions &opts);
void printFourRegRTC(FourRegOptions &opts);
void printFourRegSERCOM(FourRegOptions &opts, Sercom* sercom, uint8_t idx);
void printFourRegSUPC(FourRegOptions &opts);
void printFourRegTC(FourRegOptions &opts, Tc* tc, uint8_t idx);
void printFourRegTCC(FourRegOptions &opts, Tcc* tcc, uint8_t idx);
//FUTURE void printFourRegTRNG(FourRegOptions &opts);
//FUTURE void printFourRegUSB(FourRegOptions &opts);
void printFourRegWDT(FourRegOptions &opts);

void printFourRegs(FourRegOptions &opts);

#endif // FOUR_REGS_H
