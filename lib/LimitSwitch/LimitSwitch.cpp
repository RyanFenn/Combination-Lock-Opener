
#include <Arduino.h>
#include "LimitSwitch.h"

/*****************************************************************************/
/**
 * @brief   Sets the pin as an input.    
 */
/*****************************************************************************/
void LimitSwitch::init() {
    pinMode(LIMIT_SWITCH_PIN, INPUT); 
}

/*****************************************************************************/
/**
 * @brief   Gets the state of the limit switch.
 * @returns Returns LIMIT_SWITCH_RELEASED or LIMIT_SWITCH_ACTIVATED
 */
/*****************************************************************************/
bool LimitSwitch::getState() {
    return digitalRead(LIMIT_SWITCH_PIN);
}

