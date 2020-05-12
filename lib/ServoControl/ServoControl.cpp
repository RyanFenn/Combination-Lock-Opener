
#include <Servo.h>
#include <EEPROM.h>  
#include "ServoControl.h"
#include "Common.h"  

Servo servo; 

/*****************************************************************************/
/**
 * @brief   Initializes the servo and the servo position variable. The servo
 *          should be moved to the top potion.     
 */
/*****************************************************************************/
void ServoControl::init() {
    servo.attach(SIGNAL_PIN);
    reconfig();  
    moveBottomPosition();  
}

/*****************************************************************************/
/**
 * @brief   Similar to init(), but only reconfigures variables and/or states 
 *          that need to be. This function can be used any time after the 
 *          program has been initialized with the init() function. This
 *          function is intended to be used when retarting the program 
 *          without power-cycling.
 * @note    This function should be called once before the servo motor is
 *          moved.      
 */
/*****************************************************************************/
void ServoControl::reconfig() {
    _servoBottomPosition = EEPROM.read(SERVO_BOTTOM_POSITION_EEPROM_ADDRESS);
    _servoTopPosition = _servoBottomPosition - DISTANCE_BETWEEN_TOP_AND_BOTTOM_POSITIONS;   //note that top position should have a value that is lower than bottom position

    if(_servoBottomPosition > SERVO_BOTTOM_LIMIT || _servoTopPosition < SERVO_TOP_LIMIT) {   //this is the safe motion range (avoid servo motor stall) 
        _servoBottomPosition = DEFAULT_SERVO_BOTTOM_POSITION;   //this value may not be perfect, but it puts the servo motor in the general position so the servo doesn't hit motion limits and stall 
        _servoTopPosition = _servoBottomPosition - DISTANCE_BETWEEN_TOP_AND_BOTTOM_POSITIONS; 
        EEPROM.update(SERVO_BOTTOM_POSITION_EEPROM_ADDRESS, DEFAULT_SERVO_BOTTOM_POSITION); 
    }
}

/*****************************************************************************/
/**
 * @brief   Moves the servo motor to the top position. Keeps track of the
 *          servo position. 
 */
/*****************************************************************************/
void ServoControl::moveTopPosition() {
    _currentServoPosition = _servoTopPosition;     
    servo.write(_servoTopPosition);
}

/*****************************************************************************/
/**
 * @brief   Moves the servo motor to the bottom position. 
 */
/*****************************************************************************/
void ServoControl::moveBottomPosition() {
    _currentServoPosition = _servoBottomPosition;     
    servo.write(_servoBottomPosition); 
}

/*****************************************************************************/
/**
 * @brief   Moves the servo motor up by one increment. Keeps track of the
 *          servo position. 
 * @note    This function should be called after moveTopPosition or 
 *          moveBottomPosition because _currentServoPosition might not
 *          be in sync with the actual servo position. This function is 
 *          intended to be used for calibrating the servo motor.        
 */
/*****************************************************************************/
void ServoControl::moveUpOneIncrement() {
    _currentServoPosition -= SERVO_INCREMENT_DISTANCE;   //note that moving up means _currentServoPosition will decrease in value
    if(_currentServoPosition <= SERVO_TOP_LIMIT) {   //so that the servo motor doesn't move higher than the top limit 
        _currentServoPosition = SERVO_TOP_LIMIT; 
    }
    servo.write(_currentServoPosition); 
}

/*****************************************************************************/
/**
 * @brief   Moves the servo motor down by one increment. Keeps track of the 
 *          servo position. 
 * @note    This function should be called after moveTopPosition or 
 *          moveBottomPosition because _currentServoPosition might not
 *          be in sync with the actual servo position. This function is 
 *          intended to be used for calibrating the servo motor.     
 */
/*****************************************************************************/
void ServoControl::moveDownOneIncrement() {
    _currentServoPosition += SERVO_INCREMENT_DISTANCE;   //note that moving down means _currentServoPosition will increase in value
    if(_currentServoPosition >= SERVO_BOTTOM_LIMIT) {   //so that the servo motor doesn't move lower than the bottom limit 
        _currentServoPosition = SERVO_BOTTOM_LIMIT; 
    }
    servo.write(_currentServoPosition);    
}

/*****************************************************************************/
/**
 * @brief   Gets the current servo motor position.
 * @returns _currentServoPosition
 */
/*****************************************************************************/
unsigned char ServoControl::getCurrentPosition() {
    return _currentServoPosition; 
}
