
#include <Adafruit_NeoPixel.h>

 // Don't connect directly Ultrasonic to Nodemcu 
 // Always Use logic level converter and addional power supply ( Max cuurent : 12mA : NodeMcu)
 // HC-SR04+ (+) sign sensor is working with 3.3V but , sometime not working properly 
 // Use pin D5 & D6 of NodeMcu 
 // keep duty cycle min and delay 100sec ( Always good) 
 // Use latest firmware in order to get float value correctly 
 // Sensor accuracy depend on the quality of the sensor 
 // Not  suitable for direct water level monitor 
 // use proper enclosure box

#define TRIGGER_PIN  D1 
#define ECHO_PIN     D2 


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

#define PIXELPIN D4  // make sure to set this to the correct pin, ignored for UartDriven branch
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


void setup() {
	Serial.begin(115200);
	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);


	//TODO maybe update leds only if we got all pixeldata?
	pixels.begin();

	for (byte i = 0; i < NUMPIXELS; i++) {
		pixels.setPixelColor(i, 0, 0, 0);
	}
	pixels.show(); // This sends the updated pixel color to the hardware.

}

double getDistance()
{
	double duration,distance;
	//reading 1
	digitalWrite(TRIGGER_PIN, LOW);  // Get Start
	delayMicroseconds(2); // stable the line 
	digitalWrite(TRIGGER_PIN, HIGH); // sending 10 us pulse
	delayMicroseconds(10); // delay   
	digitalWrite(TRIGGER_PIN, LOW); // after sending pulse wating to receive signals   
	duration = pulseIn(ECHO_PIN, HIGH); // calculating time   

	distance = ((duration / 2) / 29.1) *  0.393701;
	return distance;
}

void loop() {

	double distance = getDistance();
	delay(200);
	double distance1 = getDistance();

	distance = (distance + distance1) / 2; //the average between 2 readings
	Serial.print(distance); 
	Serial.println(" cm");

	//calculate offset
	byte r = 50, g = 0, b = 0;
	if (distance > 300)
	{
		distance = distance - 300;
		r = 50;
		b = 50; 
		g = 50;
	}
	else if (distance > 200)
	{
		distance = distance - 200;
		r = 0;
		g = 0;
		b = 50;
	}
	else if (distance > 100)
	{
		distance = distance - 100;
		r = 0;
		g = 50;
		b = 0;
	}


	uint16_t currentLed = distance / 100 * NUMPIXELS;
	if (currentLed > NUMPIXELS) { currentLed = NUMPIXELS; }

	Serial.print(currentLed);
	Serial.println(" # pixels");

	for (byte i = 0; i <= currentLed; i++) {
		pixels.setPixelColor(i, r, g, b);
	}

	for (byte i = currentLed+1; i < NUMPIXELS; i++) {
		pixels.setPixelColor(i, 0, 0, 0);
	}

	pixels.show(); // This sends the updated pixel color to the hardware.
	delay(250);
}
