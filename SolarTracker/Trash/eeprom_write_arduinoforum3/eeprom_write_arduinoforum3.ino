#include "Wire.h"

// EEPROM I2C Address
#define EEPROM_I2C_ADDRESS 0x51

// Analog pin for potentiometer
int analogPin = 0;

// Integer to hold potentiometer value
int val = 0;

// Byte to hold data read from EEPROM
int readVal = 0;

// Integer to hold number of addresses to fill
int maxaddress = 150;

// Function to write to EEPROOM
void writeEEPROM(byte address, byte val, int i2c_address)
{
// Begin transmission to I2C EEPROM
Wire.beginTransmission(i2c_address);

// Send memory address as two 8-bit bytes
//Wire.write((int)(address >> 8)); // MSB
//Wire.write((int)(address & 0xFF)); // LSB
Wire.write(address);

// Send data to be stored
Wire.write(val);

// End the transmission
Wire.endTransmission();

// Add 5ms delay for EEPROM
delay(5);
}

// Function to read from EEPROM
byte readEEPROM(byte address, int i2c_address)
{
// Define byte for received data
byte rcvData = 0xFF;

// Begin transmission to I2C EEPROM
Wire.beginTransmission(i2c_address);

// Send memory address as two 8-bit bytes
//Wire.write((int)(address >> 8)); // MSB
//Wire.write((int)(address & 0xFF)); // LSB
Wire.write(address);

// End the transmission
Wire.endTransmission();

// Request one byte of data at current memory address
Wire.requestFrom(i2c_address, 1);

// Read the data and assign to variable
rcvData = Wire.read();

// Return the data as function output
return rcvData;
}

void setup()
{
  delay(3000);
// Connect to I2C bus as master
Wire.begin();

// Setup Serial Monitor
Serial.begin(9600);

// Print to Serial Monitor
Serial.println("Start Recording...");

// Run until maximum address is reached

for (int address = 0; address <= maxaddress; address++) {

// Read pot and map to range of 0-180 for servo
val = map(analogRead(analogPin), 0, 1023, 0, 180);

// Write to the servo
// Delay to allow servo to settle in position

delay(15);

// Record the position in the external EEPROM
writeEEPROM(address, val, EEPROM_I2C_ADDRESS);

// Print to Serial Monitor
Serial.print("ADDR = ");
Serial.print(address);
Serial.print("\t");
Serial.println(val);
}

// Print to Serial Monitor
Serial.println("Recording Finished!");

// Delay 5 Seconds
delay(5000);

// Print to Serial Monitor
Serial.println("Begin Playback...");

// Run until maximum address is reached

for (int address = 0; address <= maxaddress; address++) {

// Read value from EEPROM
readVal = readEEPROM(address, EEPROM_I2C_ADDRESS);


// Write to the servo
// Delay to allow servo to settle in position
// Convert value to integer for servo

delay(15);

// Print to Serial Monitor
Serial.print("ADDR = ");
Serial.print(address);
Serial.print("\t");
Serial.println(readVal);
}

// Print to Serial Monitor
Serial.println("Playback Finished!");

}

void loop()
{}
