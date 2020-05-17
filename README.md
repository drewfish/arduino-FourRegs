Arduino FourRegs
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


### void printFourRegCMCC(FourRegOptions &opts)
Prints out configuration for the cache controller.


### void printFourRegDAC(FourRegOptions &opts)
Prints out configuration for the DAC peripheral.


### void printFourRegDMAC(FourRegOptions &opts)
Prints out configuration for the DMAC peripheral.


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


### void printFourRegFREQM(FourRegOptions &opts)
Prints out configuration for an FREQM peripheral.


### void printFourRegGCLK(FourRegOptions &opts)
Prints out configuration for the general clocks.

example output:
```text
--------------------------- GCLK
GEN00:  DPLL0/1 IDC
GEN01:  DFLL48M/1 IDC
GEN02:  DPLL1/1 IDC
GEN03:  XOSC32K/1
GEN04:  DFLL48M/4 IDC
GEN05:  DFLL48M/48
GCLK_MAIN:  GEN00 (always)
GCLK_OSCCTRL_DPLL0_REF:  GEN05
GCLK_OSCCTRL_DPLL1_REF:  GEN05
GCLK_PCH03(slow):  GEN03
GCLK_EIC:  GEN02
GCLK_USB:  GEN01
GCLK_SERCOM2_CORE:  GEN01
GCLK_ADC0:  GEN01
GCLK_ADC1:  GEN01
GCLK_DAC:  GEN04
```


### void printFourRegI2S(FourRegOptions &opts)
Prints out configuration for the I2S peripheral.
This method only exists if the processor has the I2S peripheral.


### void printFourRegICM(FourRegOptions &opts)
Prints out configuration for the ICM peripheral.


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
DFLLCTRLB:  open-loop USBCRM CCDIS WAITLOCK
DFLLVAL:  FINE=131 COARSE=31
DFLLMUL:  MUL=0 FSTEP=1 CSTEP=1
DPLL0:  ENABLE refclk=GCLK_OSCCTRL_DPLL0_REF ldr=119.0 FILTER=0x0 LTIME=0x0 LBYPASS
DPLL1:  ENABLE refclk=GCLK_OSCCTRL_DPLL1_REF ldr=99.0 FILTER=0x0 LTIME=0x0 LBYPASS
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


### void printFourRegPCC(FourRegOptions &opts)
Prints out configuration for the PCC peripheral.


### void printFourRegPDEC(FourRegOptions &opts)
Prints out configuration for the PDEC peripheral.


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
--------------------------- PORT A
PA02:  input INEN
PA04:  input INEN
PA05:  input INEN
PA06:  input INEN
PA07:  input INEN
PA12:  pmux SERCOM2:0(usart:tx) input INEN
PA13:  pmux SERCOM2:1(usart:rx) input INEN
PA15:  input INEN
PA16:  pmux TCC0:4 output INEN DRVSTR
PA17:  output INEN
PA18:  input INEN
PA19:  pmux EIC:3 input INEN pull=UP
PA20:  input INEN
PA22:  pmux SERCOM3:0(i2c:sda)
PA23:  pmux SERCOM3:1(i2c:scl)
PA24:  pmux USB:DM
PA25:  pmux USB:DP
PA30:  pmux CORTEX_CM4:SWCLK
EVCTRL:
--------------------------- PORT B
PB02:  input INEN
PB08:  input INEN
PB09:  pmux ADC0:3,ADC1:1,X2/Y2 input INEN
PB11:  pmux SERCOM4:3(spi:miso)
PB12:  pmux SERCOM4:0(spi:mosi)
PB13:  pmux SERCOM4:1(spi:sck)
EVCTRL:
--------------------------- PORT C
EVCTRL:
--------------------------- PORT D
EVCTRL:
```


### void printFourRegQSPI(FourRegOptions &opts)
Prints out configuration for the QSPI peripheral.


### void printFourRegRTC(FourRegOptions &opts)
Prints out configuration for the realtime clock.


### void printFourRegSCS(FourRegOptions &opts)
Prints out configuration for the ARM Cortex-M4 system control space.

example output:
```text
--------------------------- SCS
CPUID:  REV=0x1 PARTNO=0xC24 ARCH=0xF VAR=0x0 IMPL=0x41
SysTick:  ENABLE TICKINT clksource=CPU RELOAD=119999 TENMS=0 NOREF
irq pri0:  PM MCLK OSCCTRL:0 OSCCTRL:1 OSCCTRL:2 OSCCTRL:3 OSCCTRL:4 OSC32KCTRL SUPC:0 SUPC:1 WDT RTC EIC:0 EIC:1 EIC:2 EIC:3 EIC:4 EIC:5 EIC:6 EIC:7 EIC:8 EIC:9 EIC:10 EIC:11 EIC:12 EIC:13 EIC:14 EIC:15 FREQM NVMCTRL:0 NVMCTRL:1 DMAC:0 DMAC:1 DMAC:2 DMAC:3 DMAC:4 EVSYS:0 EVSYS:1 EVSYS:2 EVSYS:3 EVSYS:4 PAC RAMECC SERCOM0:0 SERCOM0:1 SERCOM0:2 SERCOM0:3 SERCOM1:0 SERCOM1:1 SERCOM1:2 SERCOM1:3 SERCOM5:0 SERCOM5:1 SERCOM5:2 SERCOM5:3 USB:0 USB:1 USB:2 USB:3 TCC0:0 TCC0:1 TCC0:2 TCC0:3 TCC0:4 TCC0:5 TCC0:6 TCC1:0 TCC1:1 TCC1:2 TCC1:3 TCC1:4 TCC2:0 TCC2:1 TCC2:2 TCC2:3 TCC3:0 TCC3:1 TCC3:2 TCC4:0 TCC4:1 TCC4:2 TC0 TC1 TC2 TC3 TC4 TC5 PDEC:0 PDEC:1 PDEC:2 ADC0:0 ADC0:1 ADC1:0 ADC1:1 AC DAC:0 DAC:1 DAC:2 DAC:3 DAC:4 I2S PCC AES TRNG ICM PUKCC QSPI SDHC0
irq pri1:
irq pri2:
irq pri3:  SERCOM2:0 SERCOM2:1 SERCOM2:2 SERCOM2:3 SERCOM3:0 SERCOM3:1 SERCOM3:2 SERCOM3:3 SERCOM4:0 SERCOM4:1 SERCOM4:2 SERCOM4:3
irq pri4:
irq pri5:
irq pri6:
irq pri7:
```


### void printFourRegSERCOM(FourRegOptions &opts, Sercom* sercom, uint8_t idx)
Prints out configuration for a SERCOM peripheral.

example otuput:
```text
--------------------------- SERCOM2 USART (internal clock)
CTRLA:  ENABLE sampr=16x tx=PAD0 rx=PAD1 SAMPA=0x0 FORM=0x0 cmode=ASYNC CPOL=0 dord=MSB
CTRLB:  chsize=8bit TXEN RXEN LINCMD=0x0
CTRLC:  GTIME=0x0 DATA32B=0x0
BAUD:  312.4
--------------------------- SERCOM3 I2C master
CTRLA:  ENABLE SDAHOLD=0x0 SPEED=0x0 INACTOUT=0x0
CTRLB:  ackact=ACK
CTRLC:
BAUD:  BAUD=0xEF BAUDLOW=0x0 HSBAUD=0x0 HSBAUDLOW=0x0
--------------------------- SERCOM4 SPI master
CTRLA:  ENABLE mosi=PAD0 sck=PAD1 miso=PAD3 form=SPI cpha=LEADING cpol=LOW dord=MSB
CTRLB:  chsize=8bit amode=MASK RXEN
CTRLC:  ICSPACE=0
BAUD:  0x5
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


### void printFourRegTRNG(FourRegOptions &opts)
Prints out configuration for the TRNG peripheral.


### void printFourRegUSB(FourRegOptions &opts)
Prints out configuration for the USB peripheral.

example output:
```text
--------------------------- USB
CTRLA:  ENABLE RUNSTDBY mode=DEVICE
QOSCTRL:  cqos=HIGH dqos=HIGH
CTRLB:  spdconf=FS LPMHDSK=0x0
DADD:  0x29 ADDEN
PADCAL:  TRANSP=0x19 TRANSN=0x9 TRIM=0x6
ENDPOINT0:
    BANK0:  eptype=CTRL-out ADDR=0x20000A4C size=64byte EXTREG=0x0
    BANK1:  eptype=CTRL-in ADDR=0x20000888 size=64byte
ENDPOINT1:
    BANK0:  eptype=--disabled--
    BANK1:  eptype=INT-in ADDR=0x200008C8 size=64byte
ENDPOINT2:
    BANK0:  eptype=BULK-out ADDR=0x20000DA0 size=64byte EXTREG=0x0
    BANK1:  eptype=--disabled--
ENDPOINT3:
    BANK0:  eptype=--disabled--
    BANK1:  eptype=BULK-in ADDR=0x20000948 size=64byte
ENDPOINT4:
    BANK0:  eptype=--disabled--
    BANK1:  eptype=--disabled--
ENDPOINT5:
    BANK0:  eptype=--disabled--
    BANK1:  eptype=--disabled--
ENDPOINT6:
    BANK0:  eptype=--disabled--
    BANK1:  eptype=--disabled--
ENDPOINT7:
    BANK0:  eptype=--disabled--
    BANK1:  eptype=--disabled--
```


### void printFourRegWDT(FourRegOptions &opts)
Prints out configuration for the watchdog timer.


License
-------
This code is licensed under the MIT license.
See the `LICENSE` file for details.


