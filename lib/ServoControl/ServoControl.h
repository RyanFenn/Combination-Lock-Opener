
#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H
 
#define DISTANCE_BETWEEN_TOP_AND_BOTTOM_POSITIONS 40  
#define SERVO_INCREMENT_DISTANCE 20  
#define SERVO_BOTTOM_LIMIT 160  
#define SERVO_TOP_LIMIT 80   
#define DEFAULT_SERVO_BOTTOM_POSITION 140 

#define SIGNAL_PIN 41   

class ServoControl {
public:
    void init(); 
    void reconfig(); 
    void moveTopPosition(); 
    void moveBottomPosition(); 
    void moveUpOneIncrement();
    void moveDownOneIncrement(); 
    unsigned char getCurrentPosition(); 

private:
    unsigned char _currentServoPosition; 
    unsigned char _servoBottomPosition;   //during early testing, bottom position was 140
    unsigned char _servoTopPosition;      //during early testing, top position was 100
};
extern ServoControl servoControl; 

#endif
