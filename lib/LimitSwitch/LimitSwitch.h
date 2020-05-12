
#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#define LIMIT_SWITCH_PIN 38  

#define LIMIT_SWITCH_RELEASED 0
#define LIMIT_SWITCH_ACTIVATED 1

class LimitSwitch {
public:
    void init();
    bool getState(); 
};

#endif
