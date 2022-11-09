#include <Arduino.h>
// #include <BluetoothSerial.h>
#include <Servo.h>

// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

// #if !defined(CONFIG_BT_SPP_ENABLED)
// #error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
// #endif

// BluetoothSerial SerialBT;

int finger_num = 2;

int POT[] =  {34,32};
int servoPin[] = {12,13};

int BUTTON = 18;

unsigned long button_time = 0;  //button debouncing var
unsigned long last_button_time = 0; 

struct finger {
    Servo servo;
    int POS_base;
    int POS_diff;
    int last_pot_pos;
    int cur_pot_pos;
};

finger f1, f2;

struct finger *finger_mem[] = {&f1, &f2};
// int POT

void read_POT(int POT, finger *f){
    f->cur_pot_pos = analogRead(POT);
    f->POS_diff = f->cur_pot_pos - f->POS_base;
    delay(1);
    if(f->POS_diff > 1000){
        f->servo.writeMicroseconds(1800);
    }else{
        f->servo.writeMicroseconds(1000);
    }
}

void setup() {

  Serial.begin(115200);
  // SerialBT.begin("ESP32test");
  pinMode(BUTTON, INPUT_PULLUP);  // Pull Up Resistance

  for(int i = 0; i < finger_num - 1; i++){
    finger_mem[i]->servo.attach(servoPin[i]);    
  }

  Serial.println("ESP32 running :)");
  // Serial.println("The device started, now you can pair it with bluetooth!");  // put your setup code here, to run once:
}

void loop() {
  button_time = millis();
  if(digitalRead(BUTTON)  & (button_time - last_button_time > 200)){ //Reset POS if button is pressed (incl. debounce)
    
    Serial.println("Position Reset");

    for(int i = 0; i < finger_num - 1; i++){
      finger_mem[i]->POS_base = analogRead(POT[i]);    
      finger_mem[i]->POS_diff = 0;
      finger_mem[i]->servo.writeMicroseconds(2000);
      Serial.println(finger_mem[i]->POS_base);
    }

    delay(1000);
    last_button_time = button_time;
    
  }else{
    for(int i = 0; i < finger_num - 1; i++){
      read_POT(POT[i], finger_mem[i]);
      Serial.println(finger_mem[i]->POS_diff);
    }
  }
  // if (Serial.available()) {
    
  //   int ch = Serial.read();
  //   Serial.write(ch);
  //   SerialBT.write(ch);
  // }
  // if (SerialBT.available()) {
  //   Serial.write(SerialBT.read());
  // }
  // delay(20);
}