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


#define PRINTFLAG(x,y) do { if (x.bit.y) { opts.print.print(" " #y); } } while(0)
#define PRINTHEX(x) do { opts.print.print("0x"); opts.print.print(x, HEX); } while(0)
#define PRINTSCALE(x) (opts.print.print(1 << (x)))
#define PRINTPAD2(x) do { if (x < 10) { opts.print.print("0"); } opts.print.print(x, DEC); } while(0)
#define COPYVOL(dst,src) do { memcpy((void*)(&(dst)), (void*)(&(src)), sizeof(dst)); } while(0)
//FUTURE -- PRINTVEC(src,name,max) -- name[]=10011100


// When using platformio.org, peripheral details can be found in
// ~/.platformio/packages/framework-arduinosam/system/samd/CMSIS-Atmel/CMSIS/Device/ATMEL/samd51/include/component/


// table 14-4 (datasheet rev E)
static const char gclkgen_src_00[] = "XOSC0";
static const char gclkgen_src_01[] = "XOSC1";
static const char gclkgen_src_02[] = "GCLKIN";
static const char gclkgen_src_03[] = "GCLKGEN1";
static const char gclkgen_src_04[] = "OSCULP32K";
static const char gclkgen_src_05[] = "XOSC32K";
static const char gclkgen_src_06[] = "DFLL";
static const char gclkgen_src_07[] = "DPLL0";
static const char gclkgen_src_08[] = "DPLL1";
static const char gclkgen_src_UN[] = "--unknown--";
static const char* const gclkgen_srcs[] = {
    gclkgen_src_00, gclkgen_src_01, gclkgen_src_02, gclkgen_src_03,
    gclkgen_src_04, gclkgen_src_05, gclkgen_src_06, gclkgen_src_07,
    gclkgen_src_08, gclkgen_src_UN, gclkgen_src_UN, gclkgen_src_UN,
    gclkgen_src_UN, gclkgen_src_UN, gclkgen_src_UN, gclkgen_src_UN,
};
// table 14-9 (datasheet rev E)
static const char gclkchan_00[] = "OSCCTRL_DFLL48";
static const char gclkchan_01[] = "OSCCTRL_FDPLL0";
static const char gclkchan_02[] = "OSCCTRL_FDPLL1";
static const char gclkchan_03[] = "PCH03(slow)";
static const char gclkchan_04[] = "EIC";
static const char gclkchan_05[] = "FREQM_MSR";
static const char gclkchan_06[] = "FREQM_REF";
static const char gclkchan_07[] = "SERCOM0_CORE";
static const char gclkchan_08[] = "SERCOM1_CORE";
static const char gclkchan_09[] = "TC0_TC1";
static const char gclkchan_10[] = "USB";
static const char gclkchan_11[] = "EVSYS0";
static const char gclkchan_12[] = "EVSYS1";
static const char gclkchan_13[] = "EVSYS2";
static const char gclkchan_14[] = "EVSYS3";
static const char gclkchan_15[] = "EVSYS4";
static const char gclkchan_16[] = "EVSYS5";
static const char gclkchan_17[] = "EVSYS6";
static const char gclkchan_18[] = "EVSYS7";
static const char gclkchan_19[] = "EVSYS8";
static const char gclkchan_20[] = "EVSYS9";
static const char gclkchan_21[] = "EVSYS10";
static const char gclkchan_22[] = "EVSYS11";
static const char gclkchan_23[] = "SERCOM2_CORE";
static const char gclkchan_24[] = "SERCOM3_CORE";
static const char gclkchan_25[] = "TCC0_TCC1";
static const char gclkchan_26[] = "TC2_TC3";
static const char gclkchan_27[] = "CAN0";
static const char gclkchan_28[] = "CAN1";
static const char gclkchan_29[] = "TCC2_TCC3";
static const char gclkchan_30[] = "TC4_TC5";
static const char gclkchan_31[] = "PDEC";
static const char gclkchan_32[] = "AC";
static const char gclkchan_33[] = "CCL";
static const char gclkchan_34[] = "SERCOM4_CORE";
static const char gclkchan_35[] = "SERCOM5_CORE";
static const char gclkchan_36[] = "SERCOM6_CORE";
static const char gclkchan_37[] = "SERCOM7_CORE";
static const char gclkchan_38[] = "TCC4";
static const char gclkchan_39[] = "TC6_TC7";
static const char gclkchan_40[] = "ADC0";
static const char gclkchan_41[] = "ADC1";
static const char gclkchan_42[] = "DAC";
static const char gclkchan_43[] = "I2S";
static const char gclkchan_44[] = "I2S";
static const char gclkchan_45[] = "SDHC0";
static const char gclkchan_46[] = "SDHC1";
static const char gclkchan_47[] = "CM4_TRACE";
static const char* const gclkchans[] = {
    gclkchan_00, gclkchan_01, gclkchan_02, gclkchan_03, gclkchan_04, gclkchan_05, gclkchan_06, gclkchan_07,
    gclkchan_08, gclkchan_09, gclkchan_10, gclkchan_11, gclkchan_12, gclkchan_13, gclkchan_14, gclkchan_15,
    gclkchan_16, gclkchan_17, gclkchan_18, gclkchan_19, gclkchan_20, gclkchan_21, gclkchan_22, gclkchan_23,
    gclkchan_24, gclkchan_25, gclkchan_26, gclkchan_27, gclkchan_28, gclkchan_29, gclkchan_30, gclkchan_31,
    gclkchan_32, gclkchan_33, gclkchan_34, gclkchan_35, gclkchan_36, gclkchan_37, gclkchan_38, gclkchan_39,
    gclkchan_40, gclkchan_41, gclkchan_42, gclkchan_43, gclkchan_44, gclkchan_45, gclkchan_46, gclkchan_47,
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
        opts.print.print(": ");
        if (!gen.bit.GENEN) {
            opts.print.println(" --disabled--");
            continue;
        }
        opts.print.print(gclkgen_srcs[gen.bit.SRC]);
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
        opts.print.println("");
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
        opts.print.print(gclkchans[pchid]);
        opts.print.print(": ");
        if (pch.bit.CHEN) {
            opts.print.print(" GEN");
            PRINTPAD2(pch.bit.GEN);
            PRINTFLAG(pch, WRTLOCK);
            opts.print.println("");
        } else {
            opts.print.println(" --disabled--");
        }
    }
}


void printFourRegMCLK(FourRegOptions &opts) {
    opts.print.println("--------------------------- MCLK");
    opts.print.print("HSDIV:  /");
    opts.print.print(MCLK->HSDIV.reg);
    opts.print.println("");
    opts.print.print("CPUDIV:  /");
    opts.print.print(MCLK->CPUDIV.reg);
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");
}


void printFourRegOSC32KCTRL(FourRegOptions &opts) {
    opts.print.println("--------------------------- OSC32KCTRL");

    opts.print.print("EVCTRL: ");
    PRINTFLAG(OSC32KCTRL->EVCTRL, CFDEO);
    opts.print.println("");

    opts.print.print("RTCCTRL:  rtcsel=");
    switch(OSC32KCTRL->RTCCTRL.bit.RTCSEL) {
        case 0x0: opts.print.print("ULP1K"); break;
        case 0x1: opts.print.print("ULP32K"); break;
        case 0x4: opts.print.print("XOSC1K"); break;
        case 0x5: opts.print.print("XOSC32K"); break;
        default: opts.print.print("--unknown--"); break;
    }
    opts.print.println("");

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
        opts.print.println("");
    } else {
        if (opts.showDisabled) {
            opts.print.println("XOSC32K:  --disabled--");
        }
    }

    if (OSC32KCTRL->CFDCTRL.bit.CFDEN || opts.showDisabled) {
        opts.print.print("CFDCTRL: ");
        PRINTFLAG(OSC32KCTRL->CFDCTRL, CFDEN);
        PRINTFLAG(OSC32KCTRL->CFDCTRL, SWBACK);
        PRINTFLAG(OSC32KCTRL->CFDCTRL, CFDPRESC);
        opts.print.println("");
    } else {
        if (opts.showDisabled) {
            opts.print.println("CFDCTRL:  --disabled--");
        }
    }

    opts.print.print("OSCULP32K: ");
    PRINTFLAG(OSC32KCTRL->OSCULP32K, EN32K);
    PRINTFLAG(OSC32KCTRL->OSCULP32K, EN1K);
    PRINTFLAG(OSC32KCTRL->OSCULP32K, WRTLOCK);
    opts.print.print(" CALIB=");
    PRINTHEX(OSC32KCTRL->OSCULP32K.bit.CALIB);
    opts.print.println("");
}


void printFourRegOSCCTRL(FourRegOptions &opts) {
    opts.print.println("--------------------------- OSCCTRL");

    opts.print.print("EVCTRL: ");
    PRINTFLAG(OSCCTRL->EVCTRL, CFDEO0);
    PRINTFLAG(OSCCTRL->EVCTRL, CFDEO1);
    opts.print.println("");

    // DFLL
    OSCCTRL_DFLLCTRLA_Type ctrla;
    while (OSCCTRL->DFLLSYNC.bit.ENABLE) {}
    COPYVOL(ctrla, OSCCTRL->DFLLCTRLA);
    if (ctrla.bit.ENABLE || opts.showDisabled) {
        opts.print.print("DFLLCTRLA: ");
        PRINTFLAG(ctrla, ENABLE);
        PRINTFLAG(ctrla, RUNSTDBY);
        PRINTFLAG(ctrla, ONDEMAND);
        opts.print.println("");
        opts.print.print("DFLLCTRLB: ");
        OSCCTRL_DFLLCTRLB_Type ctrlb;
        while (OSCCTRL->DFLLSYNC.bit.DFLLCTRLB) {}
        COPYVOL(ctrlb, OSCCTRL->DFLLCTRLB);
        opts.print.print(ctrlb.bit.MODE ? " open-loop" : " closed-loop");
        PRINTFLAG(ctrlb, STABLE);
        PRINTFLAG(ctrlb, LLAW);
        PRINTFLAG(ctrlb, USBCRM);
        PRINTFLAG(ctrlb, CCDIS);
        PRINTFLAG(ctrlb, QLDIS);
        PRINTFLAG(ctrlb, BPLCKC);
        PRINTFLAG(ctrlb, WAITLOCK);
        opts.print.println("");
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
        opts.print.println("");
        OSCCTRL_DFLLMUL_Type dfllmul;
        while (OSCCTRL->DFLLSYNC.bit.DFLLMUL) {}
        COPYVOL(dfllmul, OSCCTRL->DFLLMUL);
        opts.print.print("DFLLMUL:  MUL=");
        opts.print.print(dfllmul.bit.MUL);
        opts.print.print(" FSTEP=");
        opts.print.print(dfllmul.bit.FSTEP);
        opts.print.print(" CSTEP=");
        opts.print.print(dfllmul.bit.CSTEP);
        opts.print.println("");
    } else {
        if (opts.showDisabled) {
            opts.print.println("DFLLCTRLA:  --disabled--");
        }
    }

    // XOSCCTRL[0:1]
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
            opts.print.println("");
        } else {
            if (opts.showDisabled) {
                opts.print.print("XOSCCTRL");
                opts.print.print(xoscid);
                opts.print.println(":  --disabled--");
            }
        }
    }

    // DPLL[0:1]
    for (uint8_t dpllid = 0; dpllid < 2; dpllid++) {
        if (OSCCTRL->Dpll[dpllid].DPLLCTRLA.bit.ENABLE || opts.showDisabled) {
            opts.print.print("DPLL");
            opts.print.print(dpllid);
            opts.print.print(": ");
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLA, ENABLE);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLA, RUNSTDBY);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLA, ONDEMAND);
            opts.print.print(" FILTER=");
            PRINTHEX(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.FILTER);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLB, WUF);
            opts.print.print(" REFCLK=");
            //FUTURE -- table for DPLLCTRLB REFCLK
            PRINTHEX(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.REFCLK);
            opts.print.print(" LTIME=");
            PRINTHEX(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.LTIME);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLB, LBYPASS);
            PRINTFLAG(OSCCTRL->Dpll[dpllid].DPLLCTRLB, DCOEN);
            if (OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.DCOEN) {
                opts.print.print(" DCOFILTER=");
                PRINTHEX(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.DCOFILTER);
            }
            opts.print.print(" DIV=");
            opts.print.print(OSCCTRL->Dpll[dpllid].DPLLCTRLB.bit.DIV);
            opts.print.print(" ldr=");
            opts.print.print(OSCCTRL->Dpll[dpllid].DPLLRATIO.bit.LDR);
            opts.print.print(".");
            opts.print.print(OSCCTRL->Dpll[dpllid].DPLLRATIO.bit.LDRFRAC);
            opts.print.println("");
        } else {
            if (opts.showDisabled) {
                opts.print.print("DPLL");
                opts.print.print(dpllid);
                opts.print.println(":  --disabled--");
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
    opts.print.print("=");
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
    opts.print.println("");
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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

    while (RTC->MODE0.SYNCBUSY.bit.FREQCORR);
    printFourRegRTC_FREQCORR(opts, RTC->MODE0.FREQCORR);

    while (RTC->MODE0.SYNCBUSY.vec.COMP);
    for (id = 0; id < 2; id++) {
        opts.print.print("COMP");
        opts.print.print(id);
        opts.print.print(":  ");
        opts.print.print(RTC->MODE0.COMP[id].bit.COMP);
        opts.print.println("");
    }

    while (RTC->MODE0.SYNCBUSY.vec.GP);
    printFourRegRTC_GP(opts, RTC->MODE0.GP);
    printFourRegRTC_TAMPCTRL(opts, RTC->MODE0.TAMPCTRL);

    opts.print.print("TIMESTAMP:  ");
    opts.print.print(RTC->MODE0.TIMESTAMP.bit.COUNT);
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

    while (RTC->MODE1.SYNCBUSY.bit.FREQCORR);
    printFourRegRTC_FREQCORR(opts, RTC->MODE1.FREQCORR);

    opts.print.print("PER:  ");
    opts.print.print(RTC->MODE1.PER.bit.PER);
    opts.print.println("");

    while (RTC->MODE1.SYNCBUSY.vec.COMP);
    for (id = 0; id < 4; id++) {
        opts.print.print("COMP");
        opts.print.print(id);
        opts.print.print(":  ");
        opts.print.print(RTC->MODE1.COMP[id].bit.COMP);
        opts.print.println("");
    }

    while (RTC->MODE1.SYNCBUSY.vec.GP);
    printFourRegRTC_GP(opts, RTC->MODE1.GP);
    printFourRegRTC_TAMPCTRL(opts, RTC->MODE1.TAMPCTRL);

    opts.print.print("TIMESTAMP:  ");
    opts.print.print(RTC->MODE1.TIMESTAMP.bit.COUNT);
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
        if (mask == 0x0) { opts.print.print("--disabled--"); }
        opts.print.println("");
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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");
}


void printFourRegPM_CFG(FourRegOptions &opts, uint8_t v) {
    switch(v) {
        case 0x0: opts.print.print("RET"); break;
        case 0x1: opts.print.print("PARTIAL"); break;
        case 0x2: opts.print.print("OFF"); break;
        default:  opts.print.print("--unknown--"); break;
    }
}
void printFourRegPM(FourRegOptions &opts) {
    opts.print.println("--------------------------- PM");

    opts.print.print("CTRLA: ");
    PRINTFLAG(PM->CTRLA, IORET);
    opts.print.println("");

    opts.print.print("HIBCFG: ");
    opts.print.print(" ramcfg=");
    printFourRegPM_CFG(opts, PM->HIBCFG.bit.RAMCFG);
    opts.print.print(" bramcfg=");
    printFourRegPM_CFG(opts, PM->HIBCFG.bit.BRAMCFG);
    opts.print.println("");

    opts.print.print("STDBYCFG: ");
    opts.print.print(" ramcfg=");
    printFourRegPM_CFG(opts, PM->STDBYCFG.bit.RAMCFG);
    opts.print.print(" FASTWKUP=");
    PRINTHEX(PM->STDBYCFG.bit.FASTWKUP);
    opts.print.println("");

    opts.print.print("BKUPCFG: ");
    opts.print.print(" bramcfg=");
    printFourRegPM_CFG(opts, PM->BKUPCFG.bit.BRAMCFG);
    opts.print.println("");

    opts.print.print("PWSAKDLY: ");
    PRINTFLAG(PM->PWSAKDLY, IGNACK);
    opts.print.print(" DLYVAL=");
    PRINTHEX(PM->PWSAKDLY.bit.DLYVAL);
    opts.print.println("");
}


void printFourReg_TC_TCC_PRESCSYNC(FourRegOptions &opts, uint8_t prescsync) {
    opts.print.print(" prescsync=");
    switch(prescsync) {
        case 0x0: opts.print.print("GCLK"); break;
        case 0x1: opts.print.print("PRESC"); break;
        case 0x2: opts.print.print("RESYNC"); break;
    }
}


void printFourRegTC_CTRLA(FourRegOptions &opts, volatile TC_CTRLA_Type& ctrla) {
    opts.print.print("CTRLA: ");
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
    opts.print.println("");
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
    }
    opts.print.println("");
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
    opts.print.println("");
}
void printFourRegTC_WAVE(FourRegOptions &opts, volatile TC_WAVE_Type& wave) {
    opts.print.print("WAVE:  WAVEGEN=");
    PRINTHEX(wave.bit.WAVEGEN);
    opts.print.println("");
}
void printFourRegTC_DRVCTRL(FourRegOptions &opts, volatile TC_DRVCTRL_Type& drvctrl) {
    opts.print.print("DRVCTRL: ");
    PRINTFLAG(drvctrl, INVEN0);
    PRINTFLAG(drvctrl, INVEN1);
    opts.print.println("");
}
void printFourRegTC_8(FourRegOptions &opts, TcCount8& tc, uint8_t idx) {
    opts.print.print("--------------------------- TC");
    opts.print.print(idx);
    opts.print.println(" COUNT8");
    while (tc.SYNCBUSY.bit.ENABLE) {}
    if (!tc.CTRLA.bit.ENABLE) {
        opts.print.println("--disabled--");
        return;
    }
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
    while (tc.SYNCBUSY.bit.ENABLE) {}
    if (!tc.CTRLA.bit.ENABLE) {
        opts.print.println("--disabled--");
        return;
    }
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
    while (tc.SYNCBUSY.bit.ENABLE) {}
    if (!tc.CTRLA.bit.ENABLE) {
        opts.print.println("--disabled--");
        return;
    }
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
    uint32_t mode = tc->COUNT8.CTRLA.bit.MODE;
    if (mode == 0x0) {
        printFourRegTC_16(opts, tc->COUNT16, idx);
        return;
    }
    if (mode == 0x1) {
        printFourRegTC_8(opts, tc->COUNT8, idx);
        return;
    }
    if (mode == 0x2) {
        printFourRegTC_32(opts, tc->COUNT32, idx);
        return;
    }
}


typedef union {
  struct {
    uint32_t SRC:2;            /*!< bit:  0.. 1  Fault A Source                     */
    uint32_t :1;               /*!< bit:      2  Reserved                           */
    uint32_t KEEP:1;           /*!< bit:      3  Fault A Keeper                     */
    uint32_t QUAL:1;           /*!< bit:      4  Fault A Qualification              */
    uint32_t BLANK:2;          /*!< bit:  5.. 6  Fault A Blanking Mode              */
    uint32_t RESTART:1;        /*!< bit:      7  Fault A Restart                    */
    uint32_t HALT:2;           /*!< bit:  8.. 9  Fault A Halt Mode                  */
    uint32_t CHSEL:2;          /*!< bit: 10..11  Fault A Capture Channel            */
    uint32_t CAPTURE:3;        /*!< bit: 12..14  Fault A Capture Action             */
    uint32_t BLANKPRESC:1;     /*!< bit:     15  Fault A Blanking Prescaler         */
    uint32_t BLANKVAL:8;       /*!< bit: 16..23  Fault A Blanking Time              */
    uint32_t FILTERVAL:4;      /*!< bit: 24..27  Fault A Filter Value               */
    uint32_t :4;               /*!< bit: 28..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} FourReg_TCC_FCTRL_Type;
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
    opts.print.println("");

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
    }
    opts.print.println("");

    for (id = 0; id < 2; id++) {
        FourReg_TCC_FCTRL_Type fctrl;
        if (id == 0) {
            opts.print.print("FCTRLA: ");
            fctrl.reg = tcc->FCTRLA.reg;
        } else {
            opts.print.print("FCTRLB: ");
            fctrl.reg = tcc->FCTRLB.reg;
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
        opts.print.println("");
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
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

    while (tcc->SYNCBUSY.bit.PATT) {}
    opts.print.print("PATT:  ");
    for (id = 0; id < 8; id++) {
        if (tcc->PATT.vec.PGE & (1<<id)) {
            if (tcc->PATT.vec.PGV & (1<<id)) {
                opts.print.print("1");
            } else {
                opts.print.print("0");
            }
        } else {
            opts.print.print(".");
        }
    }
    opts.print.println("");

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
    opts.print.println("");

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
    opts.print.println("");

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
    if (!ctrla.bit.ENABLE) {
        opts.print.println("--disabled--");
        return;
    }

    opts.print.print("CTRLA: ");
    PRINTFLAG(ctrla, ENABLE);
    PRINTFLAG(ctrla, WEN);
    PRINTFLAG(ctrla, ALWAYSON);

    opts.print.print("CONFIG:  WINDOW=");
    PRINTHEX(WDT->CONFIG.bit.WINDOW);
    opts.print.print("PER=");
    PRINTHEX(WDT->CONFIG.bit.PER);
    opts.print.println("");

    opts.print.print("EWCTRL:  EWOFFSET=");
    PRINTHEX(WDT->EWCTRL.bit.EWOFFSET);
    opts.print.println("");
}


void printFourRegs(FourRegOptions &opts) {
    // show clocks
    printFourRegGCLK(opts);
    printFourRegMCLK(opts);
    printFourRegOSC32KCTRL(opts);
    printFourRegOSCCTRL(opts);
    printFourRegRTC(opts);

    // show core peripherals
    //FUTURE printFourRegDSU(opts);
    printFourRegPAC(opts);
    printFourRegPM(opts);
    //FUTURE printFourRegSUPC(opts);
    printFourRegWDT(opts);

    // show other peripherals
    //FUTURE printFourRegAC(opts);
    //FUTURE printFourRegADC(opts);
    //FUTURE printFourRegAES(opts);
    //FUTURE printFourRegCAN(opts);
    //FUTURE printFourRegCCL(opts);
    //FUTURE printFourRegCMCC(opts);
    //FUTURE printFourRegDAC(opts);
    //FUTURE printFourRegDMAC(opts);
    //FUTURE printFourRegEIC(opts);
    //FUTURE printFourRegEVSYS(opts);
    //FUTURE printFourRegFREQM(opts);
    //FUTURE printFourRegGMAC(opts);
    //FUTURE printFourRegI2S(opts);
    //FUTURE printFourRegICM(opts);
    //FUTURE printFourRegNVMCTRL(opts);
    //FUTURE printFourRegPCC(opts);
    //FUTURE printFourRegPDEC(opts);
    //FUTURE printFourRegPORT(opts);
    //FUTURE printFourRegQSPI(opts);
    //FUTURE printFourRegSDHC(opts);
    //FUTURE printFourRegSERCOM(opts, SERCOM0, 0);
    //FUTURE printFourRegSERCOM(opts, SERCOM1, 1);
    //FUTURE printFourRegSERCOM(opts, SERCOM2, 2);
    //FUTURE printFourRegSERCOM(opts, SERCOM3, 3);
    //FUTURE printFourRegSERCOM(opts, SERCOM4, 4);
    //FUTURE printFourRegSERCOM(opts, SERCOM5, 5);
    //FUTURE printFourRegSERCOM(opts, SERCOM5, 6);
    //FUTURE printFourRegSERCOM(opts, SERCOM5, 7);
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


