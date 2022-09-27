/*
 * ESP32 Servo Example Using Arduino ESP32 Servo Library
 * John K. Bennett
 * March, 2017
 * 
 * This sketch uses the Arduino ESP32 Servo Library to sweep 4 servos in sequence.
 * 
 * Different servos require different pulse widths to vary servo angle, but the range is 
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 * 
 * Circuit:
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 * 
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pins
 * 22, 19, 23, & 18).
 * 
 * In this example, we assume four Tower Pro SG90 small servos.
 * The published min and max for this servo are 500 and 2400, respectively.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
 * Experimentally, 550 and 2350 are pretty close to 0 and 180.
 */

#include <Servo.h>
#include <Arduino.h>
Servo servo1;

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42
int SERVO1_PIN = 4;
int POT_PIN = 15;
int BTN_PIN = 18;

int servo_pos = 0;

float pot_max = 0;
float pot_val = 0;
float pot_min = 0;

float read_pot(){
	return analogRead(POT_PIN);
}

void calibrate(){
	pot_max = read_pot();
	pot_min = read_pot();
	while(!digitalRead(BTN_PIN)){
		float val = read_pot();
		if(val > pot_max) pot_max = val;
		if(val < pot_min) pot_min = val;
	}
}

void setup() {
	Serial.begin(115200);
	// Allow allocation of all timers
	pinMode(BTN_PIN, INPUT_PULLUP);
	servo1.attach(SERVO1_PIN);
}

void loop() {
	// if(!digitalRead(BTN_PIN)){
	// 	calibrate();
	// }
	for(int pos = 0; pos < 180; pos++ ){
		servo1.write(pos);
		Serial.print("Servo 1 pos: ");
		Serial.print(pos);
		Serial.print("\n");
		delay(1);
	}
	for(int pos = 180; pos > 0; pos-- ){
		servo1.write(pos);
		Serial.print("Servo 1 pos: ");
		Serial.print(pos);
		Serial.print("\n");
		delay(1);
	}
	//servo1.write(((read_pot() - pot_min) / (pot_max - pot_min)) * 180);

	delay(5000);

}