arduino FourRegs
================
This library prints the registers for Arduino-compatible boards which use an ARM SAMD51 processor.
It specifically is designed to print the values of the _configuration_ register
but not the _status_ registers.

**Intended Audience:** advanced users who have already looked at the Atmel SAM D51 datasheet.

[![Build Status](https://travis-ci.org/drewfish/arduino-FourRegs.svg?branch=master)](https://travis-ci.org/drewfish/arduino-FourRegs)

Examples
--------
This will show the registers when the device boots:
```cpp
#include <FourRegs.h>
void setup() {
    SerialUSB.begin(9600);
    while (! SerialUSB) {}  // wait for serial monitor to attach
    FourRegOptions opts = { SerialUSB, false };
    printFourRegs(opts);
}
```


This will show the registers every time USB is connected:
```cpp
#include <FourRegs.h>
bool serialShown = false;
void setup() {
    SerialUSB.begin(9600);
}
void loop() {
    if (SerialUSB) {
        if (! serialShown) {
            FourRegOptions opts = { SerialUSB, false };
            printFourRegs(opts);
            serialShown = true;
        }
    } else {
        serialShown = false;
    }
}
```



Function Reference
------------------


### struct FourRegOptions

| type | name | description |
| ---- | ---- | ---- |
| `Print&` | `print` | object used for printing output |
| `bool` | `showDisabled` | whether to show items which are disabled |


### void printFourRegs(FourRegOptions &opts)
Prints out configuration registers, as many as this library knows how.


### void printFourRegAC(FourRegOptions &opts)
Prints out configuration for the AC peripheral.


### void printFourRegADC(FourRegOptions &opts, Adc* adc, uint8_t idx)
Prints out configuration for an ADC peripheral.


### void printFourRegAES(FourRegOptions &opts)
Prints out configuration for the AES peripheral.


### void printFourRegCCL(FourRegOptions &opts)
Prints out configuration for the CCL peripheral.


### void printFourRegEIC(FourRegOptions &opts)
Prints out configuration for the external interrupt controller.

example output:
```text
--------------------------- EIC
EIC:  ENABLE cksel=GCLK_EIC
EXTINT03:  FALL
DPRESCALER:  PRESCALER0=0x0 STATES0=0x0 PRESCALER1=0x0 STATES1=0x0
```


### void printFourRegEVSYS(FourRegOptions &opts)
Prints out configuration for the event system.


### void printFourRegGCLK(FourRegOptions &opts)
Prints out configuration for the general clocks.

example output:
```text
--------------------------- GCLK
GEN00: DPLL0/1 IDC
GEN01: DFLL/1 IDC
GEN02: DPLL1/1 IDC
GEN03: XOSC32K/1
GEN04: DFLL/4 IDC
GEN05: DFLL/48
GCLK_MAIN:  GEN00 (always)
GCLK_OSCCTRL_FDPLL0:  GEN05
GCLK_OSCCTRL_FDPLL1:  GEN05
GCLK_USB:  GEN01
GCLK_ADC0:  GEN01
GCLK_ADC1:  GEN01
GCLK_DAC:  GEN04
```


### void printFourRegMCLK(FourRegOptions &opts)
Prints out configuration for the main (CPU) clock.

example output:
```text
--------------------------- MCLK
HSDIV:  /1
CPUDIV:  /1
AHBMASK:  HPB0 HPB1 HPB2 HPB3 DSU HMATRIX NVMCTRL HSRAM CMCC DMAC USB BKUPRAM PAC QSPI SDHC0 ICM PUKCC QSPI_2X NVMCTRL_SMEEPROM NVMCTRL_CACHE
APBAMASK:  PAC PM MCLK RSTC OSCCTRL OSC32KCTRL SUPC GCLK WDT RTC EIC SERCOM0 SERCOM1 TC0 TC1
APBBMASK:  USB DSU NVMCTRL PORT HMATRIX SERCOM2 SERCOM3 TCC0 TCC1 TC2 TC3 TAL RAMECC
APBCMASK:  TCC2 TCC3 TC4 TC5 QSPI
APBDMASK:  SERCOM4 SERCOM5 TCC4 ADC0 ADC1 DAC
```


### void printFourRegNVMCTRL(FourRegOptions &opts)
Prints out configuration for the nonvolatile memory controller.

example output:
```text
--------------------------- NVMCTRL
CTRLA:  AUTOWS wmode=MAN prm=SEMIAUTO RWS=0
PARAM:  NVMP=2048 psz=1024bytes SEE
STATUS:  AFIRST bootprot=16kb
RUNLOCK:  --------------------------------
SEECFG:  wmode=UNBUFFERED
SEESTAT:  ASEES=1 RLOCK SBLK=0 PSZ=0

```


### void printFourRegOSC32KCTRL(FourRegOptions &opts)
Prints out configuration for 32k oscillators.

example output:
```text
--------------------------- OSC32KCTRL
EVCTRL:
RTCCTRL:  rtcsel=ULP1K
XOSC32K:  ENABLE XTALEN EN32K STARTUP=0x0 CGM=0x1
OSCULP32K:  EN32K EN1K CALIB=0x27
```


### void printFourRegOSCCTRL(FourRegOptions &opts)
Prints out configuration for other oscillators.

example output:
```text
--------------------------- OSCCTRL
EVCTRL:
DFLLCTRLA:  ENABLE
DFLLCTRLB:  closed-loop USBCRM CCDIS WAITLOCK
DFLLVAL:  FINE=131 COARSE=31
DFLLMUL:  MUL=0 FSTEP=1 CSTEP=1
DPLL0:  ENABLE FILTER=0x0 REFCLK=0x0 LTIME=0x0 LBYPASS DIV=0 ldr=119.0
DPLL1:  ENABLE FILTER=0x0 REFCLK=0x0 LTIME=0x0 LBYPASS DIV=0 ldr=99.0
```


### void printFourRegPAC(FourRegOptions &opts)
Prints out configuration for the peripheral access controller.

example output:
```text
--------------------------- PAC
EVCTRL:
STATUSA:
STATUSB:  DSU
STATUSC:
STATUSD:
```


### void printFourRegPM(FourRegOptions &opts)
Prints out configuration for the power manager.

example output:
```text
--------------------------- PM
CTRLA:
HIBCFG:  ramcfg=RET bramcfg=RET
STDBYCFG:  ramcfg=RET FASTWKUP=0x0
BKUPCFG:  bramcfg=RET
PWSAKDLY:  DLYVAL=0x0
```


### void printFourRegPORT(FourRegOptions &opts)
Prints out configuration for the I/O pin controller ports.

example output:
```text
--------------------------- PORTA
PA02:  input INEN
PA04:  input INEN
PA05:  input INEN
PA06:  input INEN
PA07:  input INEN
PA12:  input INEN
PA13:  input INEN
PA15:  input INEN
PA16:  input INEN
PA17:  output INEN
PA18:  input INEN
PA19:  pmux EIC:3
PA20:  input INEN
PA24:  pmux USB:DM
PA25:  pmux USB:DP
PA30:  pmux CORTEX_CM4:SWCLK
EVCTRL:
--------------------------- PORTB
PB02:  input INEN
PB08:  input INEN
PB09:  input INEN
EVCTRL:
--------------------------- PORTC
EVCTRL:
--------------------------- PORTD
EVCTRL:
```


### void printFourRegRTC(FourRegOptions &opts)
Prints out configuration for the realtime clock.


### void printFourRegSERCOM(FourRegOptions &opts, Sercom* sercom, uint8_t idx)
Prints out configuration for a SERCOM peripheral.

example otuput:
```text
--------------------------- SERCOM2 USART (internal clock)
CTRLA:  ENABLE sampr=16x tx=PAD0 xck=PAD1 rx=PAD1 SAMPA=0x0 FORM=0x0 cmode=ASYNC CPOL=0 dord=MSB
CTRLB:  chsize=8bit TXEN RXEN LINCMD=0x0
CTRLC:  GTIME=0x0 DATA32B=0x0
BAUD:  312.4
```


### void printFourRegSUPC(FourRegOptions &opts)
Prints out configuration for the supply controller.

example output:
```text
BOD12:  ENABLE HYST ACTION=0x1 PSEL=0x0 LEVEL=0xD
VREG:  ENABLE VSPER=0x0
VREF:  SEL=0x0
BBPS:
BKIN:  0x2
```


### void printFourRegTC(FourRegOptions &opts, Tc* tc, uint8_t idx)
Prints out configuration for a TC peripheral.

example output:
```text
--------------------------- TC4 COUNT8
CTRLA:  prescsync=GCLK CAPTMODE0=0x0 CAPTMODE1=0x0
CTRLB:  dir=DOWN cmd=NONE
EVCTRL:  evact=OFF
WAVE:  WAVEGEN=0x3
DRVCTRL:
PER:  150
CC0:  100
CC1:  0
```


### void printFourRegTCC(FourRegOptions &opts, Tcc* tcc, uint8_t idx)
Prints out configuration for a TCC peripheral.


### void printFourRegWDT(FourRegOptions &opts)
Prints out configuration for the watchdog timer.


License
-------
This code is licensed under the MIT license.
See the `LICENSE` file for details.


