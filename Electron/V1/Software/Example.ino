// This #include statement was automatically added by the Particle IDE.
#include "adafruit-max31855/adafruit-max31855.h"

int oshLogoLedPin = D7;

// Memory...
int eepromAddress = 0x50;

#include "math.h"

int thermoCLK = A3;
int thermoCS = A2; // selectable
int thermoDO = A4;

AdafruitMAX31855 thermocouple(thermoCS); //hardware mode
bool hasProbe = false;
bool isCalibrated = false;

int calibrate(String args);
int measure(String args);
int startTimer(String args);
int stopTimer(String args);

void setup() {
    
    pinMode(oshLogoLedPin, OUTPUT);
    
    // MAX31855 Thermocouple sensor.
    thermocouple.init();
    
    Particle.function("calibrate", calibrate);
    Particle.function("measure", measure);
    Particle.function("startTimer", measure);
    Particle.function("stopTimer", measure);
    
    Particle.publish("Status", "Thermocouple V0.1.0");
    
    // wait for MAX chip to stabilize and published message to go.
    delay(500);
}

void loop() {
    
    digitalWrite(oshLogoLedPin, HIGH);
    delay(1000);
    
    //readVin();
    String senml = "{e:[";
    
    // Read the thermocouple temperatures.
    double internalTemp = thermocouple.readInternal();
    
    if (isnan(internalTemp)) {
        Particle.publish("status", "failed to read internal temperature.");
    } else {
        senml+="{'n':'internal','v':'" + String(internalTemp, DEC) + "'},";
        double probeTemperature = thermocouple.readCelsius();
    
        if (isnan(probeTemperature)) {
            Particle.publish("status", "No probe detected");
            hasProbe = false;
        } else {
            hasProbe = true;
            senml+="{'n':'probe','v':'" + String(probeTemperature, DEC) + "'},";
        }
    
        senml +="]}";
        Particle.publish("senml", senml, 60, PRIVATE);
    }

    digitalWrite(oshLogoLedPin, LOW);
    
    // TODO: Sleep to save power.
    delay(5000);
    
    
    // EEPROM TEST
    //writeEEPROM(eepromAddress, 1, 127);
    //byte read = readEEPROM(eepromAddress, 0);
    //byte read1 = readEEPROM(eepromAddress, 1);
    //Particle.publish("Status", "Read: " + String(read, DEC) + ", " + String(read1, DEC));
}

// From http://www.hobbytronics.co.uk/arduino-external-eeprom
void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
 
  delay(5);
}
 
// From http://www.hobbytronics.co.uk/arduino-external-eeprom
byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}

// ==================================================================
// Particle functions
// ==================================================================
int calibrate(String args) {
    
    double internalTemp = thermocouple.readInternal();
    double probeTemperature = thermocouple.readCelsius();

    if (isnan(probeTemperature)) {
        Particle.publish("status", "Could not calibrate, no probe detected.");
        hasProbe = false;
        isCalibrated = false;
        return 0;
    } else {
        // Probe connected, do an initial calibration.
        hasProbe = true;
            
        thermocouple.calibrate();
            
        double calibrationValue = thermocouple.readCalibration();
    
        String senml = "{e:[";    
        senml+="{'n':'internal','v':'" + String(internalTemp, DEC) + "'},";
        senml+="{'n':'calibrationValue','v':'" + String(calibrationValue, DEC) + "'},";
        senml +="]}";
        Particle.publish("senml", senml, 60, PRIVATE);
        delay(500);
        Particle.publish("status", "Calibrated at " + String(internalTemp, DEC) + "C");
        isCalibrated = true;
    }
    
    return 1;
}

// Take a measurement
int  measure(String args) {
    return 0;
}

int startTimer(String args) {
    return 0;
}

int stopTimer(String args) {
    return 0;
}
