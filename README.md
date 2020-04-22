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


### void printFourRegOSC32KCTRL(FourRegOptions &opts)
Prints out configuration for 32k oscillators.

example output:
```text
--------------------------- OSC32KCTRL
EVCTRL:
RTCCTRL:  rtcsel=ULP1K
XOSC32K:  ENABLE XTALEN EN32K startup=0x0 cgm=0x1
OSCULP32K:  EN32K EN1K calib=0x27
```


### void printFourRegOSCCTRL(FourRegOptions &opts)
Prints out configuration for other oscillators

example output:
```text
--------------------------- OSCCTRL
EVCTRL:
DFLLCTRLA:  ENABLE
DFLLCTRLB:  closed-loop USBCRM CCDIS WAITLOCK
DFLLVAL:  fine=131 coarse=31
DFLLMUL:  mul=0 fstep=1 cstep=1
DPLL0:  ENABLE filter=0x0 refclk=0x0 ltime=0x0 LBYPASS div=0 ldr=119.0
DPLL1:  ENABLE filter=0x0 refclk=0x0 ltime=0x0 LBYPASS div=0 ldr=99.0
```


### void printFourRegPM(FourRegOptions &opts)
Prints out configuration for the power manager.

example output:
```text
--------------------------- PM
CTRLA:
HIBCFG:  ramcfg=RET bramcfg=RET
STDBYCFG:  ramcfg=RET fastwkup=0x0
BKUPCFG:  bramcfg=RET
PWSAKDLY:  bramcfg=0x0
```


### void printFourRegTC(FourRegOptions &opts, Tc* tc, uint8_t idx)
Prints out configuration for a TC peripheral.

example output:
```text
--------------------------- TC4 COUNT8
CTRLA:  prescsync=GCLK captmode0=0x0 captmode1=0x0
CTRLB:  dir=DOWN cmd=NONE
EVCTRL:  evact=OFF
WAVE:  wavegen=0x3
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


