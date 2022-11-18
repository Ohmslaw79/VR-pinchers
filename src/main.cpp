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

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

BluetoothSerial SerialBT;

int POT[] =  {34,33};
int servoPin[] = {12,13};

int BUTTON = 18;
int reset_flag = 0;

unsigned long button_time = 0;  //button debouncing var
unsigned long last_button_time = 0; 

struct finger {
    Servo servo;
    int POS_base;
    int POS_diff;
    int last_pot_pos;
    int cur_pot_pos;
};

int finger_num = 2;

finger f1, f2;

struct finger *finger_mem[] = {&f1, &f2};

void read_POT(int POT, finger *f){
    f->cur_pot_pos = analogRead(POT);
    f->POS_diff = f->cur_pot_pos - f->POS_base;
    delay(1);
    //* Remove below for BT compatability
    if(f->POS_diff > 1000){
        f->servo.writeMicroseconds(1800);
    }else{
        f->servo.writeMicroseconds(1000);
    }
}

void setup() {

  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  pinMode(BUTTON, INPUT_PULLUP);  // Pull Up Resistance

  for(int i = 0; i < finger_num; i++){
    finger_mem[i]->servo.attach(servoPin[i]);    
  }

  Serial.println("ESP32 running :)");
}

void loop() {
  button_time = millis();
  if(digitalRead(BUTTON)  & (button_time - last_button_time > 200)){ //*Reset POS if button is pressed (incl. debounce)
    
    Serial.println("Position Reset");
    SerialBT.println("Position Reset");

    for(int i = 0; i < finger_num; i++){
      finger_mem[i]->POS_base = analogRead(POT[i]);    
      finger_mem[i]->POS_diff = 0;
      finger_mem[i]->servo.writeMicroseconds(1000);
      Serial << i << ":" << finger_mem[i]->POS_diff;
      Serial.println();

      SerialBT << i << ":" << finger_mem[i]->POS_diff;
      
    }

    reset_flag = 1;

    delay(1000);
    last_button_time = button_time;
    
  }else{
    for(int i = 0; i < finger_num; i++){
      read_POT(POT[i], finger_mem[i]);
      Serial << i << ":" << finger_mem[i]->POS_diff;
      Serial.println();

      SerialBT << i << ":" << finger_mem[i]->POS_diff;
      SerialBT.println();

    }
  }

  //* Bluetooth stuff
  if (Serial.available()) {
    SerialBT.write(Serial.read());
    // for(int i = 0; i < finger_num; i++){
      // SerialBT.write(i + ":" + finger_mem[i]->POS_diff);
    // }
    // Serial.println();
    reset_flag = 0;
  }

  // Recieve where to stop
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

  delay(20);
}