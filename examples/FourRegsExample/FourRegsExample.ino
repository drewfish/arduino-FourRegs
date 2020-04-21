#include <FourRegs.h>

void setup() {
  Serial.begin(9600);
  while (! Serial) {}  // wait for serial monitor to attach
  FourRegOptions opts = { Serial, false };
  printFourRegs(opts);
}

void loop() {
  // Do nothing
}
