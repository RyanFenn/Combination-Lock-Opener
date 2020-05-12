
#ifndef ALGORITHM_H
#define ALGORITHM_H

#define SERVO_UP_WAIT_TIME_MS 500  
#define SERVO_DOWN_WAIT_TIME_MS 300  

#define NO_POSITION_ASSIGNED -1

#define ALL_COMBINATIONS_TRIED 0
#define NEW_COMBINATION_SET 1

enum class AlgorithmState { 
    running, 
    error, 
    complete 
};

enum class AlgorithmCommand {
    none, 
    setNextValidCombination,
    rotateClockwiseTwice,
    goToThirdPosition,
    rotateCounterclockwiseOnce,
    goToSecondPosition,
    goToFirstPosition,
    servoUp,
    servoDown
};

class Algorithm {
public:
    void init(char* pFirstPosition, char* pSecondPosition, char* pThirdPosition);
    void reconfig(); 
    static void handleServoUpTimeLimit(); 
    static void handleServoDownTimeLimit(); 
    AlgorithmState run(unsigned int* pAttemptsCounter); 

private:
    bool setNextValidCombination(); 
    AlgorithmCommand _currentCommand; 
    AlgorithmCommand _previousCommand; 
    static bool _isServoUpTimeLimitReached;
    static bool _isServoDownTimeLimitReached; 
    char* _pFirstPosition; 
    char* _pSecondPosition;
    char* _pThirdPosition; 
    char _firstZone; 
}; 

#endif
