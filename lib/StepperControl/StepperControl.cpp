
#include <Arduino.h>  
#include "StepperControl.h"
#include "Common.h" 

/*****************************************************************************/
/**
 * @brief   Initializations are done here. This function should only be called
 *          once when the microcontroller boots (call in setup).
 */ 
/*****************************************************************************/
void StepperControl::init() {
    //Easy Driver pins
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(MS1_PIN, OUTPUT);
    pinMode(MS2_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT); 
    disableStepperMotor(); 
    reconfig(); 
}

/*****************************************************************************/
/**
 * @brief   Similar to init(), but only reconfigures variables and/or states 
 *          that need to be. This function can be used any time after the 
 *          program has been initialized with the init() function. This
 *          function is intended to be used when retarting the program 
 *          without power-cycling.  
 */
/*****************************************************************************/
void StepperControl::reconfig() {
    //Reset Easy Driver pins to default states
    digitalWrite(STEP_PIN, LOW);
    digitalWrite(DIR_PIN, HIGH); //clockwise
    digitalWrite(MS1_PIN, LOW);
    digitalWrite(MS2_PIN, LOW);

    _currentStepperCommand = StepperCommand::none;  
    _previousStepperCommand = StepperCommand::none; 
    _targetStepCount = 0; 
    _stepCounter = 0;    
    _currentStep = 0;   
}

/*****************************************************************************/
/**
 * @brief   Rotates clockwise until it gets to the first position. This
 *          function is designed to be synchronous, meaning that it is
 *          non-blocking and will be called many times before the stepper
 *          motor gets to the first position. Each time this function is 
 *          called, the stepper should move one step until it gets to the 
 *          target position. 
 * @note    Uses rotateOneStep() function.    
 * @param   targetFirstPosition    This is the target position. 
 * @returns Returns StepperState::incomplete or ::complete depending on 
 *          if the stepper motor has reached the target position. If 
 *          there is already another stepper motor command being
 *          executed synchronously, this function will return
 *          ::commandConflict
 */ 
/*****************************************************************************/
StepperState StepperControl::goToFirstPosition(char targetFirstPosition) {
    if(_currentStepperCommand == StepperCommand::none || _currentStepperCommand == StepperCommand::goToFirstPosition) {
        //converts dial position to step number (decimal numbers are truncated)
        unsigned int firstPositionStepNumber = map(targetFirstPosition, 0, NUMBER_OF_POSITIONS, 0, NUMBER_OF_STEPS);
        _previousStepperCommand = StepperCommand::goToFirstPosition;
        if(abs(firstPositionStepNumber - _currentStep) > 0) {   
            rotateOneStep(StepperDirection::clockwise);    
            _currentStepperCommand = StepperCommand::goToFirstPosition; 
            return StepperState::incomplete; 
        }
        else {
            _currentStepperCommand = StepperCommand::none; 
            return StepperState::complete; 
        }         
    }
    return StepperState::commandConflict;   //another stepper command is in the process of being executed (synchronously)
}

/*****************************************************************************/
/**
 * @brief   Rotates counterclockwise until it gets to the second position. 
 *          This function is designed to be synchronous, meaning that it is
 *          non-blocking and will be called many times before the stepper
 *          motor gets to the second position. Each time this function is 
 *          called, the stepper should move one step until it gets to the 
 *          target position.  
 * @note    Uses rotateOneStep() function.
 * @note    rotateCounterclockwiseOnce() should be called before 
 *          goToSecondPosition() is called (this will be done in 
 *          the Algorithm.cpp file).  
 * @param   targetSecondPosition   This is the target position. 
 * @returns Returns StepperState::incomplete or ::complete depending on 
 *          if the stepper motor has reached the target position. If 
 *          there is already another stepper motor command being
 *          executed synchronously, this function will return
 *          ::commandConflict     
 */ 
/*****************************************************************************/
StepperState StepperControl::goToSecondPosition(char targetSecondPosition) {
    if(_currentStepperCommand == StepperCommand::none || _currentStepperCommand == StepperCommand::goToSecondPosition) {
        //converts dial position to step number (decimal numbers are truncated)
        unsigned int secondPositionStepNumber = map(targetSecondPosition, 0, NUMBER_OF_POSITIONS, 0, NUMBER_OF_STEPS);
        _previousStepperCommand = StepperCommand::goToSecondPosition;
        if(abs(secondPositionStepNumber - _currentStep) > 0) {   
            rotateOneStep(StepperDirection::counterclockwise);    
            _currentStepperCommand = StepperCommand::goToSecondPosition; 
            return StepperState::incomplete; 
        }
        else {
            _currentStepperCommand = StepperCommand::none; 
            return StepperState::complete; 
        } 
    }
    return StepperState::commandConflict;   //another stepper command is in the process of being executed (synchronously)
}

/*****************************************************************************/
/**
 * @brief   Rotates clockwise until it gets to the third position. 
 *          This function is designed to be synchronous, meaning that it is
 *          non-blocking and will be called many times before the stepper
 *          motor gets to the third position. Each time this function is 
 *          called, the stepper should move one step until it gets to the 
 *          target position. 
 * @note    Uses rotateOneStep() function.
 * @param   targetThirdPosition   This is the target position.  
 * @returns Returns StepperState::incomplete or ::complete depending on 
 *          if the stepper motor has reached the target position. If 
 *          there is already another stepper motor command being
 *          executed synchronously, this function will return
 *          ::commandConflict
 */ 
/*****************************************************************************/
StepperState StepperControl::goToThirdPosition(char targetThirdPosition) {
    if(_currentStepperCommand == StepperCommand::none || _currentStepperCommand == StepperCommand::goToThirdPosition) {
        //converts dial position to step number (decimal numbers are truncated)
        unsigned int thirdPositionStepNumber = map(targetThirdPosition, 0, NUMBER_OF_POSITIONS, 0, NUMBER_OF_STEPS);
        _previousStepperCommand = StepperCommand::goToThirdPosition;
        if(abs(thirdPositionStepNumber - _currentStep) > 0) {   
            rotateOneStep(StepperDirection::clockwise);    
            _currentStepperCommand = StepperCommand::goToThirdPosition; 
            return StepperState::incomplete; 
        }
        else {
            _currentStepperCommand = StepperCommand::none; 
            return StepperState::complete; 
        } 
    }
    return StepperState::commandConflict;   //another stepper command is in the process of being executed (synchronously)
}

/*****************************************************************************/
/**
 * @brief   Resets the lock by rotating clockwise two full rotations. 
 *          This function is designed to be synchronous, meaning that it is
 *          non-blocking and will be called many times before the target
 *          step count is reached. Each time this function is called, the 
 *          stepper should move one step until it gets to the target 
 *          position.   
 * @note    Uses rotateOneStep() function.
 * @returns Returns StepperState::incomplete or ::complete depending on 
 *          if the stepper motor has reached the target position. If 
 *          there is already another stepper motor command being
 *          executed synchronously, this function will return
 *          ::commandConflict
 */ 
/*****************************************************************************/
StepperState StepperControl::rotateClockwiseTwice() {
    if(_currentStepperCommand == StepperCommand::none || _currentStepperCommand == StepperCommand::rotateClockwiseTwice) {   //only allows one command to be executed at a time
        if(_previousStepperCommand != StepperCommand::rotateClockwiseTwice) {   //if it is the first time this function has been called since the beginning of this synchronous execution
            _targetStepCount = 2*NUMBER_OF_STEPS;
            _stepCounter = 0; 
        }
        rotateOneStep(StepperDirection::clockwise); 
        _stepCounter++; 
        _previousStepperCommand = StepperCommand::rotateClockwiseTwice;
        if(_stepCounter < _targetStepCount) {
            _currentStepperCommand = StepperCommand::rotateClockwiseTwice; 
            return StepperState::incomplete;
        } 
        else if(_stepCounter == _targetStepCount) {
            _currentStepperCommand = StepperCommand::none; 
            return StepperState::complete;  
        }            
    }
    return StepperState::commandConflict;   //another stepper command is in the process of being executed (synchronously)
} 

/*****************************************************************************/
/**
 * @brief   Rotates counterclockwise one full rotation. This function is 
 *          designed to be synchronous, meaning that it is non-blocking and 
 *          will be called many times before the target step count is 
 *          reached. Each time this function is called, the stepper should 
 *          move one step until it gets to the target position. 
 * @note    Uses rotateOneStep() function.
 * @note    This function should be called before goToSecondPosition() is
 *          called (this will be done in the Algorithm.cpp file). 
 * @returns Returns StepperState::incomplete or ::complete depending on 
 *          if the stepper motor has reached the target position. If 
 *          there is already another stepper motor command being
 *          executed synchronously, this function will return
 *          ::commandConflict
 */ 
/*****************************************************************************/
StepperState StepperControl::rotateCounterclockwiseOnce() {
    if(_currentStepperCommand == StepperCommand::none || _currentStepperCommand == StepperCommand::rotateCounterclockwiseOnce) {   //only allows one command to be executed at a time
        if(_previousStepperCommand != StepperCommand::rotateCounterclockwiseOnce) {   //if it is the first time this function has been called since the beginning of this synchronous execution
            _targetStepCount = NUMBER_OF_STEPS;
            _stepCounter = 0; 
        }
        rotateOneStep(StepperDirection::counterclockwise); 
        _stepCounter++; 
        _previousStepperCommand = StepperCommand::rotateCounterclockwiseOnce;
        if(_stepCounter < _targetStepCount) {
            _currentStepperCommand = StepperCommand::rotateCounterclockwiseOnce; 
            return StepperState::incomplete;
        } 
        else if(_stepCounter == _targetStepCount) {
            _currentStepperCommand = StepperCommand::none; 
            return StepperState::complete;  
        }            
    }
    return StepperState::commandConflict;   //another stepper command is in the process of being executed (synchronously)
}

/*****************************************************************************/
/**
 * @brief   Rotates the stepper motor one step in the requested direction. 
 * @note    A small delay may be required to get the motor to step properly. 
 * @param   direction   The direction that the motor should turn.
 *          Options: 
 *          StepperDirection::clockwise, StepperDirection::counterclockwise
 */ 
/*****************************************************************************/
void StepperControl::rotateOneStep(StepperDirection direction) {
    if(direction == StepperDirection::clockwise) {
        digitalWrite(DIR_PIN, HIGH);   //clockwise
        _currentStep++; 
        if(_currentStep == NUMBER_OF_STEPS) {
            _currentStep = 0; 
        }
    }
    else if(direction == StepperDirection::counterclockwise) {
        digitalWrite(DIR_PIN, LOW);   //counterclockwise
        _currentStep--; 
        if(_currentStep < 0) {
            _currentStep = NUMBER_OF_STEPS - 1; 
        }
    }
    digitalWrite(STEP_PIN, HIGH);   //trigger one step
    delay(1);   
    digitalWrite(STEP_PIN, LOW);   //pull step pin low so it can be triggered again
    delay(1);   
} 

/*****************************************************************************/
/**
 * @brief   Enables the stepper motor so it can move and hold. 
 */ 
/*****************************************************************************/
void StepperControl::enableStepperMotor() {
    digitalWrite(EN_PIN, LOW);  //enabled
}

/*****************************************************************************/
/**
 * @brief   Disables the stepper motor, preventing it from moving or holding.
 *          When the stepper motor is disabled, the shaft can be rotated 
 *          freely. 
 */ 
/*****************************************************************************/
void StepperControl::disableStepperMotor() {
    digitalWrite(EN_PIN, HIGH);  //disable
}

