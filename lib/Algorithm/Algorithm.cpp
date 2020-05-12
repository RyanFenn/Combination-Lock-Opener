
#include <Ticker.h>  //https://github.com/sstaub/Ticker  (version 3.1.5 was used)
#include <EEPROM.h> 
#include "Algorithm.h"
#include "LimitSwitch.h"
#include "StepperControl.h"
#include "ServoControl.h"
#include "Common.h"

bool Algorithm::_isServoUpTimeLimitReached;  //this is a static variable (Ticker needs this variable to be static)
bool Algorithm::_isServoDownTimeLimitReached;  //this is a static variable (Ticker needs this variable to be static)

LimitSwitch limitSwitch; 
Ticker servoUpTimer(Algorithm::handleServoUpTimeLimit, SERVO_UP_WAIT_TIME_MS, 1, MILLIS); 
Ticker servoDownTimer(Algorithm::handleServoDownTimeLimit, SERVO_DOWN_WAIT_TIME_MS, 1, MILLIS); 

/*****************************************************************************/
/**
 * @brief   Initializations are done here. This function should only be called
 *          once when the microcontroller boots (call in setup). Gives class
 *          scope to the first, second, and third position variables. 
 * @param pFirstPosition  Points to the firstPosition (in main file)
 * @param pSecondPosition Points to the secondPosition (in main file)
 * @param pThirdPosition  Points to the thirdPosition (in main file)
 */
/*****************************************************************************/
void Algorithm::init(char* pFirstPosition, char* pSecondPosition, char* pThirdPosition) {  
  _pFirstPosition = pFirstPosition;
  _pSecondPosition = pSecondPosition;
  _pThirdPosition = pThirdPosition;  
  limitSwitch.init();     
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
void Algorithm::reconfig() {
    _currentCommand = AlgorithmCommand::setNextValidCombination;
    _previousCommand = AlgorithmCommand::none; 
    Algorithm::_isServoUpTimeLimitReached = false;
    Algorithm::_isServoDownTimeLimitReached = false; 
    _firstZone = EEPROM.read(FIRST_ZONE_EEPROM_ADDRESS);  
    servoUpTimer.stop(); 
    servoDownTimer.stop();     
}

/*****************************************************************************/
/**
 * @brief   This is a callback function for the servoUpTimer object. When this
 *          function is called, _isServoDownTimeLimitReached is assigned true.   
 */
/*****************************************************************************/
void Algorithm::handleServoUpTimeLimit() {
  Algorithm::_isServoUpTimeLimitReached = true;    
}

/*****************************************************************************/
/**
 * @brief   This is a callback function for the servoDownTimer object. When 
 *          this function is called, _isServoDownTimeLimitReached is assigned
 *          true.   
 */
/*****************************************************************************/
void Algorithm::handleServoDownTimeLimit() {
  Algorithm::_isServoDownTimeLimitReached = true;    
}

/*****************************************************************************/
/**
 * @brief   Runs an algorithm that tries to open a combination lock by trying
 *          every possible combination until it finds the correct one. The
 *          servo motor and the stepper motor are controlled here. The
 *          limit switch is used to detect when the lock is opened. 
 * @note    The algorithm should only be run if the limit switch is released. 
 * @param   ptrFirstPos The first position is passed in so that it can be
 *          assigned a new value and updated to the main file when necessary. 
 * @param   ptrSecondPos    The second position is passed in so that it can
 *          be assigned a new value and updated to the main file when 
 *          necessary. 
 * @param   ptrThirdPos The third position is passed in so that it can be
 *          assigned a new value and updated to the main file when necessary.  
 * @param   ptrAttemptsCounter  The attempts counter is passed in so that it
 *          can be assigned a new value and updated to the main file when 
 *          necessary. 
 * @returns Returns the current state of the algorithm. 
 *          Options: 
 *          AlgorithmState::running -> the algorithm is still running 
 *          AlgorithmState::error -> all combinations have been tried
 *              without successfully opening the combination lock
 *          AlgorithmState::complete -> the combination lock has been
 *              opened (limit swith activated). 
 */
/*****************************************************************************/
AlgorithmState Algorithm::run(unsigned int* pAttemptsCounter) {   
  switch(_currentCommand) {
    case AlgorithmCommand::setNextValidCombination:   
      if(setNextValidCombination() == ALL_COMBINATIONS_TRIED) {
        return AlgorithmState::error; 
      }
      else {
        //Detects when the dial needs to be reset (rotateClockwiseTwice). This will happen when the program has just started and when the third position rolls over to _firstZone value.
        //The reason there is an OR condition is because when *_pSecondPosition is equal to _firstZone, *_pThirdPosition will skip _firstZone. The OR condition makes sure that the 
        //rollover is still detected. 
        if((*_pThirdPosition == _firstZone) || (*_pSecondPosition == _firstZone && *_pThirdPosition == (_firstZone + ZONE_OFFSET))) {   
          _currentCommand = AlgorithmCommand::rotateClockwiseTwice; 
        }
        else {
          _currentCommand = AlgorithmCommand::goToThirdPosition; 
        }
      }
      _previousCommand = AlgorithmCommand::setNextValidCombination;  
      break;

    case AlgorithmCommand::rotateClockwiseTwice:    
      if(stepperControl.rotateClockwiseTwice() == StepperState::complete) {    
        _currentCommand = AlgorithmCommand::goToFirstPosition;  
      }
      _previousCommand = AlgorithmCommand::rotateClockwiseTwice;
      break;

    case AlgorithmCommand::goToFirstPosition:           
      if(stepperControl.goToFirstPosition(*_pFirstPosition) == StepperState::complete) {
        _currentCommand = AlgorithmCommand::rotateCounterclockwiseOnce;   
      }
      _previousCommand = AlgorithmCommand::goToFirstPosition; 
      break;

    case AlgorithmCommand::rotateCounterclockwiseOnce:    
      if(stepperControl.rotateCounterclockwiseOnce() == StepperState::complete) {
        _currentCommand = AlgorithmCommand::goToSecondPosition;   
      }
      _previousCommand = AlgorithmCommand::rotateCounterclockwiseOnce; 
      break;

    case AlgorithmCommand::goToSecondPosition:
      if(stepperControl.goToSecondPosition(*_pSecondPosition) == StepperState::complete) {
        _currentCommand = AlgorithmCommand::goToThirdPosition;   
      }
      _previousCommand = AlgorithmCommand::goToSecondPosition;  
      break;
      
    case AlgorithmCommand::goToThirdPosition:         
      if(stepperControl.goToThirdPosition(*_pThirdPosition) == StepperState::complete) {
        _currentCommand = AlgorithmCommand::servoUp;      
      }
      _previousCommand = AlgorithmCommand::goToThirdPosition; 
      break;

    case AlgorithmCommand::servoUp:
      servoUpTimer.update(); 
      if(_previousCommand == AlgorithmCommand::goToThirdPosition) {
        (*pAttemptsCounter)++; 
        servoUpTimer.start(); 
        servoControl.moveTopPosition(); 
      }
      else if(Algorithm::_isServoUpTimeLimitReached == true && _previousCommand == AlgorithmCommand::servoUp) {
        Algorithm::_isServoUpTimeLimitReached = false; 
        servoUpTimer.stop(); 
        _currentCommand = AlgorithmCommand::servoDown; 
      }       
      if(limitSwitch.getState() == LIMIT_SWITCH_ACTIVATED) {
        servoUpTimer.stop(); 
        return AlgorithmState::complete; 
      } 
      _previousCommand = AlgorithmCommand::servoUp; 
      break;

    case AlgorithmCommand::servoDown: 
      servoDownTimer.update(); 
      if(_previousCommand == AlgorithmCommand::servoUp) {
        servoDownTimer.start(); 
        servoControl.moveBottomPosition(); 
      }
      else if(Algorithm::_isServoDownTimeLimitReached == true && _previousCommand == AlgorithmCommand::servoDown) {
        Algorithm::_isServoDownTimeLimitReached = false;
        servoDownTimer.stop(); 
        _currentCommand = AlgorithmCommand::setNextValidCombination; 
      }
      _previousCommand = AlgorithmCommand::servoDown; 
      break; 
  }    
  return AlgorithmState::running;  
}

/*****************************************************************************/
/**
 * @brief Sets the next valid combination sequence. If all combinations have 
 *        been tried, do not increment anymore and keep the values for the 
 *        first, second and third positions set to the last valid combination
 *        sequence.
 * @note  This function uses *_pFirstPosition, *_pSecondPosition, and
 *        *_pThirdPosition to modify first, second, and third position
 *        variables found in the main file. 
 * @returns   Returns ALL_COMBINATIONS_TRIED or NEW_COMBINATION_SET 
 */
/*****************************************************************************/
bool Algorithm::setNextValidCombination() {
  while(1) {
    if(*_pFirstPosition == NO_POSITION_ASSIGNED) {
      *_pFirstPosition = _firstZone;  
      *_pSecondPosition = _firstZone; 
      *_pThirdPosition = _firstZone; 
    }
    else {
      *_pThirdPosition += ZONE_OFFSET; 
      if(*_pThirdPosition >= NUMBER_OF_POSITIONS) {
        *_pThirdPosition = _firstZone; 
        *_pSecondPosition += ZONE_OFFSET;
        if(*_pSecondPosition >= NUMBER_OF_POSITIONS) {
          *_pSecondPosition = _firstZone; 
          *_pFirstPosition += ZONE_OFFSET; 
          if(*_pFirstPosition >= NUMBER_OF_POSITIONS) {
            //assigns the last valid combination (remember that first/third positions cannot be the same as the second position) so that values don't keep incrementing
            *_pFirstPosition = _firstZone + ((NUMBER_OF_ZONES - 1)*(ZONE_OFFSET));    
            *_pSecondPosition = _firstZone + ((NUMBER_OF_ZONES - 2)*(ZONE_OFFSET));  
            *_pThirdPosition = _firstZone + ((NUMBER_OF_ZONES - 1)*(ZONE_OFFSET));
            return ALL_COMBINATIONS_TRIED; 
          }
        }
      }
      if(*_pFirstPosition != *_pSecondPosition && *_pSecondPosition != *_pThirdPosition) {
        return NEW_COMBINATION_SET;
      }  
    } 
  } 
}



