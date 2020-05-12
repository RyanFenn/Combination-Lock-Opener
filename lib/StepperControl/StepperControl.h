
#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

//to use STEP and DIR pins, the MegaCore needs to be used, and the "Arduino MEGA pinout" needs to be used
#define STEP_PIN 79
#define DIR_PIN 78
#define MS1_PIN 12
#define MS2_PIN 10
#define EN_PIN  11

#define NUMBER_OF_STEPS 200

enum class StepperDirection { clockwise, counterclockwise };
enum class StepperState {
    incomplete,
    complete, 
    commandConflict   //another stepper command is in the process of being executed (synchronously)
}; 

enum class StepperCommand {
    none,
    rotateClockwiseTwice,
    goToThirdPosition,
    rotateCounterclockwiseOnce,
    goToSecondPosition,
    goToFirstPosition,  
};

class StepperControl {
public: 
    void init(); 
    void reconfig(); 
    StepperState goToFirstPosition(char targetFirstPosition); 
    StepperState goToSecondPosition(char targetSecondPosition);
    StepperState goToThirdPosition(char targetThirdPosition); 
    StepperState rotateClockwiseTwice(); 
    StepperState rotateCounterclockwiseOnce(); 
    void enableStepperMotor();
    void disableStepperMotor();  

private:
    void rotateOneStep(StepperDirection direction); 
    StepperCommand _currentStepperCommand;
    StepperCommand _previousStepperCommand; 
    int _currentStep; 
    int _targetStepCount; 
    int _stepCounter; 
};
extern StepperControl stepperControl; 

#endif

