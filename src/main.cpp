#include <Arduino.h>
#include <BluetoothSerial.h>
#include <iostream>
#include "Finger.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#define SLOW_PRINT_INTERVAL 10

#define DEBOUNCE_TIME 200
int BUTTON = 17;
unsigned long button_time = 0; // button debouncing var
unsigned long last_button_time = 0;

BluetoothSerial SerialBT;

int POT[] = {36, 39, 34, 35, 32};
int servoPin[] = {23, 22, 21, 19, 18};

int finger_num = 2;
Finger f1, f2, f3, f4, f5;
struct Finger *fingers[] = {&f1, &f2, &f3, &f4, &f5};

bool slow_print = true;
int print_counter = 0;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("VR_Pinchers");
  pinMode(BUTTON, INPUT_PULLUP); // Pull Up Resistance

  for (int i = 0; i < finger_num; i++)
  {
    fingers[i]->set_servo_pin(servoPin[i]);
    fingers[i]->set_pot_pin(POT[i]);
  }
  last_button_time = millis();
  Serial.println("VR_Pinchers running :)");
}

void loop()
{
  button_time = millis();
  if (!digitalRead(BUTTON) && ((button_time - last_button_time) > 200))
  { //*Reset POS if button is pressed (incl. debounce)

    Serial.print("Recalibrating...");

    for (int i = 0; i < finger_num; i++)
    {
      SerialBT.println(String(i) + ":...");
      fingers[i]->calibrate();
      Serial.print(".");
    }
    Serial.print("\n");
    last_button_time = button_time;

    Serial.println("Recalibration Complete");
  }
  else
  {
    for (int i = 0; i < finger_num; i++)
    {
      String output = String(i) + ":" + String(fingers[i]->get_finger_pos());
      SerialBT.println(output);
      if (slow_print && print_counter == SLOW_PRINT_INTERVAL)
      {
        Serial.println(output);
      }
      else if(!slow_print)
      {
        Serial.println(output);
      }
    }

    if (print_counter == SLOW_PRINT_INTERVAL)
    {
      print_counter = 1;
    }
    else
    {
      print_counter++;
    }
    delay(50);
  }

  // Receive where to stop
  if (SerialBT.available())
  {
    String data = SerialBT.readStringUntil('\n');
    Serial.println(data);
    for (int i = 0; i < finger_num; i++)
    {
      fingers[i]->set_finger_stop(data.substring(i * 3, (i + 1) * 3).toInt());
    }
  }
}
