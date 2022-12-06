#ifndef FINGER_H_
#define FINGER_H_

#include <servo.h>

#define SERVO_ZERO 2000
#define SERVO_MAX 800

class Finger{
    private:
        Servo servo;
        int servo_pos;
        int pot_pin;
        int pot_zero;
        int pot_max = 100;

    public:
        void set_servo_pin(int _servo_pin);
        void set_pot_pin(int _pot_pin);
        int get_finger_pos();
        void set_finger_stop(int stop_pos);
        void set_bounds(int _zero, int _max);
        void calibrate();
        void wiggle_servo();
};

#endif /* FINGER_H_ */
