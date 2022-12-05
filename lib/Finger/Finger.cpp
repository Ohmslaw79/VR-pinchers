#include "Finger.h"

void Finger::set_servo_pin(int _servo_pin)
{
    servo.attach(_servo_pin);
    wiggle_servo();
}

void Finger::set_pot_pin(int _pot_pin)
{
    pot_pin = _pot_pin;
}

int Finger::get_finger_pos()
{
    return ((analogRead(pot_pin) - pot_zero) * 100) / (pot_max == 0 ? 1 : pot_max);
}
void Finger::set_finger_stop(int stop_pos)
{
    servo.writeMicroseconds(SERVO_ZERO + (10 * stop_pos));
}
void Finger::set_bounds(int _zero, int _max)
{
    pot_zero = _zero;
    pot_max = _max;
}
void Finger::calibrate()
{
    wiggle_servo();
    pot_zero = analogRead(pot_pin);
    pot_max = analogRead(pot_pin);

    for (int i = 0; i < 250; i++)
    {
        int pos = analogRead(pot_pin);
        if (pos < pot_zero)
        {
            pot_zero = pos;
        }
        else if (pos > pot_max)
        {
            pot_max = pos;
        }
        delay(10);
    }
}

void Finger::wiggle_servo(){
    servo.writeMicroseconds(1000);
    delay(1000);
    servo.writeMicroseconds(3000);
    delay(1000);
    servo.writeMicroseconds(1000);
}