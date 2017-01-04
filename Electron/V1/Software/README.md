Example code for Thermocouple ThingyStick
#########################################

Based on the Particle Photon. Should work for the Electron as well.

Assumed Chip Select for the thermocouple sensor is on A2.

To use this example:

* Create a new App in build.particle.io
* Paste in the sample code.
* Add in the "adafruit-max31855" library
* Remove the now duplicate #include for the adafruit-max31855 library.
* Set "int thermoCS = A2; // selectable" to the appropriate value for the selected chip select line.

When you first use a thermocouple probe it needs to be calibrated:

* Ensure the sensor board and the probe are at the same temperature
* Call the "calibrate" particle function
