#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Servo.h>
#include <iostream>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#define DEBOUNCE_TIME 200

struct Potentiometer{
  int pin;
  int pos;
};
struct Finger
{
  Servo servo;
  Potentiometer pot;
  int zero;     // Finger flex 0%
  int max;      // Finger flex 100%
};

BluetoothSerial SerialBT;

int finger_num = 2;

int POT[] = {36, 39, 34, 35, 32};
int servoPin[] = {23, 22, 21, 19, 18};

Finger f1, f2, f3, f4, f5;
struct Finger *finger_mem[] = {&f1, &f2, &f3, &f4, &f5};

int BUTTON = 17;
int reset_flag = 0;

unsigned long button_time = 0; // button debouncing var
unsigned long last_button_time = 0;

int tog = 0;
int togLim = 7;

void setup()
{

  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  pinMode(BUTTON, INPUT_PULLUP); // Pull Up Resistance

  for (int i = 0; i < finger_num; i++)
  {
    finger_mem[i]->servo.attach(servoPin[i]);
    finger_mem[i]->pot.pin = POT[i]
  }

  Serial.println("ESP32 running :)");

}

void loop()
{
  button_time = millis();
  if (digitalRead(BUTTON) == LOW && (button_time - last_button_time > DEBOUNCE_TIME))
  { //*Reset POS if button is pressed (incl. debounce)

    Serial.println("Position Reset");

    for (int i = 0; i < finger_num; i++)
    {
      finger_mem[i]->POS_base = analogRead(POT[i]);
      finger_mem[i]->POS_diff = 0;
      finger_mem[i]->servo.writeMicroseconds(2000);

      Serial << i << ":" << finger_mem[i]->POS_diff;
      Serial.println();
      SerialBT << i << ":" << finger_mem[i]->POS_diff;
    }

    reset_flag = 1;

    delay(1000);
    last_button_time = button_time;
  }
  else
  {

    tog++;
    for (int i = 0; i < finger_num; i++)
    {

      if (tog % togLim == 0)
      {
        read_POT(POT[i], finger_mem[i]);
        Serial << i << ":" << finger_mem[i]->POS_diff;
        Serial.println();
        SerialBT << i << ":" << finger_mem[i]->POS_diff;
        SerialBT.println();
        tog = 0;
      }
    }
  }

  //* Bluetooth stuff
  if (Serial.available())
  {
    SerialBT.write(Serial.read());
    // for(int i = 0; i < finger_num; i++){
    // SerialBT.write(i + ":" + finger_mem[i]->POS_diff);
    // }
    // Serial.println();
    reset_flag = 0;
  }

  // Recieve where to stop
  if (SerialBT.available())
  {
    Serial.write(SerialBT.read());
  }

  delay(20);
}

void read_POT(Finger *finger)
{
  finger->pot.pos = analogRead(finger->pot.pin);
}
