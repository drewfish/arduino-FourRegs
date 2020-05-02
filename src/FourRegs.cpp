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


#include "FourRegs.h"
#include <samd.h>


static const char* FourRegs__DISABLED = "--disabled--";
static const char* FourRegs__RESERVED = "--reserved--";
static const char* FourRegs__UNKNOWN = "--unknown--";
static const char* FourRegs__empty = "";
#define PRINTFLAG(x,y) do { if (x.bit.y) { opts.print.print(" " #y); } } while(0)
#define PRINTHEX(x) do { opts.print.print("0x"); opts.print.print(x, HEX); } while(0)
#define PRINTSCALE(x) (opts.print.print(1 << (x)))
#define PRINTNL() opts.print.println(FourRegs__empty)
#define PRINTPAD2(x) do { if (x < 10) { opts.print.print("0"); } opts.print.print(x, DEC); } while(0)
#define COPYVOL(dst,src) do { memcpy((void*)(&(dst)), (void*)(&(src)), sizeof(dst)); } while(0)


// When using platformio.org, peripheral details can be found in
// ~/.platformio/packages/framework-arduinosam/system/samd/CMSIS-Atmel/CMSIS/Device/ATMEL/samd51/include/component/


void printFourRegAC(FourRegOptions &opts) {
    while (AC->SYNCBUSY.bit.ENABLE) {}
    if (!AC->CTRLA.bit.ENABLE && !opts.showDisabled) {
        return;
    }
    opts.print.println("--------------------------- AC");

    opts.print.print("CTRLA: ");
    PRINTFLAG(AC->CTRLA, ENABLE);
    PRINTNL();

    opts.print.print("EVCTRL: ");
    PRINTFLAG(AC->EVCTRL, COMPEO0);
    PRINTFLAG(AC->EVCTRL, COMPEO1);
    PRINTFLAG(AC->EVCTRL, WINEO0);
    PRINTFLAG(AC->EVCTRL, COMPEI0);
    PRINTFLAG(AC->EVCTRL, COMPEI1);
    PRINTFLAG(AC->EVCTRL, INVEI0);
    PRINTFLAG(AC->EVCTRL, INVEI1);
    PRINTNL();

    while (AC->SYNCBUSY.bit.WINCTRL) {}
    opts.print.print("WINCTRL: ");
    PRINTFLAG(AC->WINCTRL, WEN0);
    opts.print.print(" WINTSEL0=");
    PRINTHEX(AC->WINCTRL.bit.WINTSEL0);
    PRINTNL();

    opts.print.print("SCALER0:  ");
    opts.print.print(AC->SCALER[0].bit.VALUE);
    PRINTNL();
    opts.print.print("SCALER1:  ");
    opts.print.print(AC->SCALER[1].bit.VALUE);
    PRINTNL();

    for (uint8_t id = 0; id < 2; id++) {
        while (AC->SYNCBUSY.vec.COMPCTRL & (1<<id)) {}
        opts.print.print("COMPCTRL");
        opts.print.print(id);
        opts.print.print(": ");
        PRINTFLAG(AC->COMPCTRL[id], ENABLE);
        PRINTFLAG(AC->COMPCTRL[id], SINGLE);
        opts.print.print("intsel=");
        switch (AC->COMPCTRL[id].bit.INTSEL) {
            case 0x0: opts.print.print("TOGGLE"); break;
            case 0x1: opts.print.print("RISING"); break;
            case 0x2: opts.print.print("FALLING"); break;
            case 0x3: opts.print.print("EOC"); break;
        }
        PRINTFLAG(AC->COMPCTRL[id], RUNSTDBY);
        opts.print.print("muxneg=");
        switch (AC->COMPCTRL[id].bit.MUXNEG) {
            case 0x0: opts.print.print("PIN0"); break;
            case 0x1: opts.print.print("PIN1"); break;
            case 0x2: opts.print.print("PIN2"); break;
            case 0x3: opts.print.print("PIN3"); break;
            case 0x4: opts.print.print("GND"); break;
            case 0x5: opts.print.print("VSCALE"); break;
            case 0x6: opts.print.print("BANDGAP"); break;
            case 0x7: opts.print.print("DAC"); break;
        }
        opts.print.print("muxpos=");
        switch (AC->COMPCTRL[id].bit.MUXPOS) {
            case 0x0: opts.print.print("PIN0"); break;
            case 0x1: opts.print.print("PIN1"); break;
            case 0x2: opts.print.print("PIN2"); break;
            case 0x3: opts.print.print("PIN3"); break;
            case 0x4: opts.print.print("GND"); break;
            case 0x5: opts.print.print("VSCALE"); break;
            default: opts.print.print(FourRegs__RESERVED); break;
        }
        PRINTFLAG(AC->COMPCTRL[id], SWAP);
        opts.print.print(" SPEED=");
        opts.print.print(AC->COMPCTRL[id].bit.SPEED);
        if (AC->COMPCTRL[id].bit.HYSTEN) {
            opts.print.print(" HYSTEN HYST=");
            opts.print.print(AC->COMPCTRL[id].bit.HYST);
        }
        opts.print.print(" flen=");
        switch (AC->COMPCTRL[id].bit.FLEN) {
            case 0x0: opts.print.print("OFF"); break;
            case 0x1: opts.print.print("MAJ3"); break;
            case 0x2: opts.print.print("MAJ5"); break;
            default: opts.print.print(FourRegs__RESERVED); break;
        }
        opts.print.print(" out=");
        switch (AC->COMPCTRL[id].bit.OUT) {
            case 0x0: opts.print.print("OFF"); break;
            case 0x1: opts.print.print("ASYNC"); break;
            case 0x2: opts.print.print("SYNC"); break;
            default: opts.print.print(FourRegs__RESERVED); break;
        }
        PRINTNL();
    }

    opts.print.print("CALIB:  ");
    PRINTHEX(AC->CALIB.bit.BIAS0);
    PRINTNL();
}


void printFourRegADC(FourRegOptions &opts, Adc* adc, uint8_t idx) {
    while (adc->SYNCBUSY.bit.ENABLE) {}
    if (!adc->CTRLA.bit.ENABLE && !opts.showDisabled) {
        return;
    }
    opts.print.print("--------------------------- ADC");
    opts.print.print(idx);
    if (adc->CTRLA.bit.SLAVEEN) {
        opts.print.println(" --slave--");
        return;
    }
    PRINTNL();

    opts.print.print("CTRLA: ");
    PRINTFLAG(adc->CTRLA, ENABLE);
    if (ADC1->CTRLA.bit.SLAVEEN == 1) {
        opts.print.print(" dualsel=");
        switch (adc->CTRLA.bit.DUALSEL) {
            case 0x0: opts.print.print("BOTH"); break;
            case 0x1: opts.print.print("INTERLEAVE"); break;
            default: opts.print.print(FourRegs__RESERVED); break;
        }
    }
    PRINTFLAG(adc->CTRLA, RUNSTDBY);
    PRINTFLAG(adc->CTRLA, ONDEMAND);
    opts.print.print(" PRESCALER=");
    PRINTHEX(adc->CTRLA.bit.PRESCALER);
    PRINTFLAG(adc->CTRLA, R2R);
    PRINTNL();

    opts.print.print("EVCTRL: ");
    PRINTFLAG(adc->EVCTRL, FLUSHEI);
    PRINTFLAG(adc->EVCTRL, STARTEI);
    PRINTFLAG(adc->EVCTRL, FLUSHINV);
    PRINTFLAG(adc->EVCTRL, STARTINV);
    PRINTFLAG(adc->EVCTRL, RESRDYEO);
    PRINTFLAG(adc->EVCTRL, WINMONEO);
    PRINTNL();

    while (adc->SYNCBUSY.bit.INPUTCTRL) {}
    opts.print.print("INPUTCTRL: ");
    opts.print.print(" muxpos=");
    if (adc->INPUTCTRL.bit.MUXPOS <= 23) {
        opts.print.print("AIN");
        opts.print.print(adc->INPUTCTRL.bit.MUXPOS);
    } else {
        switch (adc->INPUTCTRL.bit.MUXPOS) {
            case 0x18: opts.print.print("SCALEDCOREVCC"); break;
            case 0x19: opts.print.print("SCALEDVBAT"); break;
            case 0x1A: opts.print.print("SCALEDIOVCC"); break;
            case 0x1B: opts.print.print("BANDGAP"); break;
            case 0x1C: opts.print.print("PTAT"); break;
            case 0x1D: opts.print.print("CTAT"); break;
            case 0x1E: opts.print.print("DAC"); break;
            case 0x1F: opts.print.print(FourRegs__UNKNOWN); break;
        }
    }
    PRINTFLAG(adc->INPUTCTRL, DIFFMODE);
    opts.print.print(" muxneg=");
    switch (adc->INPUTCTRL.bit.MUXNEG) {
        case 0x0: opts.print.print("AIN0"); break;
        case 0x1: opts.print.print("AIN1"); break;
        case 0x2: opts.print.print("AIN2"); break;
        case 0x3: opts.print.print("AIN3"); break;
        case 0x4: opts.print.print("AIN4"); break;
        case 0x5: opts.print.print("AIN5"); break;
        case 0x6: opts.print.print("AIN6"); break;
        case 0x7: opts.print.print("AIN7"); break;
        case 0x18: opts.print.print("GND"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    PRINTFLAG(adc->INPUTCTRL, DSEQSTOP);
    PRINTNL();

    while (adc->SYNCBUSY.bit.CTRLB) {}
    opts.print.print("CTRLB: ");
    PRINTFLAG(adc->CTRLB, LEFTADJ);
    PRINTFLAG(adc->CTRLB, FREERUN);
    PRINTFLAG(adc->CTRLB, CORREN);
    opts.print.print(" ressel=");
    switch (adc->CTRLB.bit.RESSEL) {
        case 0x0: opts.print.print("12BIT"); break;
        case 0x1: opts.print.print("16BIT"); break;
        case 0x2: opts.print.print("10BIT"); break;
        case 0x3: opts.print.print("8BIT"); break;
    }
    opts.print.print(" WINMODE=");
    PRINTHEX(adc->CTRLB.bit.WINMODE);
    PRINTFLAG(adc->CTRLB, WINSS);
    PRINTNL();

    while (adc->SYNCBUSY.bit.REFCTRL) {}
    opts.print.print("REFCTRL:  refsel=");
    switch (adc->REFCTRL.bit.REFSEL) {
        case 0x0: opts.print.print("INTREF"); break;
        case 0x1: opts.print.print("INTVCC0"); break;
        case 0x2: opts.print.print("INTVCC1"); break;
        case 0x3: opts.print.print("AREFA"); break;
        case 0x4: opts.print.print("AREFB"); break;
        case 0x5: opts.print.print("AREFC"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    PRINTFLAG(adc->REFCTRL, REFCOMP);
    PRINTNL();

    while (adc->SYNCBUSY.bit.AVGCTRL) {}
    opts.print.print("AVGCTRL:  SAMPLENUM=");
    PRINTHEX(adc->AVGCTRL.bit.SAMPLENUM);
    opts.print.print(" ADJRES=");
    PRINTHEX(adc->AVGCTRL.bit.ADJRES);
    PRINTNL();

    while (adc->SYNCBUSY.bit.SAMPCTRL) {}
    opts.print.print("SAMPCTRL:  SAMPLEN=");
    PRINTHEX(adc->SAMPCTRL.bit.SAMPLEN);
    PRINTFLAG(adc->SAMPCTRL, OFFCOMP);
    PRINTNL();

    while (adc->SYNCBUSY.bit.WINLT) {}
    opts.print.print("WINLT:  ");
    opts.print.print(adc->WINLT.bit.WINLT);
    PRINTNL();

    while (adc->SYNCBUSY.bit.WINUT) {}
    opts.print.print("WINUT:  ");
    opts.print.print(adc->WINUT.bit.WINUT);
    PRINTNL();

    while (adc->SYNCBUSY.bit.CTRLB) {}
    if (adc->CTRLB.bit.CORREN) {
        while (adc->SYNCBUSY.bit.GAINCORR) {}
        opts.print.print("GAINCORR:  ");
        opts.print.print(adc->GAINCORR.bit.GAINCORR);
        PRINTNL();

        while (adc->SYNCBUSY.bit.OFFSETCORR) {}
        opts.print.print("OFFSETCORR:  ");
        opts.print.print(adc->OFFSETCORR.bit.OFFSETCORR);
        PRINTNL();
    }

    opts.print.print("DSEQCTRL: ");
    PRINTFLAG(adc->DSEQCTRL, INPUTCTRL);
    PRINTFLAG(adc->DSEQCTRL, CTRLB);
    PRINTFLAG(adc->DSEQCTRL, REFCTRL);
    PRINTFLAG(adc->DSEQCTRL, AVGCTRL);
    PRINTFLAG(adc->DSEQCTRL, SAMPCTRL);
    PRINTFLAG(adc->DSEQCTRL, WINLT);
    PRINTFLAG(adc->DSEQCTRL, WINUT);
    PRINTFLAG(adc->DSEQCTRL, GAINCORR);
    PRINTFLAG(adc->DSEQCTRL, OFFSETCORR);
    PRINTFLAG(adc->DSEQCTRL, AUTOSTART);
    PRINTNL();

    opts.print.print("CALIB:  BIASCOMP=");
    PRINTHEX(adc->CALIB.bit.BIASCOMP);
    opts.print.print(" BIASR2R=");
    PRINTHEX(adc->CALIB.bit.BIASR2R);
    opts.print.print(" BIASREFBUF=");
    PRINTHEX(adc->CALIB.bit.BIASREFBUF);
    PRINTNL();
}


void printFourRegAES(FourRegOptions &opts) {
    if (!AES->CTRLA.bit.ENABLE && !opts.showDisabled) {
        return;
    }
    opts.print.println("--------------------------- AES");

    opts.print.print("CTRLA: ");
    PRINTFLAG(AES->CTRLA, ENABLE);
    opts.print.print(" aesmode=");
    switch (AES->CTRLA.bit.AESMODE) {
        case 0: opts.print.print("ECB"); break;
        case 1: opts.print.print("CBC"); break;
        case 2: opts.print.print("OFB"); break;
        case 3: opts.print.print("CFG"); break;
        case 4: opts.print.print("Counter"); break;
        case 5: opts.print.print("CCM"); break;
        case 6: opts.print.print("GCM"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    opts.print.print(" CFBS=");
    PRINTHEX(AES->CTRLA.bit.CFBS);
    opts.print.print(" keysize=");
    switch (AES->CTRLA.bit.KEYSIZE) {
        case 0: opts.print.print("128bit"); break;
        case 1: opts.print.print("192bit"); break;
        case 2: opts.print.print("256bit"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    opts.print.print(" cipher=");
    opts.print.print(AES->CTRLA.bit.CIPHER ? "Encryption" : "Decryption");
    opts.print.print(" startmode=");
    opts.print.print(AES->CTRLA.bit.STARTMODE ? "auto" : "manual");
    PRINTFLAG(AES->CTRLA, LOD);
    PRINTFLAG(AES->CTRLA, KEYGEN);
    PRINTFLAG(AES->CTRLA, XORKEY);
    opts.print.print(" CTYPE=");
    PRINTHEX(AES->CTRLA.bit.CTYPE);
    PRINTNL();

    opts.print.print("CTRLB: ");
    PRINTFLAG(AES->CTRLB, EOM);
    PRINTFLAG(AES->CTRLB, GFMUL);
    PRINTNL();
}


void printFourRegCCL(FourRegOptions &opts) {
    uint8_t i;
    if (!CCL->CTRL.bit.ENABLE && !opts.showDisabled) {
        return;
    }
    opts.print.println("--------------------------- CCL");

    opts.print.print("CTRL: ");
    PRINTFLAG(CCL->CTRL, ENABLE);
    PRINTFLAG(CCL->CTRL, RUNSTDBY);
    PRINTNL();

    for (i = 0; i < 2; i++) {
        opts.print.print("SEQCTRL");
        opts.print.print(i);
        opts.print.print(":  seqsel=");
        switch (CCL->SEQCTRL[i].bit.SEQSEL) {
            case 0x0: opts.print.print("DISABLE"); break;
            case 0x1: opts.print.print("DFF"); break;
            case 0x2: opts.print.print("JK"); break;
            case 0x3: opts.print.print("LATCH"); break;
            case 0x4: opts.print.print("RS"); break;
            default: opts.print.print(FourRegs__RESERVED); break;
        }
        PRINTNL();
    }

    for (i = 0; i < 4; i++) {
        if (!CCL->LUTCTRL[i].bit.ENABLE && !opts.showDisabled) {
            continue;
        }
        opts.print.print("LUTCTRL");
        opts.print.print(i);
        opts.print.print(": ");
        PRINTFLAG(CCL->LUTCTRL[i], ENABLE);
        opts.print.print(" filtersel=");
        switch (CCL->LUTCTRL[i].bit.FILTSEL) {
            case 0x0: opts.print.print("DISABLE"); break;
            case 0x1: opts.print.print("SYNCH"); break;
            case 0x2: opts.print.print("FILTER"); break;
            default: opts.print.print(FourRegs__RESERVED); break;
        }
        PRINTFLAG(CCL->LUTCTRL[i], EDGESEL);
        for (uint8_t j=0; j < 3; j++) {
            uint8_t insel;
            if (j == 0) { insel = CCL->LUTCTRL[i].bit.INSEL0; }
            if (j == 1) { insel = CCL->LUTCTRL[i].bit.INSEL1; }
            if (j == 2) { insel = CCL->LUTCTRL[i].bit.INSEL2; }
            opts.print.print(" insel");
            opts.print.print(j);
            opts.print.print("=");
            switch (insel) {
                case 0x0: opts.print.print("MASK"); break;
                case 0x1: opts.print.print("FEEDBACK"); break;
                case 0x2: opts.print.print("LINK"); break;
                case 0x3: opts.print.print("EVENT"); break;
                case 0x4: opts.print.print("IO"); break;
                case 0x5: opts.print.print("AC"); break;
                case 0x6: opts.print.print("TC"); break;
                case 0x7: opts.print.print("ALTTC"); break;
                case 0x8: opts.print.print("TCC"); break;
                case 0x9: opts.print.print("SERCOM"); break;
                default: opts.print.print(FourRegs__UNKNOWN); break;
            }
        }
        PRINTFLAG(CCL->LUTCTRL[i], INVEI);
        PRINTFLAG(CCL->LUTCTRL[i], LUTEI);
        PRINTFLAG(CCL->LUTCTRL[i], LUTEO);
        opts.print.print(" TRUTH=");
        PRINTHEX(CCL->LUTCTRL[i].bit.TRUTH);
        PRINTNL();
    }
}


void printFourRegCMCC(FourRegOptions &opts) {
    if (!CMCC->SR.bit.CSTS && !opts.showDisabled) {
        return;
    }
    opts.print.println("--------------------------- CMCC");

    opts.print.print("TYPE: ");
    // datasheet rev E has fields not in Atmel CMSIS header
    if (CMCC->TYPE.reg & (1<<0)) { opts.print.print(" AP"); }
    if (CMCC->TYPE.reg & (1<<1)) { opts.print.print(" GCLK"); }
    if (CMCC->TYPE.reg & (1<<2)) { opts.print.print(" RANDP"); }
    if (CMCC->TYPE.reg & (1<<3)) { opts.print.print(" LRUP"); }
    PRINTFLAG(CMCC->TYPE, RRP);
    opts.print.print(" waynum=");
    switch (CMCC->TYPE.bit.WAYNUM) {
        case 0x0: opts.print.print("DMAPPED"); break;
        case 0x1: opts.print.print("ARCH2WAY"); break;
        case 0x2: opts.print.print("ARCH4WAY"); break;
        case 0x3: opts.print.print("ARCH8WAY"); break;
    }
    PRINTFLAG(CMCC->TYPE, LCKDOWN);
    opts.print.print(" csize=");
    switch (CMCC->TYPE.bit.CSIZE) {
        case 0x0: opts.print.print("1KB"); break;
        case 0x1: opts.print.print("2KB"); break;
        case 0x2: opts.print.print("4KB"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    opts.print.print(" clsize=");
    switch (CMCC->TYPE.bit.CLSIZE) {
        case 0x0: opts.print.print("16B"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    PRINTNL();

    opts.print.print("CFG: ");
    PRINTFLAG(CMCC->CFG, ICDIS);
    PRINTFLAG(CMCC->CFG, DCDIS);
    opts.print.print(" csizesw=");
    switch (CMCC->CFG.bit.CSIZESW) {
        case 0x0: opts.print.print("1KB"); break;
        case 0x1: opts.print.print("2KB"); break;
        case 0x2: opts.print.print("4KB"); break;
        case 0x3: opts.print.print("8KB"); break;
        case 0x4: opts.print.print("16KB"); break;
        case 0x5: opts.print.print("32KB"); break;
        case 0x6: opts.print.print("64KB"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    PRINTNL();

    opts.print.print("SR: ");
    PRINTFLAG(CMCC->SR, CSTS);
    PRINTNL();

    opts.print.print("LCKWAY:  ");
    PRINTHEX(CMCC->LCKWAY.bit.LCKWAY);
    PRINTNL();


    opts.print.print("MEN:  ");
    PRINTFLAG(CMCC->MEN, MENABLE);
    PRINTNL();

    if (CMCC->MEN.bit.MENABLE) {
        opts.print.print("MCFG:  mode=");
        switch (CMCC->MCFG.bit.MODE) {
            case 0x0: opts.print.print("CYCLE_COUNT"); break;
            case 0x1: opts.print.print("IHIT_COUNT"); break;
            case 0x2: opts.print.print("DHIT_COUNT"); break;
            default: opts.print.print(FourRegs__RESERVED); break;
        }
        PRINTNL();
    }
}


void printFourRegEIC_SENSE(FourRegOptions &opts, uint8_t sense) {
    switch (sense) {
        case 0x0: opts.print.print("none"); break;
        case 0x1: opts.print.print("RISE"); break;
        case 0x2: opts.print.print("FALL"); break;
        case 0x3: opts.print.print("BOTH"); break;
        case 0x4: opts.print.print("HIGH"); break;
        case 0x5: opts.print.print("LOW"); break;
        default:  opts.print.print(FourRegs__UNKNOWN); break;
    }
}
void printFourRegEIC(FourRegOptions &opts) {
    //FUTURE -- NMI is enabled if NMICTRL.NMISENSE is set, regardless of CTRLA.ENABLE
    while (EIC->SYNCBUSY.bit.ENABLE) {}
    if (!EIC->CTRLA.bit.ENABLE && !opts.showDisabled) {
        return;
    }
    opts.print.println("--------------------------- EIC");

    opts.print.print("EIC: ");
    PRINTFLAG(EIC->CTRLA, ENABLE);
    opts.print.print(" cksel=");
    opts.print.print(EIC->CTRLA.bit.CKSEL ? "CLK_ULP32K" : "GCLK_EIC");
    PRINTNL();

    if (EIC->NMICTRL.bit.NMISENSE || opts.showDisabled) {
        opts.print.print("NMI:  ");
        printFourRegEIC_SENSE(opts, EIC->NMICTRL.bit.NMISENSE);
        if (EIC->NMICTRL.bit.NMIFILTEN) {
            opts.print.print(" FILTEN");
        }
        if (EIC->NMICTRL.bit.NMIASYNCH) {
            opts.print.print(" ASYNCH");
        }
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.println("NMI:  none");
        }
    }

    for (uint8_t id = 0; id < 16; id++) {
        uint8_t cfg = id / 8;
        uint8_t pos = (id % 8) * 0x4;
        uint32_t entry = 0xF & (EIC->CONFIG[cfg].reg >> pos);
        if (((0x7 & entry) == 0) && !opts.showDisabled) {
            continue;
        }
        opts.print.print("EXTINT");
        PRINTPAD2(id);
        opts.print.print(":  ");
        printFourRegEIC_SENSE(opts, 0x7 & entry);
        if (0x8 & entry) {
            opts.print.print(" FILTEN");
        }
        if (EIC->ASYNCH.bit.ASYNCH & (1 << id)) {
            opts.print.print(" ASYNCH");
        }
        if (EIC->DEBOUNCEN.bit.DEBOUNCEN & (1 << id)) {
            opts.print.print(" DEBOUNCEN");
        }
        if (EIC->EVCTRL.bit.EXTINTEO & (1 << id)) {
            opts.print.print(" EXTINTEO");
        }
        PRINTNL();
    }

    opts.print.print("DPRESCALER: ");
    PRINTFLAG(EIC->DPRESCALER, TICKON);
    opts.print.print(" PRESCALER0=");
    PRINTHEX(EIC->DPRESCALER.bit.PRESCALER0);
    opts.print.print(" STATES0=");
    PRINTHEX(EIC->DPRESCALER.bit.STATES0);
    opts.print.print(" PRESCALER1=");
    PRINTHEX(EIC->DPRESCALER.bit.PRESCALER1);
    opts.print.print(" STATES1=");
    PRINTHEX(EIC->DPRESCALER.bit.STATES1);
    PRINTNL();
}


static const char* FourRegsEVSYS_EVGEN00 = "NONE";
static const char* FourRegsEVSYS_EVGEN01 = "OSCCTRL_XOSC_FAIL0";
static const char* FourRegsEVSYS_EVGEN02 = "OSCCTRL_XOSC_FAIL1";
static const char* FourRegsEVSYS_EVGEN03 = "OSC32KCTRL_XOSC_FAIL";
static const char* FourRegsEVSYS_EVGEN04 = "RTC_PER0";
static const char* FourRegsEVSYS_EVGEN05 = "RTC_PER1";
static const char* FourRegsEVSYS_EVGEN06 = "RTC_PER2";
static const char* FourRegsEVSYS_EVGEN07 = "RTC_PER3";
static const char* FourRegsEVSYS_EVGEN08 = "RTC_PER4";
static const char* FourRegsEVSYS_EVGEN09 = "RTC_PER5";
static const char* FourRegsEVSYS_EVGEN0A = "RTC_PER6";
static const char* FourRegsEVSYS_EVGEN0B = "RTC_PER7";
static const char* FourRegsEVSYS_EVGEN0C = "RTC_CMP0";
static const char* FourRegsEVSYS_EVGEN0D = "RTC_CMP1";
static const char* FourRegsEVSYS_EVGEN0E = "RTC_CMP2";
static const char* FourRegsEVSYS_EVGEN0F = "RTC_CMP3";
static const char* FourRegsEVSYS_EVGEN10 = "RTC_TAMPER";
static const char* FourRegsEVSYS_EVGEN11 = "RTC_OVF";
static const char* FourRegsEVSYS_EVGEN12 = "EIC_EXTINT00";
static const char* FourRegsEVSYS_EVGEN13 = "EIC_EXTINT01";
static const char* FourRegsEVSYS_EVGEN14 = "EIC_EXTINT02";
static const char* FourRegsEVSYS_EVGEN15 = "EIC_EXTINT03";
static const char* FourRegsEVSYS_EVGEN16 = "EIC_EXTINT04";
static const char* FourRegsEVSYS_EVGEN17 = "EIC_EXTINT05";
static const char* FourRegsEVSYS_EVGEN18 = "EIC_EXTINT06";
static const char* FourRegsEVSYS_EVGEN19 = "EIC_EXTINT07";
static const char* FourRegsEVSYS_EVGEN1A = "EIC_EXTINT08";
static const char* FourRegsEVSYS_EVGEN1B = "EIC_EXTINT09";
static const char* FourRegsEVSYS_EVGEN1C = "EIC_EXTINT10";
static const char* FourRegsEVSYS_EVGEN1D = "EIC_EXTINT11";
static const char* FourRegsEVSYS_EVGEN1E = "EIC_EXTINT12";
static const char* FourRegsEVSYS_EVGEN1F = "EIC_EXTINT13";
static const char* FourRegsEVSYS_EVGEN20 = "EIC_EXTINT14";
static const char* FourRegsEVSYS_EVGEN21 = "EIC_EXTINT15";
static const char* FourRegsEVSYS_EVGEN22 = "DMAC_CH0";
static const char* FourRegsEVSYS_EVGEN23 = "DMAC_CH1";
static const char* FourRegsEVSYS_EVGEN24 = "DMAC_CH2";
static const char* FourRegsEVSYS_EVGEN25 = "DMAC_CH3";
static const char* FourRegsEVSYS_EVGEN26 = "PAC_ACCERR";
static const char* FourRegsEVSYS_EVGEN27 = FourRegs__RESERVED;
static const char* FourRegsEVSYS_EVGEN28 = FourRegs__RESERVED;
static const char* FourRegsEVSYS_EVGEN29 = "TCC0_OVF";
static const char* FourRegsEVSYS_EVGEN2A = "TCC0_TRG";
static const char* FourRegsEVSYS_EVGEN2B = "TCC0_CNT";
static const char* FourRegsEVSYS_EVGEN2C = "TCC0_MC0";
static const char* FourRegsEVSYS_EVGEN2D = "TCC0_MC1";
static const char* FourRegsEVSYS_EVGEN2E = "TCC0_MC2";
static const char* FourRegsEVSYS_EVGEN2F = "TCC0_MC3";
static const char* FourRegsEVSYS_EVGEN30 = "TCC0_MC4";
static const char* FourRegsEVSYS_EVGEN31 = "TCC0_MC5";
static const char* FourRegsEVSYS_EVGEN32 = "TCC1_OVF";
static const char* FourRegsEVSYS_EVGEN33 = "TCC1_TRG";
static const char* FourRegsEVSYS_EVGEN34 = "TCC1_CNT";
static const char* FourRegsEVSYS_EVGEN35 = "TCC1_MC0";
static const char* FourRegsEVSYS_EVGEN36 = "TCC1_MC1";
static const char* FourRegsEVSYS_EVGEN37 = "TCC1_MC2";
static const char* FourRegsEVSYS_EVGEN38 = "TCC1_MC3";
static const char* FourRegsEVSYS_EVGEN39 = "TCC2_OVF";
static const char* FourRegsEVSYS_EVGEN3A = "TCC2_TRG";
static const char* FourRegsEVSYS_EVGEN3B = "TCC2_CNT";
static const char* FourRegsEVSYS_EVGEN3C = "TCC2_MC0";
static const char* FourRegsEVSYS_EVGEN3D = "TCC2_MC1";
static const char* FourRegsEVSYS_EVGEN3E = "TCC2_MC2";
static const char* FourRegsEVSYS_EVGEN3F = "TCC3_OVF";
static const char* FourRegsEVSYS_EVGEN40 = "TCC3_TRG";
static const char* FourRegsEVSYS_EVGEN41 = "TCC3_CNT";
static const char* FourRegsEVSYS_EVGEN42 = "TCC3_MC0";
static const char* FourRegsEVSYS_EVGEN43 = "TCC3_MC1";
static const char* FourRegsEVSYS_EVGEN44 = "TCC4_OVF";
static const char* FourRegsEVSYS_EVGEN45 = "TCC4_TRG";
static const char* FourRegsEVSYS_EVGEN46 = "TCC4_CNT";
static const char* FourRegsEVSYS_EVGEN47 = "TCC4_MC0";
static const char* FourRegsEVSYS_EVGEN48 = "TCC4_MC1";
static const char* FourRegsEVSYS_EVGEN49 = "TC0_OVF";
static const char* FourRegsEVSYS_EVGEN4A = "TC0_MC0";
static const char* FourRegsEVSYS_EVGEN4B = "TC0_MC1";
static const char* FourRegsEVSYS_EVGEN4C = "TC1_OVF";
static const char* FourRegsEVSYS_EVGEN4D = "TC1_MC0";
static const char* FourRegsEVSYS_EVGEN4E = "TC1_MC1";
static const char* FourRegsEVSYS_EVGEN4F = "TC2_OVF";
static const char* FourRegsEVSYS_EVGEN50 = "TC2_MC0";
static const char* FourRegsEVSYS_EVGEN51 = "TC2_MC1";
static const char* FourRegsEVSYS_EVGEN52 = "TC3_OVF";
static const char* FourRegsEVSYS_EVGEN53 = "TC3_MC0";
static const char* FourRegsEVSYS_EVGEN54 = "TC3_MC1";
static const char* FourRegsEVSYS_EVGEN55 = "TC4_OVF";
static const char* FourRegsEVSYS_EVGEN56 = "TC4_MC0";
static const char* FourRegsEVSYS_EVGEN57 = "TC4_MC1";
static const char* FourRegsEVSYS_EVGEN58 = "TC5_OVF";
static const char* FourRegsEVSYS_EVGEN59 = "TC5_MC0";
static const char* FourRegsEVSYS_EVGEN5A = "TC5_MC1";
static const char* FourRegsEVSYS_EVGEN5B = "TC6_OVF";
static const char* FourRegsEVSYS_EVGEN5C = "TC6_MC0";
static const char* FourRegsEVSYS_EVGEN5D = "TC6_MC1";
static const char* FourRegsEVSYS_EVGEN5E = "TC7_OVF";
static const char* FourRegsEVSYS_EVGEN5F = "TC7_MC0";
static const char* FourRegsEVSYS_EVGEN60 = "TC7_MC1";
static const char* FourRegsEVSYS_EVGEN61 = "PDEC_OVF";
static const char* FourRegsEVSYS_EVGEN62 = "PDEC_ERR";
static const char* FourRegsEVSYS_EVGEN63 = "PDEC_DIR";
static const char* FourRegsEVSYS_EVGEN64 = "PDEC_VLC";
static const char* FourRegsEVSYS_EVGEN65 = "PDEC_MC0";
static const char* FourRegsEVSYS_EVGEN66 = "PDEC_MC1";
static const char* FourRegsEVSYS_EVGEN67 = "ADC0_RESRDY";
static const char* FourRegsEVSYS_EVGEN68 = "ADC0_WINMON";
static const char* FourRegsEVSYS_EVGEN69 = "ADC1_RESRDY";
static const char* FourRegsEVSYS_EVGEN6A = "ADC1_WINMON";
static const char* FourRegsEVSYS_EVGEN6B = "AC_COMP0";
static const char* FourRegsEVSYS_EVGEN6C = "AC_COMP1";
static const char* FourRegsEVSYS_EVGEN6D = "AC_WIN";
static const char* FourRegsEVSYS_EVGEN6E = "DAC_EMPTY0";
static const char* FourRegsEVSYS_EVGEN6F = "DAC_EMPTY1";
static const char* FourRegsEVSYS_EVGEN70 = "DAC_RESRDY0";
static const char* FourRegsEVSYS_EVGEN71 = "DAC_RESRDY1";
static const char* FourRegsEVSYS_EVGEN72 = "GMAC_TSU_CMP";
static const char* FourRegsEVSYS_EVGEN73 = "TRNG_READY";
static const char* FourRegsEVSYS_EVGEN74 = "CCL_LUTOUT0";
static const char* FourRegsEVSYS_EVGEN75 = "CCL_LUTOUT1";
static const char* FourRegsEVSYS_EVGEN76 = "CCL_LUTOUT2";
static const char* FourRegsEVSYS_EVGEN77 = "CCL_LUTOUT3";
static const char* FourRegsEVSYS_EVGENs[] = {
    FourRegsEVSYS_EVGEN00, FourRegsEVSYS_EVGEN01, FourRegsEVSYS_EVGEN02, FourRegsEVSYS_EVGEN03, FourRegsEVSYS_EVGEN04, FourRegsEVSYS_EVGEN05, FourRegsEVSYS_EVGEN06, FourRegsEVSYS_EVGEN07,
    FourRegsEVSYS_EVGEN08, FourRegsEVSYS_EVGEN09, FourRegsEVSYS_EVGEN0A, FourRegsEVSYS_EVGEN0B, FourRegsEVSYS_EVGEN0C, FourRegsEVSYS_EVGEN0D, FourRegsEVSYS_EVGEN0E, FourRegsEVSYS_EVGEN0F,
    FourRegsEVSYS_EVGEN10, FourRegsEVSYS_EVGEN11, FourRegsEVSYS_EVGEN12, FourRegsEVSYS_EVGEN13, FourRegsEVSYS_EVGEN14, FourRegsEVSYS_EVGEN15, FourRegsEVSYS_EVGEN16, FourRegsEVSYS_EVGEN17,
    FourRegsEVSYS_EVGEN18, FourRegsEVSYS_EVGEN19, FourRegsEVSYS_EVGEN1A, FourRegsEVSYS_EVGEN1B, FourRegsEVSYS_EVGEN1C, FourRegsEVSYS_EVGEN1D, FourRegsEVSYS_EVGEN1E, FourRegsEVSYS_EVGEN1F,
    FourRegsEVSYS_EVGEN20, FourRegsEVSYS_EVGEN21, FourRegsEVSYS_EVGEN22, FourRegsEVSYS_EVGEN23, FourRegsEVSYS_EVGEN24, FourRegsEVSYS_EVGEN25, FourRegsEVSYS_EVGEN26, FourRegsEVSYS_EVGEN27,
    FourRegsEVSYS_EVGEN28, FourRegsEVSYS_EVGEN29, FourRegsEVSYS_EVGEN2A, FourRegsEVSYS_EVGEN2B, FourRegsEVSYS_EVGEN2C, FourRegsEVSYS_EVGEN2D, FourRegsEVSYS_EVGEN2E, FourRegsEVSYS_EVGEN2F,
    FourRegsEVSYS_EVGEN30, FourRegsEVSYS_EVGEN31, FourRegsEVSYS_EVGEN32, FourRegsEVSYS_EVGEN33, FourRegsEVSYS_EVGEN34, FourRegsEVSYS_EVGEN35, FourRegsEVSYS_EVGEN36, FourRegsEVSYS_EVGEN37,
    FourRegsEVSYS_EVGEN38, FourRegsEVSYS_EVGEN39, FourRegsEVSYS_EVGEN3A, FourRegsEVSYS_EVGEN3B, FourRegsEVSYS_EVGEN3C, FourRegsEVSYS_EVGEN3D, FourRegsEVSYS_EVGEN3E, FourRegsEVSYS_EVGEN3F,
    FourRegsEVSYS_EVGEN40, FourRegsEVSYS_EVGEN41, FourRegsEVSYS_EVGEN42, FourRegsEVSYS_EVGEN43, FourRegsEVSYS_EVGEN44, FourRegsEVSYS_EVGEN45, FourRegsEVSYS_EVGEN46, FourRegsEVSYS_EVGEN47,
    FourRegsEVSYS_EVGEN48, FourRegsEVSYS_EVGEN49, FourRegsEVSYS_EVGEN4A, FourRegsEVSYS_EVGEN4B, FourRegsEVSYS_EVGEN4C, FourRegsEVSYS_EVGEN4D, FourRegsEVSYS_EVGEN4E, FourRegsEVSYS_EVGEN4F,
    FourRegsEVSYS_EVGEN50, FourRegsEVSYS_EVGEN51, FourRegsEVSYS_EVGEN52, FourRegsEVSYS_EVGEN53, FourRegsEVSYS_EVGEN54, FourRegsEVSYS_EVGEN55, FourRegsEVSYS_EVGEN56, FourRegsEVSYS_EVGEN57,
    FourRegsEVSYS_EVGEN58, FourRegsEVSYS_EVGEN59, FourRegsEVSYS_EVGEN5A, FourRegsEVSYS_EVGEN5B, FourRegsEVSYS_EVGEN5C, FourRegsEVSYS_EVGEN5D, FourRegsEVSYS_EVGEN5E, FourRegsEVSYS_EVGEN5F,
    FourRegsEVSYS_EVGEN60, FourRegsEVSYS_EVGEN61, FourRegsEVSYS_EVGEN62, FourRegsEVSYS_EVGEN63, FourRegsEVSYS_EVGEN64, FourRegsEVSYS_EVGEN65, FourRegsEVSYS_EVGEN66, FourRegsEVSYS_EVGEN67,
    FourRegsEVSYS_EVGEN68, FourRegsEVSYS_EVGEN69, FourRegsEVSYS_EVGEN6A, FourRegsEVSYS_EVGEN6B, FourRegsEVSYS_EVGEN6C, FourRegsEVSYS_EVGEN6D, FourRegsEVSYS_EVGEN6E, FourRegsEVSYS_EVGEN6F,
    FourRegsEVSYS_EVGEN70, FourRegsEVSYS_EVGEN71, FourRegsEVSYS_EVGEN72, FourRegsEVSYS_EVGEN73, FourRegsEVSYS_EVGEN74, FourRegsEVSYS_EVGEN75, FourRegsEVSYS_EVGEN76, FourRegsEVSYS_EVGEN77,
};
// table 31-2 (datasheet rev E)
static const char* FourRegsEVSYS_USER00 = "RTC_TAMPER";
static const char* FourRegsEVSYS_USER01 = "PORT_EV0";
static const char* FourRegsEVSYS_USER02 = "PORT_EV1";
static const char* FourRegsEVSYS_USER03 = "PORT_EV2";
static const char* FourRegsEVSYS_USER04 = "PORT_EV3";
static const char* FourRegsEVSYS_USER05 = "DMAC_CH0";
static const char* FourRegsEVSYS_USER06 = "DMAC_CH1";
static const char* FourRegsEVSYS_USER07 = "DMAC_CH2";
static const char* FourRegsEVSYS_USER08 = "DMAC_CH3";
static const char* FourRegsEVSYS_USER09 = "DMAC_CH4";
static const char* FourRegsEVSYS_USER10 = "DMAC_CH5";
static const char* FourRegsEVSYS_USER11 = "DMAC_CH6";
static const char* FourRegsEVSYS_USER12 = "DMAC_CH7";
static const char* FourRegsEVSYS_USER13 = FourRegs__RESERVED;
static const char* FourRegsEVSYS_USER14 = "CM4_TRACE_START";
static const char* FourRegsEVSYS_USER15 = "CM4_TRACE_STOP";
static const char* FourRegsEVSYS_USER16 = "CM4_TRACE_TRIG";
static const char* FourRegsEVSYS_USER17 = "TCC0_EV0";
static const char* FourRegsEVSYS_USER18 = "TCC0_EV1";
static const char* FourRegsEVSYS_USER19 = "TCC0_MC0";
static const char* FourRegsEVSYS_USER20 = "TCC0_MC1";
static const char* FourRegsEVSYS_USER21 = "TCC0_MC2";
static const char* FourRegsEVSYS_USER22 = "TCC0_MC3";
static const char* FourRegsEVSYS_USER23 = "TCC0_MC4";
static const char* FourRegsEVSYS_USER24 = "TCC0_MC5";
static const char* FourRegsEVSYS_USER25 = "TCC1_EV0";
static const char* FourRegsEVSYS_USER26 = "TCC1_EV1";
static const char* FourRegsEVSYS_USER27 = "TCC1_MC0";
static const char* FourRegsEVSYS_USER28 = "TCC1_MC1";
static const char* FourRegsEVSYS_USER29 = "TCC1_MC2";
static const char* FourRegsEVSYS_USER30 = "TCC1_MC3";
static const char* FourRegsEVSYS_USER31 = "TCC2_EV0";
static const char* FourRegsEVSYS_USER32 = "TCC2_EV1";
static const char* FourRegsEVSYS_USER33 = "TCC2_MC0";
static const char* FourRegsEVSYS_USER34 = "TCC2_MC1";
static const char* FourRegsEVSYS_USER35 = "TCC2_MC2";
static const char* FourRegsEVSYS_USER36 = "TCC3_EV0";
static const char* FourRegsEVSYS_USER37 = "TCC3_EV1";
static const char* FourRegsEVSYS_USER38 = "TCC3_MC0";
static const char* FourRegsEVSYS_USER39 = "TCC3_MC1";
static const char* FourRegsEVSYS_USER40 = "TCC4_EV0";
static const char* FourRegsEVSYS_USER41 = "TCC4_EV1";
static const char* FourRegsEVSYS_USER42 = "TCC4_MC0";
static const char* FourRegsEVSYS_USER43 = "TCC4_MC1";
static const char* FourRegsEVSYS_USER44 = "TC0_EVU";
static const char* FourRegsEVSYS_USER45 = "TC1_EVU";
static const char* FourRegsEVSYS_USER46 = "TC2_EVU";
static const char* FourRegsEVSYS_USER47 = "TC3_EVU";
static const char* FourRegsEVSYS_USER48 = "TC4_EVU";
static const char* FourRegsEVSYS_USER49 = "TC5_EVU";
static const char* FourRegsEVSYS_USER50 = "TC6_EVU";
static const char* FourRegsEVSYS_USER51 = "TC7_EVU";
static const char* FourRegsEVSYS_USER52 = "PDEC_EVU0";
static const char* FourRegsEVSYS_USER53 = "PDEC_EVU1";
static const char* FourRegsEVSYS_USER54 = "PDEC_EVU2";
static const char* FourRegsEVSYS_USER55 = "ADC0_START";
static const char* FourRegsEVSYS_USER56 = "ADC0_SYNC";
static const char* FourRegsEVSYS_USER57 = "ADC1_START";
static const char* FourRegsEVSYS_USER58 = "ADC1_SYNC";
static const char* FourRegsEVSYS_USER59 = "AC_SOC0";
static const char* FourRegsEVSYS_USER60 = "AC_SOC1";
static const char* FourRegsEVSYS_USER61 = "DAC_START0";
static const char* FourRegsEVSYS_USER62 = "DAC_START1";
static const char* FourRegsEVSYS_USER63 = "CCL_LUTIN0";
static const char* FourRegsEVSYS_USER64 = "CCL_LUTIN1";
static const char* FourRegsEVSYS_USER65 = "CCL_LUTIN2";
static const char* FourRegsEVSYS_USER66 = "CCL_LUTIN3";
static const char* const FourRegsEVSYS_USERs[] = {
    FourRegsEVSYS_USER00, FourRegsEVSYS_USER01, FourRegsEVSYS_USER02, FourRegsEVSYS_USER03, FourRegsEVSYS_USER04, FourRegsEVSYS_USER05, FourRegsEVSYS_USER06, FourRegsEVSYS_USER07,
    FourRegsEVSYS_USER08, FourRegsEVSYS_USER09, FourRegsEVSYS_USER10, FourRegsEVSYS_USER11, FourRegsEVSYS_USER12, FourRegsEVSYS_USER13, FourRegsEVSYS_USER14, FourRegsEVSYS_USER15,
    FourRegsEVSYS_USER16, FourRegsEVSYS_USER17, FourRegsEVSYS_USER18, FourRegsEVSYS_USER19, FourRegsEVSYS_USER20, FourRegsEVSYS_USER21, FourRegsEVSYS_USER22, FourRegsEVSYS_USER23,
    FourRegsEVSYS_USER24, FourRegsEVSYS_USER25, FourRegsEVSYS_USER26, FourRegsEVSYS_USER27, FourRegsEVSYS_USER28, FourRegsEVSYS_USER29, FourRegsEVSYS_USER30, FourRegsEVSYS_USER31,
    FourRegsEVSYS_USER32, FourRegsEVSYS_USER33, FourRegsEVSYS_USER34, FourRegsEVSYS_USER35, FourRegsEVSYS_USER36, FourRegsEVSYS_USER37, FourRegsEVSYS_USER38, FourRegsEVSYS_USER39,
    FourRegsEVSYS_USER40, FourRegsEVSYS_USER41, FourRegsEVSYS_USER42, FourRegsEVSYS_USER43, FourRegsEVSYS_USER44, FourRegsEVSYS_USER45, FourRegsEVSYS_USER46, FourRegsEVSYS_USER47,
    FourRegsEVSYS_USER48, FourRegsEVSYS_USER49, FourRegsEVSYS_USER50, FourRegsEVSYS_USER51, FourRegsEVSYS_USER52, FourRegsEVSYS_USER53, FourRegsEVSYS_USER54, FourRegsEVSYS_USER55,
    FourRegsEVSYS_USER56, FourRegsEVSYS_USER57, FourRegsEVSYS_USER58, FourRegsEVSYS_USER59, FourRegsEVSYS_USER60, FourRegsEVSYS_USER61, FourRegsEVSYS_USER62, FourRegsEVSYS_USER63,
    FourRegsEVSYS_USER64, FourRegsEVSYS_USER65, FourRegsEVSYS_USER66,
};
void printFourRegEVSYS(FourRegOptions &opts) {
    uint8_t id;
    opts.print.println("--------------------------- EVSYS");

    opts.print.print("PRICTRL: ");
    PRINTFLAG(EVSYS->PRICTRL, RREN);
    PRINTNL();

    for (id = 0; id < 12; id++) {
        if ((EVSYS->Channel[id].CHANNEL.bit.EVGEN == 0) && !opts.showDisabled) {
            continue;
        }
        if (FourRegsEVSYS_EVGENs[EVSYS->Channel[id].CHANNEL.bit.EVGEN] == FourRegs__RESERVED) {
            continue;
        }
        opts.print.print("CHANNEL");
        PRINTPAD2(id);
        opts.print.print(": ");
        opts.print.print(FourRegsEVSYS_EVGENs[EVSYS->Channel[id].CHANNEL.bit.EVGEN]);
        opts.print.print(" path=");
        switch (EVSYS->Channel[id].CHANNEL.bit.PATH) {
            case 0x0: opts.print.print("SYNC"); break;
            case 0x1: opts.print.print("RESYNC"); break;
            case 0x2: opts.print.print("ASYNC"); break;
            default:  opts.print.print(FourRegs__UNKNOWN); break;
        }
        PRINTFLAG(EVSYS->Channel[id].CHANNEL, RUNSTDBY);
        PRINTFLAG(EVSYS->Channel[id].CHANNEL, ONDEMAND);
        PRINTNL();
    }

    for (id = 0; id < 67; id++) {
        uint8_t chid = EVSYS->USER[id].bit.CHANNEL;
        if ((EVSYS->Channel[chid].CHANNEL.bit.EVGEN == 0) && !opts.showDisabled) {
            continue;
        }
        opts.print.print("USER");
        PRINTPAD2(id);
        opts.print.print(":  ");
        opts.print.print(FourRegsEVSYS_USERs[id]);
        if (FourRegsEVSYS_USERs[id] == FourRegs__RESERVED) {
            PRINTNL();
            continue;
        }
        opts.print.print(" CHANNEL=");
        opts.print.print(chid);
        PRINTNL();
    }
}


// table 14-4 (datasheet rev E)
static const char* FourRegsGCLK_SRC00 = "XOSC0";
static const char* FourRegsGCLK_SRC01 = "XOSC1";
static const char* FourRegsGCLK_SRC02 = "GCLKIN";
static const char* FourRegsGCLK_SRC03 = "GCLKGEN1";
static const char* FourRegsGCLK_SRC04 = "OSCULP32K";
static const char* FourRegsGCLK_SRC05 = "XOSC32K";
static const char* FourRegsGCLK_SRC06 = "DFLL48M";
static const char* FourRegsGCLK_SRC07 = "DPLL0";
static const char* FourRegsGCLK_SRC08 = "DPLL1";
static const char* const FourRegsGCLK_SRCs[] = {
    FourRegsGCLK_SRC00, FourRegsGCLK_SRC01, FourRegsGCLK_SRC02, FourRegsGCLK_SRC03,
    FourRegsGCLK_SRC04, FourRegsGCLK_SRC05, FourRegsGCLK_SRC06, FourRegsGCLK_SRC07,
    FourRegsGCLK_SRC08,  FourRegs__UNKNOWN,  FourRegs__UNKNOWN,  FourRegs__UNKNOWN,
    FourRegs__UNKNOWN,   FourRegs__UNKNOWN,  FourRegs__UNKNOWN,  FourRegs__UNKNOWN,
};
// table 14-9 (datasheet rev E)
static const char* FourRegsGCLK_CHAN00 = "OSCCTRL_DFLL48M_REF";
static const char* FourRegsGCLK_CHAN01 = "OSCCTRL_DPLL0_REF";
static const char* FourRegsGCLK_CHAN02 = "OSCCTRL_DPLL1_REF";
static const char* FourRegsGCLK_CHAN03 = "PCH03(slow)";
static const char* FourRegsGCLK_CHAN04 = "EIC";
static const char* FourRegsGCLK_CHAN05 = "FREQM_MSR";
static const char* FourRegsGCLK_CHAN06 = "FREQM_REF";
static const char* FourRegsGCLK_CHAN07 = "SERCOM0_CORE";
static const char* FourRegsGCLK_CHAN08 = "SERCOM1_CORE";
static const char* FourRegsGCLK_CHAN09 = "TC0_TC1";
static const char* FourRegsGCLK_CHAN10 = "USB";
static const char* FourRegsGCLK_CHAN11 = "EVSYS0";
static const char* FourRegsGCLK_CHAN12 = "EVSYS1";
static const char* FourRegsGCLK_CHAN13 = "EVSYS2";
static const char* FourRegsGCLK_CHAN14 = "EVSYS3";
static const char* FourRegsGCLK_CHAN15 = "EVSYS4";
static const char* FourRegsGCLK_CHAN16 = "EVSYS5";
static const char* FourRegsGCLK_CHAN17 = "EVSYS6";
static const char* FourRegsGCLK_CHAN18 = "EVSYS7";
static const char* FourRegsGCLK_CHAN19 = "EVSYS8";
static const char* FourRegsGCLK_CHAN20 = "EVSYS9";
static const char* FourRegsGCLK_CHAN21 = "EVSYS10";
static const char* FourRegsGCLK_CHAN22 = "EVSYS11";
static const char* FourRegsGCLK_CHAN23 = "SERCOM2_CORE";
static const char* FourRegsGCLK_CHAN24 = "SERCOM3_CORE";
static const char* FourRegsGCLK_CHAN25 = "TCC0_TCC1";
static const char* FourRegsGCLK_CHAN26 = "TC2_TC3";
static const char* FourRegsGCLK_CHAN27 = "CAN0";
static const char* FourRegsGCLK_CHAN28 = "CAN1";
static const char* FourRegsGCLK_CHAN29 = "TCC2_TCC3";
static const char* FourRegsGCLK_CHAN30 = "TC4_TC5";
static const char* FourRegsGCLK_CHAN31 = "PDEC";
static const char* FourRegsGCLK_CHAN32 = "AC";
static const char* FourRegsGCLK_CHAN33 = "CCL";
static const char* FourRegsGCLK_CHAN34 = "SERCOM4_CORE";
static const char* FourRegsGCLK_CHAN35 = "SERCOM5_CORE";
static const char* FourRegsGCLK_CHAN36 = "SERCOM6_CORE";
static const char* FourRegsGCLK_CHAN37 = "SERCOM7_CORE";
static const char* FourRegsGCLK_CHAN38 = "TCC4";
static const char* FourRegsGCLK_CHAN39 = "TC6_TC7";
static const char* FourRegsGCLK_CHAN40 = "ADC0";
static const char* FourRegsGCLK_CHAN41 = "ADC1";
static const char* FourRegsGCLK_CHAN42 = "DAC";
static const char* FourRegsGCLK_CHAN43 = "I2S";
static const char* FourRegsGCLK_CHAN44 = "I2S";
static const char* FourRegsGCLK_CHAN45 = "SDHC0";
static const char* FourRegsGCLK_CHAN46 = "SDHC1";
static const char* FourRegsGCLK_CHAN47 = "CM4_TRACE";
static const char* const FourRegsGCLK_CHANs[] = {
    FourRegsGCLK_CHAN00, FourRegsGCLK_CHAN01, FourRegsGCLK_CHAN02, FourRegsGCLK_CHAN03, FourRegsGCLK_CHAN04, FourRegsGCLK_CHAN05, FourRegsGCLK_CHAN06, FourRegsGCLK_CHAN07,
    FourRegsGCLK_CHAN08, FourRegsGCLK_CHAN09, FourRegsGCLK_CHAN10, FourRegsGCLK_CHAN11, FourRegsGCLK_CHAN12, FourRegsGCLK_CHAN13, FourRegsGCLK_CHAN14, FourRegsGCLK_CHAN15,
    FourRegsGCLK_CHAN16, FourRegsGCLK_CHAN17, FourRegsGCLK_CHAN18, FourRegsGCLK_CHAN19, FourRegsGCLK_CHAN20, FourRegsGCLK_CHAN21, FourRegsGCLK_CHAN22, FourRegsGCLK_CHAN23,
    FourRegsGCLK_CHAN24, FourRegsGCLK_CHAN25, FourRegsGCLK_CHAN26, FourRegsGCLK_CHAN27, FourRegsGCLK_CHAN28, FourRegsGCLK_CHAN29, FourRegsGCLK_CHAN30, FourRegsGCLK_CHAN31,
    FourRegsGCLK_CHAN32, FourRegsGCLK_CHAN33, FourRegsGCLK_CHAN34, FourRegsGCLK_CHAN35, FourRegsGCLK_CHAN36, FourRegsGCLK_CHAN37, FourRegsGCLK_CHAN38, FourRegsGCLK_CHAN39,
    FourRegsGCLK_CHAN40, FourRegsGCLK_CHAN41, FourRegsGCLK_CHAN42, FourRegsGCLK_CHAN43, FourRegsGCLK_CHAN44, FourRegsGCLK_CHAN45, FourRegsGCLK_CHAN46, FourRegsGCLK_CHAN47,
};
void printFourRegGCLK(FourRegOptions &opts) {
    opts.print.println("--------------------------- GCLK");
    for (uint8_t genid = 0; genid < 12; genid++) {
        GCLK_GENCTRL_Type gen;
        while (GCLK->SYNCBUSY.reg) {}
        COPYVOL(gen, GCLK->GENCTRL[genid]);
        if (!gen.bit.GENEN && !opts.showDisabled) {
            continue;
        }
        opts.print.print("GEN");
        PRINTPAD2(genid);
        opts.print.print(":  ");
        if (!gen.bit.GENEN) {
            opts.print.println(FourRegs__DISABLED);
            continue;
        }
        opts.print.print(FourRegsGCLK_SRCs[gen.bit.SRC]);
        opts.print.print("/");
        if (gen.bit.DIVSEL) {
            PRINTSCALE(gen.bit.DIV + 1);
        } else {
            if (gen.bit.DIV > 1) {
                opts.print.print(gen.bit.DIV);
            } else {
                opts.print.print(1);
            }
        }
        PRINTFLAG(gen, IDC);
        PRINTFLAG(gen, OOV);
        PRINTFLAG(gen, OE);
        PRINTFLAG(gen, RUNSTDBY);
        PRINTNL();
    }
    opts.print.println("GCLK_MAIN:  GEN00 (always)");
    for (uint8_t pchid = 1; pchid < 48; pchid++) {
        GCLK_PCHCTRL_Type pch;
        while (GCLK->SYNCBUSY.reg) {}
        COPYVOL(pch, GCLK->PCHCTRL[pchid]);
        if (!pch.bit.CHEN && !opts.showDisabled) {
            continue;
        }
        opts.print.print("GCLK_");
        opts.print.print(FourRegsGCLK_CHANs[pchid]);
        opts.print.print(": ");
        if (pch.bit.CHEN) {
            opts.print.print(" GEN");
            PRINTPAD2(pch.bit.GEN);
            PRINTFLAG(pch, WRTLOCK);
            PRINTNL();
        } else {
            opts.print.print(" ");
            opts.print.println(FourRegs__DISABLED);
        }
    }
}


void printFourRegMCLK(FourRegOptions &opts) {
    opts.print.println("--------------------------- MCLK");
    opts.print.print("HSDIV:  /");
    opts.print.print(MCLK->HSDIV.reg);
    PRINTNL();
    opts.print.print("CPUDIV:  /");
    opts.print.print(MCLK->CPUDIV.reg);
    PRINTNL();

    opts.print.print("AHBMASK: ");
    if (MCLK->AHBMASK.bit.HPB0_) { opts.print.print(" HPB0"); }
    if (MCLK->AHBMASK.bit.HPB1_) { opts.print.print(" HPB1"); }
    if (MCLK->AHBMASK.bit.HPB2_) { opts.print.print(" HPB2"); }
    if (MCLK->AHBMASK.bit.HPB3_) { opts.print.print(" HPB3"); }
    if (MCLK->AHBMASK.bit.DSU_) { opts.print.print(" DSU"); }
    if (MCLK->AHBMASK.bit.HMATRIX_) { opts.print.print(" HMATRIX"); }
    if (MCLK->AHBMASK.bit.NVMCTRL_) { opts.print.print(" NVMCTRL"); }
    if (MCLK->AHBMASK.bit.HSRAM_) { opts.print.print(" HSRAM"); }
    if (MCLK->AHBMASK.bit.CMCC_) { opts.print.print(" CMCC"); }
    if (MCLK->AHBMASK.bit.DMAC_) { opts.print.print(" DMAC"); }
    if (MCLK->AHBMASK.bit.USB_) { opts.print.print(" USB"); }
    if (MCLK->AHBMASK.bit.BKUPRAM_) { opts.print.print(" BKUPRAM"); }
    if (MCLK->AHBMASK.bit.PAC_) { opts.print.print(" PAC"); }
    if (MCLK->AHBMASK.bit.QSPI_) { opts.print.print(" QSPI"); }
    if (MCLK->AHBMASK.bit.SDHC0_) { opts.print.print(" SDHC0"); }
    if (MCLK->AHBMASK.bit.SDHC1_) { opts.print.print(" SDHC1"); }
    if (MCLK->AHBMASK.bit.ICM_) { opts.print.print(" ICM"); }
    if (MCLK->AHBMASK.bit.PUKCC_) { opts.print.print(" PUKCC"); }
    if (MCLK->AHBMASK.bit.QSPI_2X_) { opts.print.print(" QSPI_2X"); }
    if (MCLK->AHBMASK.bit.NVMCTRL_SMEEPROM_) { opts.print.print(" NVMCTRL_SMEEPROM"); }
    if (MCLK->AHBMASK.bit.NVMCTRL_CACHE_) { opts.print.print(" NVMCTRL_CACHE"); }
    PRINTNL();

    opts.print.print("APBAMASK: ");
    if (MCLK->APBAMASK.bit.PAC_) { opts.print.print(" PAC"); }
    if (MCLK->APBAMASK.bit.PM_) { opts.print.print(" PM"); }
    if (MCLK->APBAMASK.bit.MCLK_) { opts.print.print(" MCLK"); }
    if (MCLK->APBAMASK.bit.RSTC_) { opts.print.print(" RSTC"); }
    if (MCLK->APBAMASK.bit.OSCCTRL_) { opts.print.print(" OSCCTRL"); }
    if (MCLK->APBAMASK.bit.OSC32KCTRL_) { opts.print.print(" OSC32KCTRL"); }
    if (MCLK->APBAMASK.bit.SUPC_) { opts.print.print(" SUPC"); }
    if (MCLK->APBAMASK.bit.GCLK_) { opts.print.print(" GCLK"); }
    if (MCLK->APBAMASK.bit.WDT_) { opts.print.print(" WDT"); }
    if (MCLK->APBAMASK.bit.RTC_) { opts.print.print(" RTC"); }
    if (MCLK->APBAMASK.bit.EIC_) { opts.print.print(" EIC"); }
    if (MCLK->APBAMASK.bit.FREQM_) { opts.print.print(" FREQM"); }
    if (MCLK->APBAMASK.bit.SERCOM0_) { opts.print.print(" SERCOM0"); }
    if (MCLK->APBAMASK.bit.SERCOM1_) { opts.print.print(" SERCOM1"); }
    if (MCLK->APBAMASK.bit.TC0_) { opts.print.print(" TC0"); }
    if (MCLK->APBAMASK.bit.TC1_) { opts.print.print(" TC1"); }
    PRINTNL();

    opts.print.print("APBBMASK: ");
    if (MCLK->APBBMASK.bit.USB_) { opts.print.print(" USB"); }
    if (MCLK->APBBMASK.bit.DSU_) { opts.print.print(" DSU"); }
    if (MCLK->APBBMASK.bit.NVMCTRL_) { opts.print.print(" NVMCTRL"); }
    if (MCLK->APBBMASK.bit.PORT_) { opts.print.print(" PORT"); }
    if (MCLK->APBBMASK.bit.HMATRIX_) { opts.print.print(" HMATRIX"); }
    if (MCLK->APBBMASK.bit.EVSYS_) { opts.print.print(" EVSYS"); }
    if (MCLK->APBBMASK.bit.SERCOM2_) { opts.print.print(" SERCOM2"); }
    if (MCLK->APBBMASK.bit.SERCOM3_) { opts.print.print(" SERCOM3"); }
    if (MCLK->APBBMASK.bit.TCC0_) { opts.print.print(" TCC0"); }
    if (MCLK->APBBMASK.bit.TCC1_) { opts.print.print(" TCC1"); }
    if (MCLK->APBBMASK.bit.TC2_) { opts.print.print(" TC2"); }
    if (MCLK->APBBMASK.bit.TC3_) { opts.print.print(" TC3"); }
    if (MCLK->APBBMASK.bit.TAL_) { opts.print.print(" TAL"); }
    if (MCLK->APBBMASK.bit.RAMECC_) { opts.print.print(" RAMECC"); }
    PRINTNL();

    opts.print.print("APBCMASK: ");
    if (MCLK->APBCMASK.bit.TCC2_) { opts.print.print(" TCC2"); }
    if (MCLK->APBCMASK.bit.TCC3_) { opts.print.print(" TCC3"); }
    if (MCLK->APBCMASK.bit.TC4_) { opts.print.print(" TC4"); }
    if (MCLK->APBCMASK.bit.TC5_) { opts.print.print(" TC5"); }
    if (MCLK->APBCMASK.bit.PDEC_) { opts.print.print(" PDEC"); }
    if (MCLK->APBCMASK.bit.AC_) { opts.print.print(" AC"); }
    if (MCLK->APBCMASK.bit.AES_) { opts.print.print(" AES"); }
    if (MCLK->APBCMASK.bit.TRNG_) { opts.print.print(" TRNG"); }
    if (MCLK->APBCMASK.bit.ICM_) { opts.print.print(" ICM"); }
    if (MCLK->APBCMASK.bit.QSPI_) { opts.print.print(" QSPI"); }
    if (MCLK->APBCMASK.bit.CCL_) { opts.print.print(" CCL"); }
    PRINTNL();

    opts.print.print("APBDMASK: ");
    if (MCLK->APBDMASK.bit.SERCOM4_) { opts.print.print(" SERCOM4"); }
    if (MCLK->APBDMASK.bit.SERCOM5_) { opts.print.print(" SERCOM5"); }
    if (MCLK->APBDMASK.bit.SERCOM6_) { opts.print.print(" SERCOM6"); }
    if (MCLK->APBDMASK.bit.SERCOM7_) { opts.print.print(" SERCOM7"); }
    if (MCLK->APBDMASK.bit.TCC4_) { opts.print.print(" TCC4"); }
    if (MCLK->APBDMASK.bit.TC6_) { opts.print.print(" TC6"); }
    if (MCLK->APBDMASK.bit.TC7_) { opts.print.print(" TC7"); }
    if (MCLK->APBDMASK.bit.ADC0_) { opts.print.print(" ADC0"); }
    if (MCLK->APBDMASK.bit.ADC1_) { opts.print.print(" ADC1"); }
    if (MCLK->APBDMASK.bit.DAC_) { opts.print.print(" DAC"); }
    if (MCLK->APBDMASK.bit.I2S_) { opts.print.print(" I2S"); }
    if (MCLK->APBDMASK.bit.PCC_) { opts.print.print(" PCC"); }
    PRINTNL();
}


void printFourRegNVMCTRL(FourRegOptions &opts) {
    opts.print.println("--------------------------- NVMCTRL");

    opts.print.print("CTRLA: ");
    PRINTFLAG(NVMCTRL->CTRLA, AUTOWS);
    PRINTFLAG(NVMCTRL->CTRLA, SUSPEN);
    opts.print.print(" wmode=");
    switch (NVMCTRL->CTRLA.bit.WMODE) {
        case 0x0: opts.print.print("MAN"); break;
        case 0x1: opts.print.print("ADW"); break;
        case 0x2: opts.print.print("AQW"); break;
        case 0x3: opts.print.print("PW"); break;
    }
    opts.print.print(" prm=");
    switch (NVMCTRL->CTRLA.bit.PRM) {
        case 0x0: opts.print.print("SEMIAUTO"); break;
        case 0x1: opts.print.print("FULLAUTO"); break;
        case 0x2: opts.print.print(FourRegs__RESERVED); break;
        case 0x3: opts.print.print("MANUAL"); break;
    }
    opts.print.print(" RWS=");
    opts.print.print(NVMCTRL->CTRLA.bit.RWS);
    PRINTFLAG(NVMCTRL->CTRLA, AHBNS0);
    PRINTFLAG(NVMCTRL->CTRLA, AHBNS1);
    PRINTFLAG(NVMCTRL->CTRLA, CACHEDIS0);
    PRINTFLAG(NVMCTRL->CTRLA, CACHEDIS1);
    PRINTNL();

    opts.print.print("PARAM:  NVMP=");
    opts.print.print(NVMCTRL->PARAM.bit.NVMP);
    opts.print.print(" psz=");
    opts.print.print(8 * (2 << NVMCTRL->PARAM.bit.PSZ));
    opts.print.print("bytes");
    PRINTFLAG(NVMCTRL->PARAM, SEE);
    PRINTNL();

    opts.print.print("STATUS: ");
    PRINTFLAG(NVMCTRL->STATUS, AFIRST);
    PRINTFLAG(NVMCTRL->STATUS, BPDIS);
    opts.print.print(" bootprot=");
    opts.print.print(8 * (15 - NVMCTRL->STATUS.bit.BOOTPROT));
    opts.print.print("kb");
    PRINTNL();

    opts.print.print("RUNLOCK:  ");
    for (uint8_t i = 0; i < 32; i++) {
        opts.print.print(
                (NVMCTRL->RUNLOCK.bit.RUNLOCK & (1 << i)) ?
                "-" :
                "X"
                );
    }
    PRINTNL();

    opts.print.print("SEECFG:  wmode=");
    opts.print.print(
            NVMCTRL->SEECFG.bit.WMODE ?
            "BUFFERED" :
            "UNBUFFERED"
            );
    PRINTFLAG(NVMCTRL->SEECFG, APRDIS);
    PRINTNL();

    opts.print.print("SEESTAT:  ASEES=");
    opts.print.print(NVMCTRL->SEESTAT.bit.ASEES);
    PRINTFLAG(NVMCTRL->SEESTAT, LOCK);
    PRINTFLAG(NVMCTRL->SEESTAT, RLOCK);
    opts.print.print(" SBLK=");
    opts.print.print(NVMCTRL->SEESTAT.bit.SBLK);
    opts.print.print(" PSZ=");
    opts.print.print(NVMCTRL->SEESTAT.bit.PSZ);
    PRINTNL();
}


void printFourRegOSC32KCTRL(FourRegOptions &opts) {
    opts.print.println("--------------------------- OSC32KCTRL");

    opts.print.print("EVCTRL: ");
    PRINTFLAG(OSC32KCTRL->EVCTRL, CFDEO);
    PRINTNL();

    opts.print.print("RTCCTRL:  rtcsel=");
    switch(OSC32KCTRL->RTCCTRL.bit.RTCSEL) {
        case 0x0: opts.print.print("ULP1K"); break;
        case 0x1: opts.print.print("ULP32K"); break;
        case 0x4: opts.print.print("XOSC1K"); break;
        case 0x5: opts.print.print("XOSC32K"); break;
        default: opts.print.print(FourRegs__UNKNOWN); break;
    }
    PRINTNL();

    if (OSC32KCTRL->XOSC32K.bit.ENABLE || opts.showDisabled) {
        opts.print.print("XOSC32K: ");
        PRINTFLAG(OSC32KCTRL->XOSC32K, ENABLE);
        PRINTFLAG(OSC32KCTRL->XOSC32K, XTALEN);
        PRINTFLAG(OSC32KCTRL->XOSC32K, EN32K);
        PRINTFLAG(OSC32KCTRL->XOSC32K, EN1K);
        PRINTFLAG(OSC32KCTRL->XOSC32K, RUNSTDBY);
        PRINTFLAG(OSC32KCTRL->XOSC32K, ONDEMAND);
        PRINTFLAG(OSC32KCTRL->XOSC32K, WRTLOCK);
        opts.print.print(" STARTUP=");
        PRINTHEX(OSC32KCTRL->XOSC32K.bit.STARTUP);
        opts.print.print(" CGM=");
        PRINTHEX(OSC32KCTRL->XOSC32K.bit.CGM);
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.print("XOSC32K:  ");
            opts.print.println(FourRegs__DISABLED);
        }
    }

    if (OSC32KCTRL->CFDCTRL.bit.CFDEN || opts.showDisabled) {
        opts.print.print("CFDCTRL: ");
        PRINTFLAG(OSC32KCTRL->CFDCTRL, CFDEN);
        PRINTFLAG(OSC32KCTRL->CFDCTRL, SWBACK);
        PRINTFLAG(OSC32KCTRL->CFDCTRL, CFDPRESC);
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.print("CFDCTRL:  ");
            opts.print.println(FourRegs__DISABLED);
        }
    }

    opts.print.print("OSCULP32K: ");
    PRINTFLAG(OSC32KCTRL->OSCULP32K, EN32K);
    PRINTFLAG(OSC32KCTRL->OSCULP32K, EN1K);
    PRINTFLAG(OSC32KCTRL->OSCULP32K, WRTLOCK);
    opts.print.print(" CALIB=");
    PRINTHEX(OSC32KCTRL->OSCULP32K.bit.CALIB);
    PRINTNL();
}


void printFourRegOSCCTRL(FourRegOptions &opts) {
    opts.print.println("--------------------------- OSCCTRL");

    opts.print.print("EVCTRL: ");
    PRINTFLAG(OSCCTRL->EVCTRL, CFDEO0);
    PRINTFLAG(OSCCTRL->EVCTRL, CFDEO1);
    PRINTNL();

    OSCCTRL_DFLLCTRLA_Type ctrla;
    while (OSCCTRL->DFLLSYNC.bit.ENABLE) {}
    COPYVOL(ctrla, OSCCTRL->DFLLCTRLA);
    if (ctrla.bit.ENABLE || opts.showDisabled) {
        opts.print.print("DFLLCTRLA: ");
        PRINTFLAG(ctrla, ENABLE);
        PRINTFLAG(ctrla, RUNSTDBY);
        PRINTFLAG(ctrla, ONDEMAND);
        PRINTNL();

        opts.print.print("DFLLCTRLB: ");
        OSCCTRL_DFLLCTRLB_Type ctrlb;
        while (OSCCTRL->DFLLSYNC.bit.DFLLCTRLB) {}
        COPYVOL(ctrlb, OSCCTRL->DFLLCTRLB);
        opts.print.print(ctrlb.bit.MODE ? " closed-loop" : " open-loop");
        PRINTFLAG(ctrlb, STABLE);
        PRINTFLAG(ctrlb, LLAW);
        PRINTFLAG(ctrlb, USBCRM);
        PRINTFLAG(ctrlb, CCDIS);
        PRINTFLAG(ctrlb, QLDIS);
        PRINTFLAG(ctrlb, BPLCKC);
        PRINTFLAG(ctrlb, WAITLOCK);
        PRINTNL();

        OSCCTRL_DFLLVAL_Type dfllval;
        while (OSCCTRL->DFLLSYNC.bit.DFLLVAL) {}
        COPYVOL(dfllval, OSCCTRL->DFLLVAL);
        opts.print.print("DFLLVAL:  FINE=");
        opts.print.print(dfllval.bit.FINE);
        opts.print.print(" COARSE=");
        opts.print.print(dfllval.bit.COARSE);
        if (ctrlb.bit.MODE) {
            opts.print.print(" DIFF=");
            opts.print.print(dfllval.bit.DIFF);
        }
        PRINTNL();

        OSCCTRL_DFLLMUL_Type dfllmul;
        while (OSCCTRL->DFLLSYNC.bit.DFLLMUL) {}
        COPYVOL(dfllmul, OSCCTRL->DFLLMUL);
        opts.print.print("DFLLMUL:  MUL=");
        opts.print.print(dfllmul.bit.MUL);
        opts.print.print(" FSTEP=");
        opts.print.print(dfllmul.bit.FSTEP);
        opts.print.print(" CSTEP=");
        opts.print.print(dfllmul.bit.CSTEP);
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.print("DFLLCTRLA:  ");
            opts.print.println(FourRegs__DISABLED);
        }
    }

    for (uint8_t xoscid = 0; xoscid < 2; xoscid++) {
        if (OSCCTRL->XOSCCTRL[xoscid].bit.ENABLE || opts.showDisabled) {
            opts.print.print("XOSCCTRL");
            opts.print.print(xoscid);
            opts.print.print(": ");
            PRINTFLAG(OSCCTRL->XOSCCTRL[xoscid], ENABLE);
            PRINTFLAG(OSCCTRL->XOSCCTRL[xoscid], XTALEN);
            PRINTFLAG(OSCCTRL->XOSCCTRL[xoscid], RUNSTDBY);
            PRINTFLAG(OSCCTRL->XOSCCTRL[xoscid], ONDEMAND);
            PRINTFLAG(OSCCTRL->XOSCCTRL[xoscid], LOWBUFGAIN);
            opts.print.print(" IPTAT=");
            opts.print.print(OSCCTRL->XOSCCTRL[xoscid].bit.IPTAT);
            opts.print.print(" IMULT=");
            opts.print.print(OSCCTRL->XOSCCTRL[xoscid].bit.IMULT);
            PRINTFLAG(OSCCTRL->XOSCCTRL[xoscid], ENALC);
            PRINTFLAG(OSCCTRL->XOSCCTRL[xoscid], SWBEN);
            opts.print.print(" STARTUP=");
            PRINTHEX(OSCCTRL->XOSCCTRL[xoscid].bit.STARTUP);
            opts.print.print(" cfdpresc=");
            opts.print.print(OSCCTRL->XOSCCTRL[xoscid].bit.CFDPRESC);
            PRINTNL();
        } else {
            if (opts.showDisabled) {
                opts.print.print("XOSCCTRL");
                opts.print.print(xoscid);
                opts.print.print(":  ");
                opts.print.println(FourRegs__DISABLED);
            }
        }
    }

    for (uint8_t dpllid = 0; dpllid < 2; dpllid++) {
        if (OSCCTRL->Dpll[dpllid].DPLLCTRLA.bit.ENABLE || opts.showDisabled) {
            opts.print.print("DPLL");
            opts.print.print(dpllid);
            opts.print.print(": ");
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLA, ENABLE);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLA, RUNSTDBY);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLA, ONDEMAND);
            //FUTURE -- calculate actual clock speed [out = REFCLK * (LDR+1+(LDRFRAC/32))]
            uint8_t refclk = OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.REFCLK;
            opts.print.print(" refclk=");
            switch (refclk) {
                case 0x0:
                    opts.print.print("GCLK_OSCCTRL_DPLL");
                    opts.print.print(dpllid);
                    opts.print.print("_REF");
                    break;
                case 0x1: opts.print.print("XOSC32"); break;
                case 0x2:
                          opts.print.print("XOSC0/");
                          PRINTSCALE(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.DIV + 1);
                          break;
                case 0x3:
                          opts.print.print("XOSC1/");
                          PRINTSCALE(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.DIV + 1);
                          break;
                default: opts.print.print(FourRegs__RESERVED); break;
            }
            opts.print.print(" ldr=");
            opts.print.print(OSCCTRL->Dpll[dpllid].DPLLRATIO.bit.LDR);
            opts.print.print(".");
            opts.print.print(OSCCTRL->Dpll[dpllid].DPLLRATIO.bit.LDRFRAC);
            opts.print.print(" FILTER=");
            PRINTHEX(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.FILTER);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLB, WUF);
            opts.print.print(" LTIME=");
            PRINTHEX(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.LTIME);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLB, LBYPASS);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLB, DCOEN);
            if (OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.DCOEN) {
                opts.print.print(" DCOFILTER=");
                PRINTHEX(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.DCOFILTER);
            }
            PRINTNL();
        } else {
            if (opts.showDisabled) {
                opts.print.print("DPLL");
                opts.print.print(dpllid);
                opts.print.print(":  ");
                opts.print.println(FourRegs__DISABLED);
            }
        }
    }
}


void printFourRegRTC_BKUP(FourRegOptions &opts, volatile RTC_BKUP_Type *bkup) {
    for (uint8_t id = 0; id < 8; id++) {
        opts.print.print("BKUP");
        opts.print.print(id);
        opts.print.print(":  ");
        opts.print.println(bkup[id].bit.BKUP);
    }
}

void printFourRegRTC_FREQCORR(FourRegOptions &opts, volatile RTC_FREQCORR_Type &freqcorr) {
    opts.print.print("FREQCORR:  ");
    opts.print.print(freqcorr.bit.SIGN ? '-' : '+');
    opts.print.println(freqcorr.bit.VALUE);
}

void printFourRegRTC_GP(FourRegOptions &opts, volatile RTC_GP_Type *gp) {
    for (uint8_t id = 0; id < 4; id++) {
        opts.print.print("GP");
        opts.print.print(id);
        opts.print.print(":  ");
        opts.print.println(gp[id].bit.GP);
    }
}

void printFourRegRTC_TAMPCTRLn(FourRegOptions &opts, uint8_t idx, uint8_t inact, uint8_t tamlvl, uint8_t debnc) {
    if (inact == 0x0 && !opts.showDisabled) {
        return;
    }
    opts.print.print(" ");
    opts.print.print(idx);
    opts.print.print(":");
    switch (inact) {
        case 0x0: opts.print.print("OFF"); break;
        case 0x1: opts.print.print("WAKE"); break;
        case 0x2: opts.print.print("CAPTURE"); break;
        case 0x3: opts.print.print("ACTL"); break;
    }
    opts.print.print(tamlvl ? ",rise" : ",fall");
    if (debnc) {
        opts.print.print(",DEBNC");
    }
}

void printFourRegRTC_TAMPCTRL(FourRegOptions &opts, volatile RTC_TAMPCTRL_Type &tampctrl) {
    opts.print.print("TAMPCTRL: ");
    printFourRegRTC_TAMPCTRLn(opts, 0, tampctrl.bit.IN0ACT, tampctrl.bit.TAMLVL0, tampctrl.bit.DEBNC0);
    printFourRegRTC_TAMPCTRLn(opts, 1, tampctrl.bit.IN1ACT, tampctrl.bit.TAMLVL1, tampctrl.bit.DEBNC1);
    printFourRegRTC_TAMPCTRLn(opts, 2, tampctrl.bit.IN2ACT, tampctrl.bit.TAMLVL2, tampctrl.bit.DEBNC2);
    printFourRegRTC_TAMPCTRLn(opts, 3, tampctrl.bit.IN3ACT, tampctrl.bit.TAMLVL3, tampctrl.bit.DEBNC3);
    printFourRegRTC_TAMPCTRLn(opts, 4, tampctrl.bit.IN4ACT, tampctrl.bit.TAMLVL4, tampctrl.bit.DEBNC4);
    PRINTNL();
}

void printFourRegRTC_MODE0(FourRegOptions &opts) {
    uint8_t id;
    opts.print.println("--------------------------- RTC COUNT32");

    while (RTC->MODE0.SYNCBUSY.bit.ENABLE) {}
    opts.print.print("CTRLA: ");
    PRINTFLAG(RTC->MODE0.CTRLA, ENABLE);
    opts.print.print(" MODE=");
    PRINTHEX(RTC->MODE0.CTRLA.bit.MODE);
    PRINTFLAG(RTC->MODE0.CTRLA, MATCHCLR);
    opts.print.print(" PRESCALER=");
    PRINTHEX(RTC->MODE0.CTRLA.bit.PRESCALER);
    PRINTFLAG(RTC->MODE0.CTRLA, BKTRST);
    PRINTFLAG(RTC->MODE0.CTRLA, GPTRST);
    PRINTFLAG(RTC->MODE0.CTRLA, COUNTSYNC);
    PRINTNL();

    opts.print.print("CTRLB: ");
    PRINTFLAG(RTC->MODE0.CTRLB, GP0EN);
    PRINTFLAG(RTC->MODE0.CTRLB, GP2EN);
    PRINTFLAG(RTC->MODE0.CTRLB, DEBMAJ);
    PRINTFLAG(RTC->MODE0.CTRLB, DEBASYNC);
    PRINTFLAG(RTC->MODE0.CTRLB, RTCOUT);
    PRINTFLAG(RTC->MODE0.CTRLB, DMAEN);
    opts.print.print(" DEBF=");
    PRINTHEX(RTC->MODE0.CTRLB.bit.DEBF);
    opts.print.print(" ACTF=");
    PRINTHEX(RTC->MODE0.CTRLB.bit.ACTF);
    PRINTNL();

    opts.print.print("EVCTRL: ");
    for (id = 0; id < 8; id++) {
        if (RTC->MODE0.EVCTRL.vec.PEREO & (1<<id)) {
            opts.print.print(" PEREO");
            opts.print.print(id);
        }
    }
    PRINTFLAG(RTC->MODE0.EVCTRL, CMPEO0);
    PRINTFLAG(RTC->MODE0.EVCTRL, CMPEO1);
    PRINTFLAG(RTC->MODE0.EVCTRL, TAMPEREO);
    PRINTFLAG(RTC->MODE0.EVCTRL, OVFEO);
    PRINTFLAG(RTC->MODE0.EVCTRL, TAMPEVEI);
    PRINTNL();

    while (RTC->MODE0.SYNCBUSY.bit.FREQCORR);
    printFourRegRTC_FREQCORR(opts, RTC->MODE0.FREQCORR);

    while (RTC->MODE0.SYNCBUSY.vec.COMP);
    for (id = 0; id < 2; id++) {
        opts.print.print("COMP");
        opts.print.print(id);
        opts.print.print(":  ");
        opts.print.print(RTC->MODE0.COMP[id].bit.COMP);
        PRINTNL();
    }

    while (RTC->MODE0.SYNCBUSY.vec.GP);
    printFourRegRTC_GP(opts, RTC->MODE0.GP);
    printFourRegRTC_TAMPCTRL(opts, RTC->MODE0.TAMPCTRL);

    opts.print.print("TIMESTAMP:  ");
    opts.print.print(RTC->MODE0.TIMESTAMP.bit.COUNT);
    PRINTNL();

    printFourRegRTC_BKUP(opts, RTC->MODE0.BKUP);
}

void printFourRegRTC_MODE1(FourRegOptions &opts) {
    uint8_t id;
    opts.print.println("--------------------------- RTC COUNT16");

    while (RTC->MODE1.SYNCBUSY.bit.ENABLE) {}
    opts.print.print("CTRLA: ");
    PRINTFLAG(RTC->MODE1.CTRLA, ENABLE);
    opts.print.print(" MODE=");
    PRINTHEX(RTC->MODE1.CTRLA.bit.MODE);
    opts.print.print(" PRESCALER=");
    PRINTHEX(RTC->MODE1.CTRLA.bit.PRESCALER);
    PRINTFLAG(RTC->MODE1.CTRLA, BKTRST);
    PRINTFLAG(RTC->MODE1.CTRLA, GPTRST);
    PRINTFLAG(RTC->MODE1.CTRLA, COUNTSYNC);
    PRINTNL();

    opts.print.print("CTRLB: ");
    PRINTFLAG(RTC->MODE1.CTRLB, GP0EN);
    PRINTFLAG(RTC->MODE1.CTRLB, GP2EN);
    PRINTFLAG(RTC->MODE1.CTRLB, DEBMAJ);
    PRINTFLAG(RTC->MODE1.CTRLB, DEBASYNC);
    PRINTFLAG(RTC->MODE1.CTRLB, RTCOUT);
    PRINTFLAG(RTC->MODE1.CTRLB, DMAEN);
    opts.print.print(" DEBF=");
    PRINTHEX(RTC->MODE1.CTRLB.bit.DEBF);
    opts.print.print(" ACTF=");
    PRINTHEX(RTC->MODE1.CTRLB.bit.ACTF);
    PRINTNL();

    opts.print.print("EVCTRL: ");
    for (id = 0; id < 8; id++) {
        if (RTC->MODE1.EVCTRL.vec.PEREO & (1<<id)) {
            opts.print.print(" PEREO");
            opts.print.print(id);
        }
    }
    PRINTFLAG(RTC->MODE1.EVCTRL, CMPEO0);
    PRINTFLAG(RTC->MODE1.EVCTRL, CMPEO1);
    PRINTFLAG(RTC->MODE1.EVCTRL, CMPEO2);
    PRINTFLAG(RTC->MODE1.EVCTRL, CMPEO3);
    PRINTFLAG(RTC->MODE1.EVCTRL, TAMPEREO);
    PRINTFLAG(RTC->MODE1.EVCTRL, OVFEO);
    PRINTFLAG(RTC->MODE1.EVCTRL, TAMPEVEI);
    PRINTNL();

    while (RTC->MODE1.SYNCBUSY.bit.FREQCORR);
    printFourRegRTC_FREQCORR(opts, RTC->MODE1.FREQCORR);

    opts.print.print("PER:  ");
    opts.print.print(RTC->MODE1.PER.bit.PER);
    PRINTNL();

    while (RTC->MODE1.SYNCBUSY.vec.COMP);
    for (id = 0; id < 4; id++) {
        opts.print.print("COMP");
        opts.print.print(id);
        opts.print.print(":  ");
        opts.print.print(RTC->MODE1.COMP[id].bit.COMP);
        PRINTNL();
    }

    while (RTC->MODE1.SYNCBUSY.vec.GP);
    printFourRegRTC_GP(opts, RTC->MODE1.GP);
    printFourRegRTC_TAMPCTRL(opts, RTC->MODE1.TAMPCTRL);

    opts.print.print("TIMESTAMP:  ");
    opts.print.print(RTC->MODE1.TIMESTAMP.bit.COUNT);
    PRINTNL();

    printFourRegRTC_BKUP(opts, RTC->MODE1.BKUP);
}

void printFourRegRTC_MODE2(FourRegOptions &opts) {
    uint8_t id;
    opts.print.println("--------------------------- RTC CLOCK/CALENDAR");

    while (RTC->MODE2.SYNCBUSY.bit.ENABLE) {}
    opts.print.print("CTRLA: ");
    PRINTFLAG(RTC->MODE2.CTRLA, ENABLE);
    opts.print.print(" MODE=");
    PRINTHEX(RTC->MODE2.CTRLA.bit.MODE);
    PRINTFLAG(RTC->MODE2.CTRLA, MATCHCLR);
    opts.print.print(" PRESCALER=");
    PRINTHEX(RTC->MODE2.CTRLA.bit.PRESCALER);
    PRINTFLAG(RTC->MODE2.CTRLA, BKTRST);
    PRINTFLAG(RTC->MODE2.CTRLA, GPTRST);
    PRINTFLAG(RTC->MODE2.CTRLA, CLOCKSYNC);
    PRINTNL();

    opts.print.print("CTRLB: ");
    PRINTFLAG(RTC->MODE2.CTRLB, GP0EN);
    PRINTFLAG(RTC->MODE2.CTRLB, GP2EN);
    PRINTFLAG(RTC->MODE2.CTRLB, DEBMAJ);
    PRINTFLAG(RTC->MODE2.CTRLB, DEBASYNC);
    PRINTFLAG(RTC->MODE2.CTRLB, RTCOUT);
    PRINTFLAG(RTC->MODE2.CTRLB, DMAEN);
    opts.print.print(" DEBF=");
    PRINTHEX(RTC->MODE2.CTRLB.bit.DEBF);
    opts.print.print(" ACTF=");
    PRINTHEX(RTC->MODE2.CTRLB.bit.ACTF);
    PRINTNL();

    opts.print.print("EVCTRL: ");
    for (id = 0; id < 8; id++) {
        if (RTC->MODE2.EVCTRL.vec.PEREO & (1<<id)) {
            opts.print.print(" PEREO");
            opts.print.print(id);
        }
    }
    PRINTFLAG(RTC->MODE2.EVCTRL, ALARMEO0);
    PRINTFLAG(RTC->MODE2.EVCTRL, ALARMEO1);
    PRINTFLAG(RTC->MODE2.EVCTRL, TAMPEREO);
    PRINTFLAG(RTC->MODE2.EVCTRL, OVFEO);
    PRINTFLAG(RTC->MODE2.EVCTRL, TAMPEVEI);
    PRINTNL();

    while (RTC->MODE2.SYNCBUSY.bit.FREQCORR);
    printFourRegRTC_FREQCORR(opts, RTC->MODE2.FREQCORR);

    for (id = 0; id < 2; id++) {
        while (RTC->MODE2.SYNCBUSY.vec.ALARM & (1<<id)) {}
        uint8_t mask;
        mask = RTC->MODE2.Mode2Alarm[id].MASK.bit.SEL;
        if ((mask == 0x0) && !opts.showDisabled) {
            continue;
        }
        opts.print.print("ALARM");
        opts.print.print(id);
        opts.print.print(":  ");
        RTC_MODE2_ALARM_Type alarm;
        alarm.reg = RTC->MODE2.Mode2Alarm[id].ALARM.reg;
        if (mask >= 0x6) { PRINTPAD2(alarm.bit.YEAR); }
        if (mask >= 0x5) { PRINTPAD2(alarm.bit.MONTH); }
        if (mask >= 0x4) { PRINTPAD2(alarm.bit.DAY); }
        if (mask >= 0x3) { PRINTPAD2(alarm.bit.HOUR); }
        if (mask >= 0x2) { PRINTPAD2(alarm.bit.MINUTE); }
        if (mask >= 0x1) { PRINTPAD2(alarm.bit.SECOND); }
        if (mask == 0x0) { opts.print.print(FourRegs__DISABLED); }
        PRINTNL();
    }

    while (RTC->MODE2.SYNCBUSY.vec.GP);
    printFourRegRTC_GP(opts, RTC->MODE2.GP);
    printFourRegRTC_TAMPCTRL(opts, RTC->MODE2.TAMPCTRL);
    printFourRegRTC_BKUP(opts, RTC->MODE2.BKUP);
}

void printFourRegRTC(FourRegOptions &opts) {
    if (!opts.showDisabled && !RTC->MODE0.CTRLA.bit.ENABLE) {
        return;
    }
    switch (RTC->MODE0.CTRLA.bit.MODE) {
        case 0x0: printFourRegRTC_MODE0(opts); break;
        case 0x1: printFourRegRTC_MODE1(opts); break;
        case 0x2: printFourRegRTC_MODE2(opts); break;
        default:  opts.print.print(FourRegs__UNKNOWN); break;
    }
}


void printFourRegPAC(FourRegOptions &opts) {
    // a little tricker to figure out if nothing interesting is happening
    if (!opts.showDisabled &&
            !PAC->STATUSA.reg && !PAC->STATUSB.reg &&
            !PAC->STATUSC.reg && !PAC->STATUSD.reg)
    {
        return;
    }
    opts.print.println("--------------------------- PAC");

    opts.print.print("EVCTRL: ");
    PRINTFLAG(PAC->EVCTRL, ERREO);
    PRINTNL();

    opts.print.print("STATUSA: ");
    if (PAC->STATUSA.bit.PAC_) { opts.print.print(" PAC"); }
    if (PAC->STATUSA.bit.PM_) { opts.print.print(" PM"); }
    if (PAC->STATUSA.bit.MCLK_) { opts.print.print(" MCLK"); }
    if (PAC->STATUSA.bit.RSTC_) { opts.print.print(" RSTC"); }
    if (PAC->STATUSA.bit.OSCCTRL_) { opts.print.print(" OSCCTRL"); }
    if (PAC->STATUSA.bit.OSC32KCTRL_) { opts.print.print(" OSC32KCTRL"); }
    if (PAC->STATUSA.bit.SUPC_) { opts.print.print(" SUPC"); }
    if (PAC->STATUSA.bit.GCLK_) { opts.print.print(" GCLK"); }
    if (PAC->STATUSA.bit.WDT_) { opts.print.print(" WDT"); }
    if (PAC->STATUSA.bit.RTC_) { opts.print.print(" RTC"); }
    if (PAC->STATUSA.bit.EIC_) { opts.print.print(" EIC"); }
    if (PAC->STATUSA.bit.FREQM_) { opts.print.print(" FREQM"); }
    if (PAC->STATUSA.bit.SERCOM0_) { opts.print.print(" SERCOM0"); }
    if (PAC->STATUSA.bit.SERCOM1_) { opts.print.print(" SERCOM1"); }
    if (PAC->STATUSA.bit.TC0_) { opts.print.print(" TC0"); }
    if (PAC->STATUSA.bit.TC1_) { opts.print.print(" TC1"); }
    PRINTNL();

    opts.print.print("STATUSB: ");
    if (PAC->STATUSB.bit.USB_) { opts.print.print(" USB"); }
    if (PAC->STATUSB.bit.DSU_) { opts.print.print(" DSU"); }
    if (PAC->STATUSB.bit.NVMCTRL_) { opts.print.print(" NVMCTRL"); }
    if (PAC->STATUSB.bit.CMCC_) { opts.print.print(" CMCC"); }
    if (PAC->STATUSB.bit.PORT_) { opts.print.print(" PORT"); }
    if (PAC->STATUSB.bit.DMAC_) { opts.print.print(" DMAC"); }
    if (PAC->STATUSB.bit.HMATRIX_) { opts.print.print(" HMATRIX"); }
    if (PAC->STATUSB.bit.EVSYS_) { opts.print.print(" EVSYS"); }
    if (PAC->STATUSB.bit.SERCOM2_) { opts.print.print(" SERCOM2"); }
    if (PAC->STATUSB.bit.SERCOM3_) { opts.print.print(" SERCOM3"); }
    if (PAC->STATUSB.bit.TCC0_) { opts.print.print(" TCC0"); }
    if (PAC->STATUSB.bit.TCC1_) { opts.print.print(" TCC1"); }
    if (PAC->STATUSB.bit.TC2_) { opts.print.print(" TC2"); }
    if (PAC->STATUSB.bit.TC3_) { opts.print.print(" TC3"); }
    if (PAC->STATUSB.bit.TAL_) { opts.print.print(" TAL"); }
    if (PAC->STATUSB.bit.RAMECC_) { opts.print.print(" RAMECC"); }
    PRINTNL();

    opts.print.print("STATUSC: ");
    if (PAC->STATUSC.bit.TCC2_) { opts.print.print(" TCC2"); }
    if (PAC->STATUSC.bit.TCC3_) { opts.print.print(" TCC3"); }
    if (PAC->STATUSC.bit.TC4_) { opts.print.print(" TC4"); }
    if (PAC->STATUSC.bit.TC5_) { opts.print.print(" TC5"); }
    if (PAC->STATUSC.bit.PDEC_) { opts.print.print(" PDEC"); }
    if (PAC->STATUSC.bit.AC_) { opts.print.print(" AC"); }
    if (PAC->STATUSC.bit.AES_) { opts.print.print(" AES"); }
    if (PAC->STATUSC.bit.TRNG_) { opts.print.print(" TRNG"); }
    if (PAC->STATUSC.bit.ICM_) { opts.print.print(" ICM"); }
    if (PAC->STATUSC.bit.PUKCC_) { opts.print.print(" PUKCC"); }
    if (PAC->STATUSC.bit.QSPI_) { opts.print.print(" QSPI"); }
    if (PAC->STATUSC.bit.CCL_) { opts.print.print(" CCL"); }
    PRINTNL();

    opts.print.print("STATUSD: ");
    if (PAC->STATUSD.bit.SERCOM4_) { opts.print.print(" SERCOM4"); }
    if (PAC->STATUSD.bit.SERCOM5_) { opts.print.print(" SERCOM5"); }
    if (PAC->STATUSD.bit.SERCOM6_) { opts.print.print(" SERCOM6"); }
    if (PAC->STATUSD.bit.SERCOM7_) { opts.print.print(" SERCOM7"); }
    if (PAC->STATUSD.bit.TCC4_) { opts.print.print(" TCC4"); }
    if (PAC->STATUSD.bit.TC6_) { opts.print.print(" TC6"); }
    if (PAC->STATUSD.bit.TC7_) { opts.print.print(" TC7"); }
    if (PAC->STATUSD.bit.ADC0_) { opts.print.print(" ADC0"); }
    if (PAC->STATUSD.bit.ADC1_) { opts.print.print(" ADC1"); }
    if (PAC->STATUSD.bit.DAC_) { opts.print.print(" DAC"); }
    if (PAC->STATUSD.bit.I2S_) { opts.print.print(" I2S"); }
    if (PAC->STATUSD.bit.PCC_) { opts.print.print(" PCC"); }
    PRINTNL();
}


void printFourRegPM_CFG(FourRegOptions &opts, uint8_t v) {
    switch(v) {
        case 0x0: opts.print.print("RET"); break;
        case 0x1: opts.print.print("PARTIAL"); break;
        case 0x2: opts.print.print("OFF"); break;
        default:  opts.print.print(FourRegs__UNKNOWN); break;
    }
}
void printFourRegPM(FourRegOptions &opts) {
    opts.print.println("--------------------------- PM");

    opts.print.print("CTRLA: ");
    PRINTFLAG(PM->CTRLA, IORET);
    PRINTNL();

    opts.print.print("HIBCFG: ");
    opts.print.print(" ramcfg=");
    printFourRegPM_CFG(opts, PM->HIBCFG.bit.RAMCFG);
    opts.print.print(" bramcfg=");
    printFourRegPM_CFG(opts, PM->HIBCFG.bit.BRAMCFG);
    PRINTNL();

    opts.print.print("STDBYCFG: ");
    opts.print.print(" ramcfg=");
    printFourRegPM_CFG(opts, PM->STDBYCFG.bit.RAMCFG);
    opts.print.print(" FASTWKUP=");
    PRINTHEX(PM->STDBYCFG.bit.FASTWKUP);
    PRINTNL();

    opts.print.print("BKUPCFG: ");
    opts.print.print(" bramcfg=");
    printFourRegPM_CFG(opts, PM->BKUPCFG.bit.BRAMCFG);
    PRINTNL();

    opts.print.print("PWSAKDLY: ");
    PRINTFLAG(PM->PWSAKDLY, IGNACK);
    opts.print.print(" DLYVAL=");
    PRINTHEX(PM->PWSAKDLY.bit.DLYVAL);
    PRINTNL();
}


// table 6-1 (datasheet rev E)
struct FourRegsPORT_PMUX {
    char        id;
    const char* name;
};
static const FourRegsPORT_PMUX FourRegsPORT_PMUXs[14] = {
    { 'A', "EIC" },
    { 'B', "ANAREF,ADC0,ADC1,AC,DAC,PTC" }, // analog stuff
    { 'C', "SERCOM" },
    { 'D', "SERCOM" },
    { 'E', "TC" },
    { 'F', "TCC" },
    { 'G', "TCC,PDEC" },
    { 'H', "QSPI,CAN1,USB,CORTEX_CM4" },
    { 'I', "SDHC,CAN0" },
    { 'J', "I2S" },
    { 'K', "PCC" },
    { 'L', "GMAC" },
    { 'M', "GCLK,AC" },
    { 'N', "CCL" },
};
struct FourRegsPORT_Pin {
    const char* name;
    const char* pmux[14];   // see PMUX names above
};
static const FourRegsPORT_Pin FourRegsPORT_pins[4][32] = {
    {
        { "PA00", {   "EIC:0",                      NULL,        NULL, "SERCOM1:0", "TC2:0",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PA01", {   "EIC:1",                      NULL,        NULL, "SERCOM1:1", "TC2:1",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PA02", {   "EIC:2",            "ADC0:0,DAC:0",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PA03", {   "EIC:3",      "VREFA,ADC0:1,X0/Y0",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PA04", {   "EIC:4", "VREFB,ADC0:4,AC:0,X3/Y3",        NULL, "SERCOM0:0", "TC0:0",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:IO" } },
        { "PA05", {   "EIC:5",       "ADC0:5,AC:1,DAC:1",        NULL, "SERCOM0:1", "TC0:1",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:I1" } },
        { "PA06", {   "EIC:6", "VREFC,ADC0:6,AC:2,X4/Y4",        NULL, "SERCOM0:2", "TC1:0",     NULL,        NULL,         NULL,   "SDHC0:SDCD",       NULL,        NULL,           NULL,     NULL,  "CCL:I2" } },
        { "PA07", {   "EIC:7",       "ADC0:7,AC:3,X5/Y5",        NULL, "SERCOM0:3", "TC1:1",     NULL,        NULL,         NULL,   "SDHC0:SDWP",       NULL,        NULL,           NULL,     NULL,  "CCL:O0" } },
        { "PA08", { "EIC:NMI",     "ADC0:8,ADC1:2,X6/Y6", "SERCOM0:0", "SERCOM2:1", "TC0:0", "TCC0:0",    "TCC1:4", "QSPI:DATA0",  "SDHC0:SDCMD", "I2S:MCK0",        NULL,           NULL,     NULL,  "CCL:I3" } },
        { "PA09", {   "EIC:9",     "ADC0:9,ADC1:3,X7/Y7", "SERCOM0:1", "SERCOM2:0", "TC0:1", "TCC0:1",    "TCC1:5", "QSPI:DATA1", "SDHC0:SDDAT0",  "I2S:FS0",        NULL,           NULL,     NULL,  "CCL:I4" } },
        { "PA10", {  "EIC:10",           "ADC0:10,X8/Y8", "SERCOM0:2", "SERCOM2:2", "TC1:0", "TCC0:2",    "TCC1:6", "QSPI:DATA2", "SDHC0:SDDAT1", "I2S:SCK0",        NULL,           NULL, "GCLK:4",  "CCL:I5" } },
        { "PA11", {  "EIC:11",           "ADC0:11,X9/Y9", "SERCOM0:3", "SERCOM2:3", "TC1:1", "TCC0:3",    "TCC1:7", "QSPI:DATA3", "SDHC0:SDDAT2",  "I2S:SDO",        NULL,           NULL, "GCLK:5",  "CCL:O1" } },
        { "PA12", {  "EIC:12",                      NULL, "SERCOM2:0", "SERCOM4:1", "TC2:0", "TCC0:6",    "TCC1:2",         NULL,   "SDHC0:SDCD",       NULL,  "PCC:DEN1",    "GMAC:GRX1",   "AC:0",      NULL } },
        { "PA13", {  "EIC:13",                      NULL, "SERCOM2:1", "SERCOM4:0", "TC2:1", "TCC0:7",    "TCC1:3",         NULL,   "SDCH0:SDWP",       NULL,  "PCC:DEN2",    "GMAC:GRX0",   "AC:1",      NULL } },
        { "PA14", {  "EIC:14",                      NULL, "SERCOM2:2", "SERCOM4:2", "TC3:0", "TCC2:0",    "TCC1:2",         NULL,           NULL,       NULL,   "PCC:CLK",   "GMAC:GTXCK", "GCLK:0",      NULL } },
        { "PA15", {  "EIC:15",                      NULL, "SERCOM2:3", "SERCOM4:3", "TC3:1", "TCC2:1",    "TCC1:3",         NULL,           NULL,       NULL,        NULL,   "GMAC:GRXER", "GCLK:1",      NULL } },
        { "PA16", {   "EIC:0",                 "X10/Y10", "SERCOM1:0", "SERCOM3:1", "TC2:0", "TCC1:0",    "TCC0:4",         NULL,           NULL,       NULL, "PCC:DATA0", "GMAC:GRXDIV6", "GCLK:2",  "CCL:I0" } },
        { "PA17", {   "EIC:1",                 "X11/Y11", "SERCOM1:1", "SERCOM3:0", "TC2:1", "TCC1:1",    "TCC0:5",         NULL,           NULL,       NULL, "PCC:DATA1",   "GMAC:GTXEN", "GCLK:3",  "CCL:I1" } },
        { "PA18", {   "EIC:2",                 "X12/Y12", "SERCOM1:2", "SERCOM3:2", "TC3:0", "TCC1:2",    "TCC0:6",         NULL,           NULL,       NULL, "PCC:DATA2",    "GMAC:GTX0",   "AC:0",  "CCL:I2" } },
        { "PA19", {   "EIC:3",                 "X13/Y13", "SERCOM1:3", "SERCOM3:3", "TC3:0", "TCC1:3",    "TCC0:7",         NULL,           NULL,       NULL, "PCC:DATA3",    "GMAC:GTX1",   "AC:1",  "CCL:O0" } },
        { "PA20", {   "EIC:4",                 "X14/Y14", "SERCOM5:2", "SERCOM3:2", "TC7:0", "TCC1:4",    "TCC0:0",         NULL,  "SDHC1:SDCMD",  "I2S:FS0", "PCC:DATA4",    "GMAC:GMDC",     NULL,      NULL } },
        { "PA21", {   "EIC:5",                 "X15/Y15", "SERCOM5:3", "SERCOM3:3", "TC7:1", "TCC1:5",    "TCC0:1",         NULL,   "SDHC1:SDCK",  "I2S:SDO", "PCC:DATA5",   "GMAC:GMDIO",     NULL,      NULL } },
        { "PA22", {   "EIC:6",                 "X16/Y16", "SERCOM3:0", "SERCOM5:1", "TC4:0", "TCC1:6",    "TCC0:2",         NULL,      "CAN0:TX",  "I2S:SDI", "PCC:DATA6",           NULL,     NULL,  "CCL:I6" } },
        { "PA23", {   "EIC:7",                 "X17/Y17", "SERCOM3:1", "SERCOM5:0", "TC4:1", "TCC1:7",    "TCC0:3",  "USB:SOF1K",      "CAN0:RX",  "I2S:FS1", "PCC:DATA7",           NULL,     NULL,  "CCL:I7" } },
        { "PA24", {   "EIC:8",                      NULL, "SERCOM3:2", "SERCOM5:2", "TC5:0", "TCC2:2", "PDEC:QDI0",     "USB:DM",      "CAN0:TX",       NULL,        NULL,           NULL,     NULL,  "CCL:I8" } },
        { "PA25", {   "EIC:9",                      NULL, "SERCOM3:3", "SERCOM5:3", "TC5:1",     NULL, "PDEC:QDI1",     "USB:DP",      "CAN0:RX",       NULL,        NULL,           NULL,     NULL,  "CCL:O2" } },
        {/*PA26*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PA27", {  "EIC:11",                 "X18/Y18",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL, "GCLK:1",      NULL } },
        {/*PA28*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        {/*PA29*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PA30", {  "EIC:14",                 "X19/Y19",        NULL, "SERCOM1:2", "TC6:0", "TCC2:0",        NULL, "CORTEX_CM4:SWCLK",     NULL,       NULL,        NULL,           NULL, "GCLK:0",  "CCL:I3" } },
        { "PA31", {  "EIC:15",                      NULL,        NULL, "SERCOM1:3", "TC6:1", "TCC2:1",        NULL, "CORTEX_CM4:SWDIO",     NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:O0" } },
    },
    {
        { "PB00", {   "EIC:0",         "ADC0:12,X30/Y30",        NULL, "SERCOM5:2", "TC7:0",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:I1" } },
        { "PB01", {   "EIC:1",         "ADC0:13,X31/Y31",        NULL, "SERCOM5:3", "TC7:1",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:I2" } },
        { "PB02", {   "EIC:2",         "ADCO:14,X20/Y20",        NULL, "SERCOM5:0", "TC6:0", "TCC2:2",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:O2" } },
        { "PB03", {   "EIC:3",         "ADC0:15,X21/Y21",        NULL, "SERCOM5:1", "TC6:1",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PB04", {   "EIC:4",          "ADC1:6,X22/Y22",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PB05", {   "EIC:5",          "ADC1:7,X23/Y23",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PB06", {   "EIC:6",          "ADC1:8,X24/Y24",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:I6" } },
        { "PB07", {   "EIC:7",          "ADC1:9,X25/Y25",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:I7" } },
        { "PB08", {   "EIC:8",     "ADC0:2,ADC1:0,X1/Y1",        NULL, "SERCOM4:0", "TC4:0",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:I8" } },
        { "PB09", {   "EIC:9",     "ADC0:3,ADC1:1,X2/Y2",        NULL, "SERCOM4:1", "TC4:1",     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:O2" } },
        { "PB10", {  "EIC:10",                      NULL,        NULL, "SERCOM4:2", "TC5:0", "TCC0:4",    "TCC1:0",   "QSPI:SCK", "SDHC0:SDDAT3",  "I2S:SDI",        NULL,           NULL, "GCLK:4", "CCL:I11" } },
        { "PB11", {  "EIC:11",                      NULL,        NULL, "SERCOM4:3", "TC5:1", "TCC0:5",    "TCC1:1",    "QSPI:CS",   "SDHC0:SDCK",  "I2S:FS1",        NULL,           NULL, "GCLK:5",  "CCL:O1" } },
        { "PB12", {  "EIC:12",                 "X26/Y26", "SERCOM4:0",        NULL, "TC4:0", "TCC3:0",    "TCC0:0",    "CAN1:TX",   "SDHC0:SDCD", "I2S:SCK1",        NULL,           NULL, "GCLK:6",      NULL } },
        { "PB13", {  "EIC:13",                 "X27/Y27", "SERCOM4:1",        NULL, "TC4:1", "TCC3:1",    "TCC0:1",    "CAN1:RX",   "SDHC0:SDWP", "I2S:MCK1",        NULL,           NULL, "GCLK:7",      NULL } },
        { "PB14", {  "EIC:14",                 "X28/Y28", "SERCOM4:2",        NULL, "TC5:0", "TCC4:0",    "TCC0:2",    "CAN1:TX",           NULL,       NULL, "PCC:DATA8",    "GMAC:GMDC", "GCLK:0",  "CCL:I9" } },
        { "PB15", {  "EIC:15",                 "X29/Y29", "SERCOM4:3",        NULL, "TC5:1", "TCC4:1",    "TCC0:3",    "CAN1:RX",           NULL,       NULL, "PCC:DATA9",   "GMAC:GMDIO", "GCLK:1", "CCL:I10" } },
        { "PB16", {   "EIC:0",                      NULL, "SERCOM5:0",        NULL, "TC6:0", "TCC3:0",    "TCC0:4",         NULL,   "SDHC1:SDCD", "I2S:SCK0",        NULL,           NULL, "GCLK:2", "CCL:I11" } },
        { "PB17", {   "EIC:1",                      NULL, "SERCOM5:1",        NULL, "TC6:1", "TCC3:1",    "TCC0:5",         NULL,   "SDHC1:SDWP", "I2S:MCK0",        NULL,           NULL, "GCLK:3",  "CCL:O3" } },
        { "PB18", {   "EIC:2",                      NULL, "SERCOM5:2", "SERCOM7:2",    NULL, "TCC1:0", "PDEC:QDI0",         NULL, "SDHC1:SDDAT0",       NULL,        NULL,           NULL, "GCLK:4",      NULL } },
        { "PB19", {   "EIC:3",                      NULL, "SERCOM5:3", "SERCOM7:3",    NULL, "TCC1:1", "PDEC:QDI1",         NULL, "SDHC1:SDDAT1",       NULL,        NULL,           NULL, "GCLK:5",      NULL } },
        { "PB20", {   "EIC:4",                      NULL, "SERCOM3:0", "SERCOM7:1",    NULL, "TCC1:2", "PDEC:QDI2",         NULL, "SDHC1:SDDAT2",       NULL,        NULL,           NULL, "GCLK:6",      NULL } },
        { "PB21", {   "EIC:5",                      NULL, "SERCOM3:1", "SERCOM7:0",    NULL, "TCC1:3",        NULL,         NULL, "SDHC1:SDDAT3",       NULL,        NULL,           NULL, "GCLK:7",      NULL } },
        { "PB22", {   "EIC:6",                      NULL, "SERCOM1:2", "SERCOM5:2", "TC7:0",     NULL, "PDEC:QDI2",  "USB:SOF1K",           NULL,       NULL,        NULL,           NULL, "GCLK:0",  "CCL:I0" } },
        { "PB23", {   "EIC:7",                      NULL, "SERCOM1:3", "SERCOM5:3", "TC7:1",     NULL, "PDEC:QDI0",         NULL,           NULL,       NULL,        NULL,           NULL, "GCLK:1",      NULL } },
        { "PB24", {   "EIC:8",                      NULL, "SERCOM0:0", "SERCOM2:1",    NULL,     NULL, "PDEC:QDI1",         NULL,           NULL,       NULL,        NULL,           NULL,  "AC:0:",      NULL } },
        { "PB25", {   "EIC:9",                      NULL, "SERCOM0:1", "SERCOM2:0",    NULL,     NULL, "PDEC:QDI2",         NULL,           NULL,       NULL,        NULL,           NULL,  "AC:0:",      NULL } },
        { "PB26", {  "EIC:12",                      NULL, "SERCOM2:0", "SERCOM4:1",    NULL, "TCC1:2",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PB27", {  "EIC:13",                      NULL, "SERCOM2:1", "SERCOM4:0",    NULL, "TCC1:3",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PB28", {  "EIC:14",                      NULL, "SERCOM2:2", "SERCOM4:2",    NULL, "TCC1:4",        NULL,         NULL,           NULL, "I2S:SCK1",        NULL,           NULL,     NULL,      NULL } },
        { "PB29", {  "EIC:15",                      NULL, "SERCOM2:3", "SERCOM4:3",    NULL, "TCC1:5",        NULL,         NULL,           NULL, "I2S:MCK1",        NULL,           NULL,     NULL,      NULL } },
        { "PB30", {  "EIC:14",                      NULL,        NULL, "SERCOM5:1", "TC0:0", "TCC4:0",    "TCC0:6", "CORTEX_CM4:SWO",       NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PB31", {  "EIC:15",                      NULL,        NULL, "SERCOM5:0", "TC0:1", "TCC4:1",    "TCC0:7",         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
    },
    {
        { "PC00", {   "EIC:0",                 "ADC1:10",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC01", {   "EIC:1",                 "ADC1:11",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC02", {   "EIC:2",                  "ADC1:4",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC03", {   "EIC:3",                  "ADC1:5",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC04", {   "EIC:4",                      NULL, "SERCOM6:0",        NULL,    NULL, "TCC0:0",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC05", {   "EIC:5",                      NULL, "SERCOM6:1",        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC06", {   "EIC:6",                      NULL, "SERCOM6:2",        NULL,    NULL,     NULL,        NULL,         NULL,   "SDHC0:SDCD",       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC07", {   "EIC:9",                      NULL, "SERCOM6:3",        NULL,    NULL,     NULL,        NULL,         NULL,   "SDHC0:SDWP",       NULL,        NULL,           NULL,     NULL,      NULL } },
        {/*PC08*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        {/*PC09*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC10", {  "EIC:10",                      NULL, "SERCOM6:2", "SERCOM7:2",    NULL, "TCC0:0",    "TCC1:4",         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC11", {  "EIC:11",                      NULL, "SERCOM6:3", "SERCOM7:3",    NULL, "TCC0:1",    "TCC1:5",         NULL,           NULL,       NULL,        NULL,    "GMAC:GMDC",     NULL,      NULL } },
        { "PC12", {  "EIC:12",                      NULL, "SERCOM7:0", "SERCOM6:1",    NULL, "TCC0:2",    "TCC1:6",         NULL,           NULL,       NULL, "PCC:DATA0",   "GMAC:GMDIO",     NULL,      NULL } },
        { "PC13", {  "EIC:13",                      NULL, "SERCOM7:1", "SERCOM6:0",    NULL, "TCC0:3",    "TCC1:7",         NULL,           NULL,       NULL, "PCC:DATA1",           NULL,     NULL,      NULL } },
        { "PC14", {  "EIC:14",                      NULL, "SERCOM7:2", "SERCOM6:2",    NULL, "TCC0:4",    "TCC1:0",         NULL,           NULL,       NULL, "PCC:DATA2",    "GMAC:GRX3",     NULL,      NULL } },
        { "PC15", {  "EIC:15",                      NULL, "SERCOM7:3", "SERCOM6:3",    NULL, "TCC0:5",    "TCC1:1",         NULL,           NULL,       NULL, "PCC:DATA3",    "GMAC:GRX2",     NULL,      NULL } },
        { "PC16", {   "EIC:0",                      NULL, "SERCOM6:0", "SERCOM0:1", "TC3:0", "TCC0:0", "PDEC:QDI0",         NULL,           NULL,       NULL,        NULL,    "GMAC:GTX2",     NULL,      NULL } },
        { "PC17", {   "EIC:1",                      NULL, "SERCOM6:1", "SERCOM0:0", "TC3:1", "TCC0:1", "PDEC:QDI1",         NULL,           NULL,       NULL,        NULL,    "GMAC:GTX3",     NULL,      NULL } },
        { "PC18", {   "EIC:2",                      NULL, "SERCOM6:2", "SERCOM0:2",    NULL, "TCC0:2", "PDEC:QDI2",         NULL,           NULL,       NULL,        NULL,   "GMAC:GRXCK",     NULL,      NULL } },
        { "PC19", {   "EIC:3",                      NULL, "SERCOM6:3", "SERCOM0:3",    NULL, "TCC0:3",        NULL,         NULL,           NULL,       NULL,        NULL,   "GMAC:GTXER",     NULL,      NULL } },
        { "PC20", {   "EIC:4",                      NULL,        NULL,        NULL,    NULL, "TCC0:4",        NULL,         NULL,   "SDHC1:SDCD",       NULL,        NULL,   "GMAC:GRXDV",     NULL,  "CCL:I9" } },
        { "PC21", {   "EIC:5",                      NULL,        NULL,        NULL,    NULL, "TCC0:5",        NULL,         NULL,   "SDHC1:SDWP",       NULL,        NULL,    "GMAC:GCOL",     NULL, "CCL:I10" } },
        { "PC22", {   "EIC:6",                      NULL, "SERCOM1:0", "SERCOM3:1",    NULL, "TCC0:6",        NULL,         NULL,           NULL,       NULL,        NULL,    "GMAC:GMDC",     NULL,      NULL } },
        { "PC23", {   "EIC:7",                      NULL, "SERCOM1:1", "SERCOM3:0",    NULL, "TCC0:7",        NULL,         NULL,           NULL,       NULL,        NULL,   "GMAC:GMDIO",     NULL,      NULL } },
        { "PC24", {   "EIC:8",                      NULL, "SERCOM0:2", "SERCOM2:2",    NULL,     NULL,        NULL, "CORTEX_CM4:DATA3",     NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC25", {   "EIC:9",                      NULL, "SERCOM0:3", "SERCOM2:3",    NULL,     NULL,        NULL, "CORTEX_CM4:DATA2",     NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC26", {  "EIC:10",                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL, "CORTEX_CM4:DATA1",     NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC27", {  "EIC:11",                      NULL, "SERCOM1:0",        NULL,    NULL,     NULL,        NULL, "CORTEX_CM4:CLK",       NULL,       NULL,        NULL,           NULL, "CORTEX_CM4:SWO", "CCL:I4" } },
        { "PC28", {  "EIC:12",                      NULL, "SERCOM1:1",        NULL,    NULL,     NULL,        NULL, "CORTEX_CM4:DATA0",     NULL,       NULL,        NULL,           NULL,     NULL,  "CCL:I5" } },
        {/*PC29*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC30", {  "EIC:14",                 "ADC1:12",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
        { "PC31", {  "EIC:15",                 "ADC1:13",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,      NULL } },
    },
    {
        { "PD00", {   "EIC:0",                 "ADC1:14",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD01", {   "EIC:1",                 "ADC1:15",        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD02*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD03*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD04*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD05*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD06*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD07*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD08", {   "EIC:3",                      NULL, "SERCOM7:0", "SERCOM6:1",    NULL, "TCC0:1",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD09", {   "EIC:4",                      NULL, "SERCOM7:1", "SERCOM6:0",    NULL, "TCC0:2",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD10", {   "EIC:5",                      NULL, "SERCOM7:2", "SERCOM6:2",    NULL, "TCC0:3",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD11", {   "EIC:6",                      NULL, "SERCOM7:3", "SERCOM6:3",    NULL, "TCC0:4",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD12", {   "EIC:7",                      NULL,        NULL,        NULL,    NULL, "TCC0:5",        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD13*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD14*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD15*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD16*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD17*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD18*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD19*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD20", {  "EIC:10",                      NULL, "SERCOM1:2", "SERCOM3:2",    NULL, "TCC1:0",        NULL,         NULL,   "SDHC1:SDCD",       NULL,        NULL,           NULL,     NULL,     NULL } },
        { "PD21", {  "EIC:11",                      NULL, "SERCOM1:3", "SERCOM3:3",    NULL, "TCC1:1",        NULL,         NULL,   "SDHC1:SDWP",       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD22*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD23*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD24*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD25*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD26*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD27*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD28*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD29*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD30*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
        {/*PD31*/NULL, { NULL,                      NULL,        NULL,        NULL,    NULL,     NULL,        NULL,         NULL,           NULL,       NULL,        NULL,           NULL,     NULL,     NULL } },
    },
};
void printFourRegPORT(FourRegOptions &opts) {
    for (uint8_t gid = 0; gid < 4; gid++) {
        opts.print.print("--------------------------- PORT");
        opts.print.print(char('A' + gid));
        PRINTNL();

        for (uint8_t pid = 0; pid < 32; pid++) {
            if (!FourRegsPORT_pins[gid][pid].name) {
                // pin not defined in datasheet (rev E)
                continue;
            }
            uint32_t dir = (PORT->Group[gid].DIR.bit.DIR & (1 << pid));
            uint8_t inen = PORT->Group[gid].PINCFG[pid].bit.INEN;
            uint8_t pullen = PORT->Group[gid].PINCFG[pid].bit.PULLEN;
            uint8_t pmuxen = PORT->Group[gid].PINCFG[pid].bit.PMUXEN;
            bool disabled = !dir && !inen && !pullen && !pmuxen;    // [32.6.3.4] Digital Functionality Disabled
            if (disabled && !opts.showDisabled) {
                continue;
            }
            opts.print.print(FourRegsPORT_pins[gid][pid].name);
            opts.print.print(":  ");
            if (pmuxen) {
                uint8_t pmux;
                if ((pid%2) == 0) {
                    pmux = PORT->Group[gid].PMUX[pid/2].bit.PMUXE;
                } else {
                    pmux = PORT->Group[gid].PMUX[pid/2].bit.PMUXO;
                }
                opts.print.print("pmux ");
                const char *pmuxName = FourRegsPORT_pins[gid][pid].pmux[pmux];
                if (!pmuxName) {
                    pmuxName = FourRegsPORT_PMUXs[pmux].name;
                }
                opts.print.println(pmuxName);
                continue;
            }
            if (dir) {
                opts.print.print("output");
                if (inen) {
                    opts.print.print(" INEN");
                }
                PRINTFLAG(PORT->Group[gid].PINCFG[pid], DRVSTR);
            } else {
                opts.print.print("input");
                if (inen) {
                    opts.print.print(" INEN");
                }
                if (PORT->Group[gid].CTRL.bit.SAMPLING & (1<<pid)) {
                    opts.print.print(" SAMPLING");
                }
                if (pullen) {
                    opts.print.print(" pull=");
                    opts.print.print(
                            PORT->Group[gid].OUT.bit.OUT & (1<<pid) ?
                            "UP" : "DOWN"
                            );
                }
            }
            PRINTNL();
        }

        opts.print.print("EVCTRL: ");
        if (PORT->Group[gid].EVCTRL.bit.PORTEI0 || opts.showDisabled) {
            opts.print.print(" evt0:");
            if (PORT->Group[gid].EVCTRL.bit.PORTEI0) {
                opts.print.print("PORTEI0,");
            }
            switch (PORT->Group[gid].EVCTRL.bit.EVACT0) {
                case 0x0: opts.print.print("OUT,"); break;
                case 0x1: opts.print.print("SET,"); break;
                case 0x2: opts.print.print("CLR,"); break;
                case 0x3: opts.print.print("TGL,"); break;
            }
            opts.print.print(PORT->Group[gid].EVCTRL.bit.PID0);
            opts.print.print(" evt1=");
            if (PORT->Group[gid].EVCTRL.bit.PORTEI1) {
                opts.print.print("PORTEI1,");
            }
            switch (PORT->Group[gid].EVCTRL.bit.EVACT1) {
                case 0x0: opts.print.print("OUT,"); break;
                case 0x1: opts.print.print("SET,"); break;
                case 0x2: opts.print.print("CLR,"); break;
                case 0x3: opts.print.print("TGL,"); break;
            }
            opts.print.print(PORT->Group[gid].EVCTRL.bit.PID1);
            opts.print.print(" evt2=");
            if (PORT->Group[gid].EVCTRL.bit.PORTEI2) {
                opts.print.print("PORTEI2,");
            }
            switch (PORT->Group[gid].EVCTRL.bit.EVACT2) {
                case 0x0: opts.print.print("OUT,"); break;
                case 0x1: opts.print.print("SET,"); break;
                case 0x2: opts.print.print("CLR,"); break;
                case 0x3: opts.print.print("TGL,"); break;
            }
            opts.print.print(PORT->Group[gid].EVCTRL.bit.PID2);
            opts.print.print(" evt3=");
            if (PORT->Group[gid].EVCTRL.bit.PORTEI3) {
                opts.print.print("PORTEI3,");
            }
            switch (PORT->Group[gid].EVCTRL.bit.EVACT3) {
                case 0x0: opts.print.print("OUT,"); break;
                case 0x1: opts.print.print("SET,"); break;
                case 0x2: opts.print.print("CLR,"); break;
                case 0x3: opts.print.print("TGL,"); break;
            }
            opts.print.print(PORT->Group[gid].EVCTRL.bit.PID3);
        }
        PRINTNL();
    }
    //FUTURE -- walk Arduino g_APinDescription (see WVariant.h)
}


void printFourRegSERCOM_I2CM(FourRegOptions &opts, SercomI2cm &i2cm) {
    opts.print.print("CTRLA: ");
    PRINTFLAG(i2cm.CTRLA, ENABLE);
    PRINTFLAG(i2cm.CTRLA, RUNSTDBY);
    PRINTFLAG(i2cm.CTRLA, PINOUT);
    opts.print.print(" SDAHOLD=");
    PRINTHEX(i2cm.CTRLA.bit.SDAHOLD);
    PRINTFLAG(i2cm.CTRLA, MEXTTOEN);
    PRINTFLAG(i2cm.CTRLA, SEXTTOEN);
    opts.print.print(" SPEED=");
    PRINTHEX(i2cm.CTRLA.bit.SPEED);
    PRINTFLAG(i2cm.CTRLA, SCLSM);
    opts.print.print(" INACTOUT=");
    PRINTHEX(i2cm.CTRLA.bit.INACTOUT);
    PRINTFLAG(i2cm.CTRLA, LOWTOUTEN);
    PRINTNL();

    opts.print.print("CTRLB: ");
    PRINTFLAG(i2cm.CTRLB, SMEN);
    PRINTFLAG(i2cm.CTRLB, QCEN);
    opts.print.print(" ackact=");
    opts.print.print(i2cm.CTRLB.bit.ACKACT ? "NACK" : "ACK");
    PRINTNL();

    opts.print.print("CTRLC: ");
    PRINTFLAG(i2cm.CTRLC, DATA32B);
    PRINTNL();

    opts.print.print("BAUD: ");
    opts.print.print(" BAUD=");
    PRINTHEX(i2cm.BAUD.bit.BAUD);
    opts.print.print(" BAUDLOW=");
    PRINTHEX(i2cm.BAUD.bit.BAUDLOW);
    opts.print.print(" HSBAUD=");
    PRINTHEX(i2cm.BAUD.bit.HSBAUD);
    opts.print.print(" HSBAUDLOW=");
    PRINTHEX(i2cm.BAUD.bit.HSBAUDLOW);
    PRINTNL();
}

void printFourRegSERCOM_I2CS(FourRegOptions &opts, SercomI2cs &i2cs) {
    opts.print.print("CTRLA: ");
    PRINTFLAG(i2cs.CTRLA, ENABLE);
    PRINTFLAG(i2cs.CTRLA, RUNSTDBY);
    PRINTFLAG(i2cs.CTRLA, PINOUT);
    opts.print.print(" SDAHOLD=");
    PRINTHEX(i2cs.CTRLA.bit.SDAHOLD);
    PRINTFLAG(i2cs.CTRLA, SEXTTOEN);
    opts.print.print(" SPEED=");
    PRINTHEX(i2cs.CTRLA.bit.SPEED);
    PRINTFLAG(i2cs.CTRLA, SCLSM);
    PRINTFLAG(i2cs.CTRLA, LOWTOUTEN);
    PRINTNL();

    opts.print.print("CTRLB: ");
    PRINTFLAG(i2cs.CTRLB, SMEN);
    PRINTFLAG(i2cs.CTRLB, GCMD);
    PRINTFLAG(i2cs.CTRLB, AACKEN);
    opts.print.print(" AMODE=");
    PRINTHEX(i2cs.CTRLB.bit.AMODE);
    opts.print.print(" ackact=");
    opts.print.print(i2cs.CTRLB.bit.ACKACT ? "NACK" : "ACK");
    PRINTNL();

    opts.print.print("CTRLC: ");
    opts.print.print(" SDASETUP=");
    PRINTHEX(i2cs.CTRLC.bit.SDASETUP);
    PRINTFLAG(i2cs.CTRLC, DATA32B);
    PRINTNL();
}

void printFourRegSERCOM_SPI(FourRegOptions &opts, SercomSpi &spi, bool master) {
    opts.print.print("CTRLA: ");
    PRINTFLAG(spi.CTRLA, ENABLE);
    PRINTFLAG(spi.CTRLA, RUNSTDBY);
    PRINTFLAG(spi.CTRLA, IBON);
    switch (spi.CTRLA.bit.DOPO) {
        case 0x0:
            opts.print.print(master ? " mosi=" : " miso=");
            opts.print.print("PAD0 sck=PAD1");
            if (!master || spi.CTRLB.bit.MSSEN) {
                opts.print.print(" ss=PAD2");
            }
            break;
        case 0x2:
            opts.print.print(master ? " mosi=" : " miso=");
            opts.print.print("PAD3 sck=PAD1");
            if (!master || spi.CTRLB.bit.MSSEN) {
                opts.print.print(" ss=PAD2");
            }
            break;
        default:
            opts.print.print(" DOPO=");
            opts.print.print(FourRegs__RESERVED);
            break;
    }
    opts.print.print(master ? " miso=" : " mosi=");
    switch (spi.CTRLA.bit.DIPO) {
        case 0x0: opts.print.print("PAD0"); break;
        case 0x1: opts.print.print("PAD1"); break;
        case 0x2: opts.print.print("PAD2"); break;
        case 0x3: opts.print.print("PAD3"); break;
    }
    opts.print.print(" form=");
    switch (spi.CTRLA.bit.FORM) {
        case 0x0: opts.print.print("SPI"); break;
        case 0x2: opts.print.print("SPI_ADDR"); break;
        default:  opts.print.print(FourRegs__RESERVED); break;
    }
    opts.print.print(" cpha=");
    opts.print.print(spi.CTRLA.bit.CPHA ? "TRAILING" : "LEADING");
    opts.print.print(" cpol=");
    opts.print.print(spi.CTRLA.bit.CPOL ? "HIGH" : "LOW");
    opts.print.print(" dord=");
    opts.print.print(spi.CTRLA.bit.DORD ? "LSB" : "MSB");
    PRINTNL();

    opts.print.print("CTRLB: ");
    opts.print.print(" chsize=");
    switch (spi.CTRLB.bit.CHSIZE) {
        case 0x0: opts.print.print("8bit"); break;
        case 0x1: opts.print.print("9bit"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    PRINTFLAG(spi.CTRLB, PLOADEN);
    PRINTFLAG(spi.CTRLB, SSDE);
    PRINTFLAG(spi.CTRLB, MSSEN);
    opts.print.print(" amode=");
    switch (spi.CTRLB.bit.AMODE) {
        case 0x0: opts.print.print("MASK"); break;
        case 0x1: opts.print.print("2ADDRS"); break;
        case 0x2: opts.print.print("RANGE"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    PRINTFLAG(spi.CTRLB, RXEN);
    PRINTNL();

    opts.print.print("CTRLC: ");
    opts.print.print(" ICSPACE=");
    opts.print.print(spi.CTRLC.bit.ICSPACE);
    PRINTFLAG(spi.CTRLC, DATA32B);
    PRINTNL();

    opts.print.print("BAUD:  ");
    PRINTHEX(spi.BAUD.bit.BAUD);
    PRINTNL();
}

void printFourRegSERCOM_USART(FourRegOptions &opts, SercomUsart &usart) {
    opts.print.print("CTRLA: ");
    PRINTFLAG(usart.CTRLA, ENABLE);
    PRINTFLAG(usart.CTRLA, RUNSTDBY);
    PRINTFLAG(usart.CTRLA, IBON);
    PRINTFLAG(usart.CTRLA, TXINV);
    PRINTFLAG(usart.CTRLA, RXINV);
    opts.print.print(" sampr=");
    bool arithbaud = true;
    switch (usart.CTRLA.bit.SAMPR) {
        case 0x0: arithbaud = true; opts.print.print("16x"); break;
        case 0x1: arithbaud = false; opts.print.print("16x"); break;
        case 0x2: arithbaud = true; opts.print.print("8x"); break;
        case 0x3: arithbaud = false; opts.print.print("8x"); break;
        case 0x4: arithbaud = true; opts.print.print("3x"); break;
        default: opts.print.print(FourRegs__RESERVED); break;
    }
    switch (usart.CTRLA.bit.TXPO) {
        case 0x0: opts.print.print(" tx=PAD0 xck=PAD1"); break;
        case 0x1: opts.print.print(FourRegs__RESERVED); break;
        case 0x2: opts.print.print(" tx=PAD0 rts=PAD2 cts=PAD3"); break;
        case 0x3: opts.print.print(" tx=PAD0 xck=PAD1 rts=PAD2"); break;
    }
    switch (usart.CTRLA.bit.RXPO) {
        case 0x0: opts.print.print(" rx=PAD0"); break;
        case 0x1: opts.print.print(" rx=PAD1"); break;
        case 0x2: opts.print.print(" rx=PAD2"); break;
        case 0x3: opts.print.print(" rx=PAD3"); break;
    }
    opts.print.print(" SAMPA=");
    PRINTHEX(usart.CTRLA.bit.SAMPA);
    opts.print.print(" FORM=");
    PRINTHEX(usart.CTRLA.bit.FORM);
    opts.print.print(" cmode=");
    opts.print.print(usart.CTRLA.bit.CMODE ? "SYNC" : "ASYNC");
    opts.print.print(" CPOL=");
    opts.print.print(usart.CTRLA.bit.CPOL);
    opts.print.print(" dord=");
    opts.print.print(usart.CTRLA.bit.CPOL ? "LSB" : "MSB");
    PRINTNL();

    while (usart.SYNCBUSY.bit.CTRLB) {}
    opts.print.print("CTRLB:  chsize=");
    switch (usart.CTRLB.bit.CHSIZE) {
        case 0x0: opts.print.print("8bit"); break;
        case 0x1: opts.print.print("9bit"); break;
        case 0x2: opts.print.print(FourRegs__RESERVED); break;
        case 0x3: opts.print.print(FourRegs__RESERVED); break;
        case 0x4: opts.print.print(FourRegs__RESERVED); break;
        case 0x5: opts.print.print("5bit"); break;
        case 0x6: opts.print.print("6bit"); break;
        case 0x7: opts.print.print("7bit"); break;
    }
    PRINTFLAG(usart.CTRLB, SBMODE);
    PRINTFLAG(usart.CTRLB, COLDEN);
    PRINTFLAG(usart.CTRLB, SFDE);
    PRINTFLAG(usart.CTRLB, ENC);
    PRINTFLAG(usart.CTRLB, PMODE);
    PRINTFLAG(usart.CTRLB, TXEN);
    PRINTFLAG(usart.CTRLB, RXEN);
    opts.print.print(" LINCMD=");
    PRINTHEX(usart.CTRLB.bit.LINCMD);
    PRINTNL();

    opts.print.print("CTRLC: ");
    switch (usart.CTRLA.bit.FORM) {
        case 0x0:
        case 0x1:
        case 0x7:
            opts.print.print(" GTIME=");
            PRINTHEX(usart.CTRLC.bit.GTIME);
            break;
        case 0x2:
            opts.print.print(" BRKLEN=");
            PRINTHEX(usart.CTRLC.bit.BRKLEN);
            opts.print.print(" HDRDLY=");
            PRINTHEX(usart.CTRLC.bit.HDRDLY);
            PRINTFLAG(usart.CTRLC, INACK);
            if (usart.CTRLC.bit.INACK == 0) {
                PRINTFLAG(usart.CTRLC, DSNACK);
            }
            if (usart.CTRLA.bit.CMODE == 0) {
                opts.print.print(" MAXITER=");
                PRINTHEX(usart.CTRLC.bit.MAXITER);
            }
            break;
    }
    opts.print.print(" DATA32B=");
    PRINTHEX(usart.CTRLC.bit.DATA32B);
    PRINTNL();

    opts.print.print("BAUD:  ");
    if (arithbaud) {
        opts.print.print(usart.BAUD.bit.BAUD);
    } else {
        opts.print.print(usart.BAUD.FRAC.BAUD);
        opts.print.print(".");
        opts.print.print(usart.BAUD.FRAC.FP);
    }
    PRINTNL();

    if (usart.CTRLB.bit.ENC) {
        opts.print.print("RXPL:  ");
        PRINTHEX(usart.RXPL.bit.RXPL);
        PRINTNL();
    }
}

void printFourRegSERCOM(FourRegOptions &opts, Sercom* sercom, uint8_t idx) {
    while (sercom->I2CM.SYNCBUSY.bit.ENABLE) {}
    if (! sercom->I2CM.CTRLA.bit.ENABLE) {
        if (opts.showDisabled) {
            opts.print.print("--------------------------- SERCOM");
            opts.print.print(idx);
            PRINTNL();
            opts.print.println(FourRegs__DISABLED);
        }
        return;
    }
    opts.print.print("--------------------------- SERCOM");
    opts.print.print(idx);
    switch (sercom->I2CM.CTRLA.bit.MODE) {
        case 0x0:
            opts.print.println(" USART (external clock)");
            printFourRegSERCOM_USART(opts, sercom->USART);
            break;
        case 0x1:
            opts.print.println(" USART (internal clock)");
            printFourRegSERCOM_USART(opts, sercom->USART);
            break;
        case 0x2:
            opts.print.println(" SPI slave");
            printFourRegSERCOM_SPI(opts, sercom->SPI, false);
            break;
        case 0x3:
            opts.print.println(" SPI master");
            printFourRegSERCOM_SPI(opts, sercom->SPI, true);
            break;
        case 0x4:
            opts.print.println(" I2C slave");
            printFourRegSERCOM_I2CS(opts, sercom->I2CS);
            break;
        case 0x5:
            opts.print.println(" I2C master");
            printFourRegSERCOM_I2CM(opts, sercom->I2CM);
            break;
        default:
            opts.print.print(" ");
            opts.print.println(FourRegs__RESERVED);
            break;
    }
}


void printFourRegSUPC(FourRegOptions &opts) {
    opts.print.println("--------------------------- SUPC");

    if (opts.showDisabled || SUPC->BOD33.bit.ENABLE) {
        opts.print.print("BOD33: ");
        PRINTFLAG(SUPC->BOD33, ENABLE);
        opts.print.print(" ACTION=");
        PRINTHEX(SUPC->BOD33.bit.ACTION);
        PRINTFLAG(SUPC->BOD33, STDBYCFG);
        PRINTFLAG(SUPC->BOD33, RUNSTDBY);
        PRINTFLAG(SUPC->BOD33, RUNHIB);
        PRINTFLAG(SUPC->BOD33, RUNBKUP);
        opts.print.print(" HYST=");
        PRINTHEX(SUPC->BOD33.bit.HYST);
        opts.print.print(" PSEL=");
        PRINTHEX(SUPC->BOD33.bit.PSEL);
        opts.print.print(" LEVEL=");
        PRINTHEX(SUPC->BOD33.bit.LEVEL);
        opts.print.print(" VBATLEVEL=");
        PRINTHEX(SUPC->BOD33.bit.VBATLEVEL);
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.print("BOD33:  ");
            opts.print.println(FourRegs__DISABLED);
        }
    }

    if (opts.showDisabled || SUPC->BOD12.bit.ENABLE) {
        opts.print.print("BOD12: ");
        PRINTFLAG(SUPC->BOD12, ENABLE);
        PRINTFLAG(SUPC->BOD12, HYST);
        opts.print.print(" ACTION=");
        PRINTHEX(SUPC->BOD12.bit.ACTION);
        PRINTFLAG(SUPC->BOD12, STDBYCFG);
        PRINTFLAG(SUPC->BOD12, RUNSTDBY);
        PRINTFLAG(SUPC->BOD12, ACTCFG);
        opts.print.print(" PSEL=");
        PRINTHEX(SUPC->BOD12.bit.PSEL);
        opts.print.print(" LEVEL=");
        PRINTHEX(SUPC->BOD12.bit.LEVEL);
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.print("BOD12:  ");
            opts.print.println(FourRegs__DISABLED);
        }
    }

    if (opts.showDisabled || SUPC->VREG.bit.ENABLE) {
        opts.print.print("VREG: ");
        PRINTFLAG(SUPC->VREG, ENABLE);
        PRINTFLAG(SUPC->VREG, SEL);
        PRINTFLAG(SUPC->VREG, RUNBKUP);
        PRINTFLAG(SUPC->VREG, VSEN);
        opts.print.print(" VSPER=");
        PRINTHEX(SUPC->VREG.bit.VSPER);
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.print("VREG:  ");
            opts.print.println(FourRegs__DISABLED);
        }
    }

    opts.print.print("VREF: ");
    PRINTFLAG(SUPC->VREF, TSEN);
    PRINTFLAG(SUPC->VREF, VREFOE);
    PRINTFLAG(SUPC->VREF, TSSEL);
    PRINTFLAG(SUPC->VREF, RUNSTDBY);
    PRINTFLAG(SUPC->VREF, ONDEMAND);
    opts.print.print(" SEL=");
    PRINTHEX(SUPC->VREF.bit.SEL);
    PRINTNL();

    opts.print.print("BBPS: ");
    PRINTFLAG(SUPC->BBPS, CONF);
    PRINTFLAG(SUPC->BBPS, WAKEEN);
    PRINTNL();

    if (opts.showDisabled || SUPC->BKOUT.bit.EN) {
        opts.print.print("BKOUT: ");
        if (SUPC->BKOUT.bit.EN & 0x1) {
            opts.print.print(" 1=EN");
            if (SUPC->BKOUT.bit.RTCTGL & 0x1) {
                opts.print.print(",RTC");
            }
        }
        if (SUPC->BKOUT.bit.EN & 0x2) {
            opts.print.print(" 2=EN");
            if (SUPC->BKOUT.bit.RTCTGL & 0x2) {
                opts.print.print(",RTC");
            }
        }
        PRINTNL();
    } else {
        if (opts.showDisabled) {
            opts.print.print("BKOUT:  ");
            opts.print.println(FourRegs__DISABLED);
        }
    }

    opts.print.print("BKIN:  ");
    PRINTHEX(SUPC->BKIN.bit.BKIN);
    PRINTNL();
}


void printFourReg_TC_TCC_PRESCSYNC(FourRegOptions &opts, uint8_t prescsync) {
    opts.print.print(" prescsync=");
    switch(prescsync) {
        case 0x0: opts.print.print("GCLK"); break;
        case 0x1: opts.print.print("PRESC"); break;
        case 0x2: opts.print.print("RESYNC"); break;
        default:  opts.print.print(FourRegs__UNKNOWN); break;
    }
}


void printFourRegTC_CTRLA(FourRegOptions &opts, volatile TC_CTRLA_Type& ctrla) {
    opts.print.print("CTRLA: ");
    PRINTFLAG(ctrla, ENABLE);
    PRINTFLAG(ctrla, RUNSTDBY);
    PRINTFLAG(ctrla, ONDEMAND);
    printFourReg_TC_TCC_PRESCSYNC(opts, ctrla.bit.PRESCSYNC);
    PRINTFLAG(ctrla, ALOCK);
    PRINTFLAG(ctrla, CAPTEN0);
    PRINTFLAG(ctrla, CAPTEN1);
    PRINTFLAG(ctrla, COPEN0);
    PRINTFLAG(ctrla, COPEN1);
    opts.print.print(" CAPTMODE0=");
    PRINTHEX(ctrla.bit.CAPTMODE0);
    opts.print.print(" CAPTMODE1=");
    PRINTHEX(ctrla.bit.CAPTMODE1);
    PRINTNL();
}
void printFourRegTC_CTRLB(FourRegOptions &opts, volatile TC_CTRLBSET_Type& ctrlb) {
    opts.print.print("CTRLB:  dir=");
    opts.print.print(ctrlb.bit.DIR ? "UP": "DOWN");
    PRINTFLAG(ctrlb, LUPD);
    PRINTFLAG(ctrlb, ONESHOT);
    opts.print.print(" cmd=");
    switch(ctrlb.bit.CMD) {
        case 0x0: opts.print.print("NONE"); break;
        case 0x1: opts.print.print("RETRIGGER"); break;
        case 0x2: opts.print.print("STOP"); break;
        case 0x3: opts.print.print("UPDATE"); break;
        case 0x4: opts.print.print("READSYNC"); break;
        default:  opts.print.print(FourRegs__UNKNOWN); break;
    }
    PRINTNL();
}
void printFourRegTC_EVCTRL(FourRegOptions &opts, volatile TC_EVCTRL_Type& evctrl) {
    opts.print.print("EVCTRL:  evact=");
    switch(evctrl.bit.EVACT) {
        case 0x0: opts.print.print("OFF"); break;
        case 0x1: opts.print.print("RETRIGGER"); break;
        case 0x2: opts.print.print("COUNT"); break;
        case 0x3: opts.print.print("START"); break;
        case 0x4: opts.print.print("STAMP"); break;
        case 0x5: opts.print.print("PPW"); break;
        case 0x6: opts.print.print("PWP"); break;
        case 0x7: opts.print.print("PW"); break;
    }
    PRINTFLAG(evctrl, TCINV);
    PRINTFLAG(evctrl, TCEI);
    PRINTFLAG(evctrl, OVFEO);
    PRINTFLAG(evctrl, MCEO0);
    PRINTFLAG(evctrl, MCEO1);
    PRINTNL();
}
void printFourRegTC_WAVE(FourRegOptions &opts, volatile TC_WAVE_Type& wave) {
    opts.print.print("WAVE:  WAVEGEN=");
    PRINTHEX(wave.bit.WAVEGEN);
    PRINTNL();
}
void printFourRegTC_DRVCTRL(FourRegOptions &opts, volatile TC_DRVCTRL_Type& drvctrl) {
    opts.print.print("DRVCTRL: ");
    PRINTFLAG(drvctrl, INVEN0);
    PRINTFLAG(drvctrl, INVEN1);
    PRINTNL();
}
void printFourRegTC_8(FourRegOptions &opts, TcCount8& tc, uint8_t idx) {
    opts.print.print("--------------------------- TC");
    opts.print.print(idx);
    opts.print.println(" COUNT8");
    printFourRegTC_CTRLA(opts, tc.CTRLA);
    while (tc.SYNCBUSY.bit.CTRLB) {}
    printFourRegTC_CTRLB(opts, tc.CTRLBSET);
    printFourRegTC_EVCTRL(opts, tc.EVCTRL);
    printFourRegTC_WAVE(opts, tc.WAVE);
    printFourRegTC_DRVCTRL(opts, tc.DRVCTRL);
    while (tc.SYNCBUSY.bit.PER) {}
    opts.print.print("PER:  ");
    opts.print.println(tc.PER.bit.PER);
    while (tc.SYNCBUSY.bit.CC0) {}
    opts.print.print("CC0:  ");
    opts.print.println(tc.CC[0].bit.CC);
    while (tc.SYNCBUSY.bit.CC1) {}
    opts.print.print("CC1:  ");
    opts.print.println(tc.CC[1].bit.CC);
}
void printFourRegTC_16(FourRegOptions &opts, TcCount16& tc, uint8_t idx) {
    opts.print.print("--------------------------- TC");
    opts.print.print(idx);
    opts.print.println(" COUNT16");
    printFourRegTC_CTRLA(opts, tc.CTRLA);
    while (tc.SYNCBUSY.bit.CTRLB) {}
    printFourRegTC_CTRLB(opts, tc.CTRLBSET);
    printFourRegTC_EVCTRL(opts, tc.EVCTRL);
    printFourRegTC_WAVE(opts, tc.WAVE);
    printFourRegTC_DRVCTRL(opts, tc.DRVCTRL);
    while (tc.SYNCBUSY.bit.CC0) {}
    opts.print.print("CC0:  ");
    opts.print.println(tc.CC[0].bit.CC);
    while (tc.SYNCBUSY.bit.CC1) {}
    opts.print.print("CC1:  ");
    opts.print.println(tc.CC[1].bit.CC);
}
void printFourRegTC_32(FourRegOptions &opts, TcCount32& tc, uint8_t idx) {
    opts.print.print("--------------------------- TC");
    opts.print.print(idx);
    opts.print.println(" COUNT32");
    printFourRegTC_CTRLA(opts, tc.CTRLA);
    while (tc.SYNCBUSY.bit.CTRLB) {}
    printFourRegTC_CTRLB(opts, tc.CTRLBSET);
    printFourRegTC_EVCTRL(opts, tc.EVCTRL);
    printFourRegTC_WAVE(opts, tc.WAVE);
    printFourRegTC_DRVCTRL(opts, tc.DRVCTRL);
    while (tc.SYNCBUSY.bit.CC0) {}
    opts.print.print("CC0:  ");
    opts.print.println(tc.CC[0].bit.CC);
    while (tc.SYNCBUSY.bit.CC1) {}
    opts.print.print("CC1:  ");
    opts.print.println(tc.CC[1].bit.CC);
}
void printFourRegTC(FourRegOptions &opts, Tc* tc, uint8_t idx) {
    while (tc->COUNT8.SYNCBUSY.bit.ENABLE) {}
    if (!opts.showDisabled && !tc->COUNT8.CTRLA.bit.ENABLE) {
        return;
    }
    while (tc->COUNT8.SYNCBUSY.bit.STATUS) {}
    if (tc->COUNT8.STATUS.bit.SLAVE) {
        opts.print.print("--------------------------- TC");
        opts.print.print(idx);
        opts.print.println(" --slave--");
        return;
    }
    switch (tc->COUNT8.CTRLA.bit.MODE) {
        // TODO -- do same header printing here as is done for SERCOM
        case 0x0: printFourRegTC_16(opts, tc->COUNT16, idx); break;
        case 0x1: printFourRegTC_8(opts, tc->COUNT8, idx); break;
        case 0x2: printFourRegTC_32(opts, tc->COUNT32, idx); break;
        default:
                  opts.print.print("--------------------------- TC");
                  opts.print.print(idx);
                  opts.print.print(" ");
                  opts.print.println(FourRegs__UNKNOWN);
                  break;
    }
}


typedef union {
    struct {
        uint32_t SRC:2;         /*!< bit:  0.. 1  Fault A Source                     */
        uint32_t :1;            /*!< bit:      2  Reserved                           */
        uint32_t KEEP:1;        /*!< bit:      3  Fault A Keeper                     */
        uint32_t QUAL:1;        /*!< bit:      4  Fault A Qualification              */
        uint32_t BLANK:2;       /*!< bit:  5.. 6  Fault A Blanking Mode              */
        uint32_t RESTART:1;     /*!< bit:      7  Fault A Restart                    */
        uint32_t HALT:2;        /*!< bit:  8.. 9  Fault A Halt Mode                  */
        uint32_t CHSEL:2;       /*!< bit: 10..11  Fault A Capture Channel            */
        uint32_t CAPTURE:3;     /*!< bit: 12..14  Fault A Capture Action             */
        uint32_t BLANKPRESC:1;  /*!< bit:     15  Fault A Blanking Prescaler         */
        uint32_t BLANKVAL:8;    /*!< bit: 16..23  Fault A Blanking Time              */
        uint32_t FILTERVAL:4;   /*!< bit: 24..27  Fault A Filter Value               */
        uint32_t :4;            /*!< bit: 28..31  Reserved                           */
    } bit;                      /*!< Structure used for bit  access                  */
    uint32_t reg;               /*!< Type      used for register access              */
} FourRegsTCC_FCTRL_Type;
void printFourRegTCC(FourRegOptions &opts, Tcc* tcc, uint8_t idx) {
    uint8_t id;
    while (tcc->SYNCBUSY.bit.ENABLE) {}
    if (!opts.showDisabled && !tcc->CTRLA.bit.ENABLE) {
        return;
    }
    opts.print.print("--------------------------- TCC");
    opts.print.println(idx);

    opts.print.print("CTRLA: ");
    PRINTFLAG(tcc->CTRLA, ENABLE);
    PRINTFLAG(tcc->CTRLA, RUNSTDBY);
    uint8_t dith = tcc->CTRLA.bit.RESOLUTION;
    opts.print.print(" resolution=");
    switch(dith) {
        case 0x0: opts.print.print("NONE"); break;
        case 0x1: opts.print.print("DITH4"); break;
        case 0x2: opts.print.print("DITH5"); break;
        case 0x3: opts.print.print("DITH6"); break;
    }
    opts.print.print(" PRESCALER=");
    PRINTHEX(tcc->CTRLA.bit.PRESCALER);
    printFourReg_TC_TCC_PRESCSYNC(opts, tcc->CTRLA.bit.PRESCSYNC);
    PRINTFLAG(tcc->CTRLA, ALOCK);
    PRINTFLAG(tcc->CTRLA, MSYNC);
    PRINTFLAG(tcc->CTRLA, DMAOS);
    PRINTFLAG(tcc->CTRLA, CPTEN0);
    PRINTFLAG(tcc->CTRLA, CPTEN1);
    PRINTFLAG(tcc->CTRLA, CPTEN2);
    PRINTFLAG(tcc->CTRLA, CPTEN3);
    PRINTFLAG(tcc->CTRLA, CPTEN4);
    PRINTFLAG(tcc->CTRLA, CPTEN5);
    PRINTNL();

    while (tcc->SYNCBUSY.bit.CTRLB) {}
    opts.print.print("CTRLB:  dir=");
    opts.print.print(tcc->CTRLBSET.bit.DIR ? "UP": "DOWN");
    PRINTFLAG(tcc->CTRLBSET, LUPD);
    PRINTFLAG(tcc->CTRLBSET, ONESHOT);
    opts.print.print(" IDXCMD=");
    PRINTHEX(tcc->CTRLBSET.bit.IDXCMD);
    opts.print.print(" cmd=");
    switch(tcc->CTRLBSET.bit.CMD) {
        case 0x0: opts.print.print("NONE"); break;
        case 0x1: opts.print.print("RETRIGGER"); break;
        case 0x2: opts.print.print("STOP"); break;
        case 0x3: opts.print.print("UPDATE"); break;
        case 0x4: opts.print.print("READSYNC"); break;
        case 0x5: opts.print.print("DMAOS"); break;
        default:  opts.print.print(FourRegs__UNKNOWN); break;
    }
    PRINTNL();

    for (id = 0; id < 2; id++) {
        FourRegsTCC_FCTRL_Type fctrl;
        if (id == 0) {
            opts.print.print("FCTRLA: ");
            COPYVOL(fctrl, tcc->FCTRLA);
        } else {
            opts.print.print("FCTRLB: ");
            COPYVOL(fctrl, tcc->FCTRLB);
        }
        opts.print.print(" SRC=");
        PRINTHEX(fctrl.bit.SRC);
        PRINTFLAG(fctrl, KEEP);
        PRINTFLAG(fctrl, QUAL);
        opts.print.print(" BLANK=");
        PRINTHEX(fctrl.bit.BLANK);
        PRINTFLAG(fctrl, RESTART);
        opts.print.print(" HALT=");
        PRINTHEX(fctrl.bit.HALT);
        opts.print.print(" chsel=CC");
        opts.print.print(fctrl.bit.CHSEL);
        opts.print.print(" CAPTURE=");
        PRINTHEX(fctrl.bit.CAPTURE);
        PRINTFLAG(fctrl, BLANKPRESC);
        opts.print.print(" BLANKVAL=");
        PRINTHEX(fctrl.bit.BLANKVAL);
        opts.print.print(" FILTERVAL=");
        PRINTHEX(fctrl.bit.FILTERVAL);
        PRINTNL();
    }

    opts.print.print("WEXCTRL: ");
    opts.print.print(" OTMX=");
    PRINTHEX(tcc->WEXCTRL.bit.OTMX);
    PRINTFLAG(tcc->WEXCTRL, DTIEN0);
    PRINTFLAG(tcc->WEXCTRL, DTIEN1);
    PRINTFLAG(tcc->WEXCTRL, DTIEN2);
    PRINTFLAG(tcc->WEXCTRL, DTIEN3);
    opts.print.print(" DTLS=");
    opts.print.print(tcc->WEXCTRL.bit.DTLS);
    opts.print.print(" DTHS=");
    opts.print.print(tcc->WEXCTRL.bit.DTHS);
    PRINTNL();

    opts.print.print("DRVCTRL: ");
    for (id = 0; id < 8; id++) {
        if (tcc->DRVCTRL.vec.NRE & (1<<id)) {
            opts.print.print(" NRE");
            opts.print.print(id);
        }
    }
    for (id = 0; id < 8; id++) {
        if (tcc->DRVCTRL.vec.NRV & (1<<id)) {
            opts.print.print(" NRV");
            opts.print.print(id);
        }
    }
    for (id = 0; id < 8; id++) {
        if (tcc->DRVCTRL.vec.INVEN & (1<<id)) {
            opts.print.print(" INVEN");
            opts.print.print(id);
        }
    }
    opts.print.print(" FILTERVAL0=");
    PRINTHEX(tcc->DRVCTRL.bit.FILTERVAL0);
    opts.print.print(" FILTERVAL1=");
    PRINTHEX(tcc->DRVCTRL.bit.FILTERVAL1);
    PRINTNL();

    opts.print.print("EVCTRL: ");
    opts.print.print(" EVACT0=");
    PRINTHEX(tcc->EVCTRL.bit.EVACT0);
    opts.print.print(" EVACT1=");
    PRINTHEX(tcc->EVCTRL.bit.EVACT1);
    PRINTFLAG(tcc->EVCTRL, OVFEO);
    PRINTFLAG(tcc->EVCTRL, TRGEO);
    PRINTFLAG(tcc->EVCTRL, CNTEO);
    PRINTFLAG(tcc->EVCTRL, TCINV0);
    PRINTFLAG(tcc->EVCTRL, TCINV1);
    PRINTFLAG(tcc->EVCTRL, TCEI0);
    PRINTFLAG(tcc->EVCTRL, TCEI1);
    PRINTFLAG(tcc->EVCTRL, MCEI0);
    PRINTFLAG(tcc->EVCTRL, MCEI1);
    PRINTFLAG(tcc->EVCTRL, MCEI2);
    PRINTFLAG(tcc->EVCTRL, MCEI3);
    PRINTFLAG(tcc->EVCTRL, MCEI4);
    PRINTFLAG(tcc->EVCTRL, MCEI5);
    PRINTFLAG(tcc->EVCTRL, MCEO0);
    PRINTFLAG(tcc->EVCTRL, MCEO1);
    PRINTFLAG(tcc->EVCTRL, MCEO2);
    PRINTFLAG(tcc->EVCTRL, MCEO3);
    PRINTFLAG(tcc->EVCTRL, MCEO4);
    PRINTFLAG(tcc->EVCTRL, MCEO5);
    PRINTNL();

    while (tcc->SYNCBUSY.bit.PATT) {}
    opts.print.print("PATT:  ");
    for (id = 0; id < 8; id++) {
        if (tcc->PATT.vec.PGE & (1<<id)) {
            opts.print.print((tcc->PATT.vec.PGV & (1<<id)) ? "1" : "0");
        } else {
            opts.print.print(".");
        }
    }
    PRINTNL();

    while (tcc->SYNCBUSY.bit.WAVE) {}
    opts.print.print("WAVE: ");
    opts.print.print(" WAVEGEN=");
    PRINTHEX(tcc->WAVE.bit.WAVEGEN);
    opts.print.print(" RAMP=");
    PRINTHEX(tcc->WAVE.bit.RAMP);
    PRINTFLAG(tcc->WAVE, CIPEREN);
    for (id = 0; id < 4; id++) {
        if (tcc->WAVE.vec.CICCEN & (1<<id)) {
            opts.print.print(" CICCEN");
            opts.print.print(id);
        }
    }
    for (id = 0; id < 6; id++) {
        opts.print.print(" POL");
        opts.print.print(id);
        opts.print.print("=");
        opts.print.print(tcc->WAVE.vec.POL & (1<<id));
    }
    for (id = 0; id < 4; id++) {
        if (tcc->WAVE.vec.SWAP & (1<<id)) {
            opts.print.print(" SWAP");
            opts.print.print(id);
        }
    }
    PRINTNL();

    while (tcc->SYNCBUSY.bit.PER) {}
    opts.print.print("PER:  ");
    switch(dith) {
        case 0x0:
            opts.print.print(tcc->PER.bit.PER);
            break;
        case 0x1:
            opts.print.print(tcc->PER.DITH4.PER);
            opts.print.print(" DITHER=");
            opts.print.print(tcc->PER.DITH4.DITHER);
            break;
        case 0x2:
            opts.print.print(tcc->PER.DITH5.PER);
            opts.print.print(" DITHER=");
            opts.print.print(tcc->PER.DITH5.DITHER);
            break;
        case 0x3:
            opts.print.print(tcc->PER.DITH6.PER);
            opts.print.print(" DITHER=");
            opts.print.print(tcc->PER.DITH6.DITHER);
            break;
    }
    PRINTNL();

    for (id = 0; id < 6; id++) {
        while (tcc->SYNCBUSY.vec.CC & (1<<id)) {}
        opts.print.print("CC");
        opts.print.print(id);
        opts.print.print(":  ");
        switch(dith) {
            case 0x0:
                opts.print.print(tcc->CC[id].bit.CC);
                break;
            case 0x1:
                opts.print.print(tcc->CC[id].DITH4.CC);
                opts.print.print(" DITHER=");
                opts.print.print(tcc->CC[id].DITH4.DITHER);
                break;
            case 0x2:
                opts.print.print(tcc->CC[id].DITH5.CC);
                opts.print.print(" DITHER=");
                opts.print.print(tcc->CC[id].DITH5.DITHER);
                break;
            case 0x3:
                opts.print.print(tcc->CC[id].DITH6.CC);
                opts.print.print(" DITHER=");
                opts.print.print(tcc->CC[id].DITH6.DITHER);
                break;
        }
        PRINTNL();
    }
}


void printFourRegWDT(FourRegOptions &opts) {
    WDT_CTRLA_Type ctrla;
    while (WDT->SYNCBUSY.reg) {}
    COPYVOL(ctrla, WDT->CTRLA);
    if (!opts.showDisabled && !ctrla.bit.ENABLE) {
        return;
    }
    opts.print.println("--------------------------- WDT");

    opts.print.print("CTRLA: ");
    PRINTFLAG(ctrla, ENABLE);
    PRINTFLAG(ctrla, WEN);
    PRINTFLAG(ctrla, ALWAYSON);

    opts.print.print("CONFIG:  WINDOW=");
    PRINTHEX(WDT->CONFIG.bit.WINDOW);
    opts.print.print("PER=");
    PRINTHEX(WDT->CONFIG.bit.PER);
    PRINTNL();

    opts.print.print("EWCTRL:  EWOFFSET=");
    PRINTHEX(WDT->EWCTRL.bit.EWOFFSET);
    PRINTNL();
}


void printFourRegs(FourRegOptions &opts) {
    // show clocks
    printFourRegGCLK(opts);
    printFourRegOSCCTRL(opts);
    printFourRegOSC32KCTRL(opts);
    printFourRegMCLK(opts);
    printFourRegRTC(opts);

    // show core peripherals
    printFourRegCMCC(opts);
    //FUTURE printFourRegDSU(opts);
    printFourRegPAC(opts);
    printFourRegPM(opts);
    printFourRegSUPC(opts);
    printFourRegWDT(opts);

    // show other peripherals
    printFourRegAC(opts);
    printFourRegADC(opts, ADC0, 0);
    printFourRegADC(opts, ADC1, 1);
    printFourRegAES(opts);
    //FUTURE printFourRegCAN(opts);
    printFourRegCCL(opts);
    //FUTURE printFourRegDAC(opts);
    //FUTURE printFourRegDMAC(opts);
    printFourRegEIC(opts);
    printFourRegEVSYS(opts);
    //FUTURE printFourRegFREQM(opts);
    //FUTURE printFourRegGMAC(opts);
    //FUTURE printFourRegI2S(opts);
    //FUTURE printFourRegICM(opts);
    printFourRegNVMCTRL(opts);
    //FUTURE printFourRegPCC(opts);
    //FUTURE printFourRegPDEC(opts);
    printFourRegPORT(opts);
    //FUTURE printFourRegQSPI(opts);
    printFourRegSERCOM(opts, SERCOM0, 0);
    printFourRegSERCOM(opts, SERCOM1, 1);
    printFourRegSERCOM(opts, SERCOM2, 2);
    printFourRegSERCOM(opts, SERCOM3, 3);
    printFourRegSERCOM(opts, SERCOM4, 4);
    printFourRegSERCOM(opts, SERCOM5, 5);
#ifdef SERCOM6
    printFourRegSERCOM(opts, SERCOM6, 6);
#endif
#ifdef SERCOM7
    printFourRegSERCOM(opts, SERCOM7, 7);
#endif
    printFourRegTCC(opts, TCC0, 0);
    printFourRegTCC(opts, TCC1, 1);
    printFourRegTCC(opts, TCC2, 2);
#ifdef TCC3
    printFourRegTCC(opts, TCC3, 3);
#endif
#ifdef TCC4
    printFourRegTCC(opts, TCC4, 4);
#endif
    printFourRegTC(opts, TC0, 0);
    printFourRegTC(opts, TC1, 1);
    printFourRegTC(opts, TC2, 2);
    printFourRegTC(opts, TC3, 3);
#ifdef TC4
    printFourRegTC(opts, TC4, 4);
#endif
#ifdef TC5
    printFourRegTC(opts, TC5, 5);
#endif
#ifdef TC6
    printFourRegTC(opts, TC6, 6);
#endif
#ifdef TC7
    printFourRegTC(opts, TC7, 7);
#endif
    //FUTURE printFourRegTRNG(opts);
    //FUTURE printFourRegUSB(opts);
}


