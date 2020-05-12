
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <EEPROM.h>

// custom fonts
#include <Fonts/FreeSans12pt7b.h> 
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans18pt7b.h> 
#include <Fonts/FreeSans9pt7b.h>

#include "Display.h"
#include "ServoControl.h"  
#include "Common.h"

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);   //X plus, Y plus, X minus, Y minus, resistance accross X plates

/*****************************************************************************/
/**
 * @brief   Initializations are done here. This function should only be called
 *          once when the microcontroller boots (call in setup).
 */
/*****************************************************************************/
void Display::init() {
    tft.reset();
    tft.begin(0x9341); 
    tft.setRotation(1);   // (options are 0, 1, 2, or 3) 
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
void Display::reconfig() {
    _previousPage = DisplayPage::notAssigned;

    //initialized to values that will never be equivalent to firstPosition, secondPosition, or thirdPosition (these variables will be assigned different values when the program is running)
    _previousFirstPosition = PREVIOUS_POSITION_INIT_VALUE; 
    _previousSecondPosition = PREVIOUS_POSITION_INIT_VALUE; 
    _previousThirdPosition = PREVIOUS_POSITION_INIT_VALUE;
}

/*****************************************************************************/
/**
 * @brief   Draws the display page. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_homePage() {
    if(_previousPage != DisplayPage::home) {   //so that the page is only drawn once  
        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);

        tft.setCursor(15,30);
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        printTextCentered("Combination Lock", 30);
        printTextCentered("Opener", 60);

        tft.drawFastHLine(0,75, 320, CUSTOM_GREEN);
        drawSetupButton(BUTTON_RELEASED); 
        drawRunProgramButton(BUTTON_RELEASED); 

        _previousPage = DisplayPage::home;
    }
}

/*****************************************************************************/
/**
 * @brief   Draws setup page 1. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage1() {   
    if(_previousPage != DisplayPage::setup1) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Setup", 40);
        drawBackButton(BUTTON_RELEASED);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b); 

        tft.setCursor(15,100);
        tft.print("Please remove the lock if it is");  
        tft.setCursor(15,122);
        tft.print("inserted. Press continue to move");
        tft.setCursor(15,144);
        tft.print("the shackle-puller to the bottom");
        tft.setCursor(15,166);
        tft.print("position.");   

        drawContinueButton(BUTTON_RELEASED); 

        _previousPage = DisplayPage::setup1; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws setup page 2. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage2() {
    if(_previousPage != DisplayPage::setup2) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Setup", 40);
        drawBackButton(BUTTON_RELEASED); 
        drawExitButton(BUTTON_RELEASED); 
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);
        tft.setCursor(15,100);   
        tft.print("Would you like to change");   
        tft.setCursor(15,122);
        tft.print("the saved value for the"); 
        tft.setCursor(15,144);
        tft.print("first zone's starting");  
        tft.setCursor(15,166);
        tft.print("position?");

        drawStandardBlueButton("Yes", BUTTON_RELEASED, 230, 85, 80);  
        drawStandardBlueButton("No", BUTTON_RELEASED, 230, 180, 80);

        _previousPage = DisplayPage::setup2; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws setup page 3. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage3() {
    if(_previousPage != DisplayPage::setup3) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Setup", 40);
        drawBackButton(BUTTON_RELEASED); 
        drawExitButton(BUTTON_RELEASED); 
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setCursor(15,100);
        tft.setFont(&FreeSans9pt7b); 
        tft.print("Select a zone starting position.");  

        drawStandardBlueButton("0", BUTTON_RELEASED, 52, 120);  
        drawStandardBlueButton("1", BUTTON_RELEASED, 107, 120);  
        drawStandardBlueButton("2", BUTTON_RELEASED, 162, 120);  
        drawStandardBlueButton("3", BUTTON_RELEASED, 217, 120); 
        drawStandardBlueButton("4", BUTTON_RELEASED, 52, 175);  
        drawStandardBlueButton("5", BUTTON_RELEASED, 107, 175); 
        
        _previousPage = DisplayPage::setup3; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws setup page 4. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage4() {
    if(_previousPage != DisplayPage::setup4) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Setup", 40);
        drawBackButton(BUTTON_RELEASED); 
        drawExitButton(BUTTON_RELEASED); 
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);
        tft.setCursor(15,100); 
        tft.print("Would you like to");    
        tft.setCursor(15,122);
        tft.print("recalibrate the servo"); 
        tft.setCursor(15,144);
        tft.print("motor? This may be");  
        tft.setCursor(15,166);
        tft.print("necessary if the pinion");
        tft.setCursor(15,188);
        tft.print("was detached");  

        drawStandardBlueButton("Yes", BUTTON_RELEASED, 230, 85, 80);  
        drawStandardBlueButton("No", BUTTON_RELEASED, 230, 180, 80);

        _previousPage = DisplayPage::setup4; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws setup page 5. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage5() {
    if(_previousPage != DisplayPage::setup5) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);

        printTextCentered("Setup", 40); 

        drawBackButton(BUTTON_RELEASED);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);
        tft.setCursor(15,100); 
        tft.print("Adjust the shackle-puller's");  
        tft.setCursor(15,122);
        tft.print("height if necessary and");
        tft.setCursor(15,144);
        tft.print("then press continue. The");
        tft.setCursor(15,166);
        tft.print("height will be saved. ");

        drawStandardBlueButton("Up", BUTTON_RELEASED, 260, 85);    
        drawStandardBlueButton("Dn", BUTTON_RELEASED, 260, 180);
        drawContinueButton(BUTTON_RELEASED); 

        _previousPage = DisplayPage::setup5; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws setup page 6. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage6() {
    if(_previousPage != DisplayPage::setup6) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Setup", 40);
        drawBackButton(BUTTON_RELEASED);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b); 

        tft.setCursor(15,100);
        tft.print("Please insert the lock.");  

        drawContinueButton(BUTTON_RELEASED);

        _previousPage = DisplayPage::setup6; 
    }
}


/*****************************************************************************/
/**
 * @brief   Draws setup page 7. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage7() {
    if(_previousPage != DisplayPage::setup7) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Setup", 40);
        drawBackButton(BUTTON_RELEASED);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);

        tft.setCursor(15,100); 
        tft.print("Turn the dial to the zero position. "); 

        drawContinueButton(BUTTON_RELEASED);

        _previousPage = DisplayPage::setup7; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws setup page 8. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_setupPage8() {
    if(_previousPage != DisplayPage::setup8) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Setup", 40);
        drawBackButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);
        printTextCentered("Setup is complete.", 100);

        drawMainMenuButton(BUTTON_RELEASED); 
        _previousPage = DisplayPage::setup8; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws run program page 1. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_runProgramPage1() {
    if(_previousPage != DisplayPage::runProgram1) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Run Program", 40);
        drawBackButton(BUTTON_RELEASED);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b); 

        tft.setCursor(15,100);
        tft.print("Please remove the lock if it is");  
        tft.setCursor(15,122);
        tft.print("inserted. Press continue to move");
        tft.setCursor(15,144);
        tft.print("the shackle-puller to the bottom");
        tft.setCursor(15,166);
        tft.print("position.");   

        drawContinueButton(BUTTON_RELEASED); 

        _previousPage = DisplayPage::runProgram1; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws run program page 2. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_runProgramPage2() {
    if(_previousPage != DisplayPage::runProgram2) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Run Program", 40);
        drawBackButton(BUTTON_RELEASED);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b); 

        tft.setCursor(15,100);
        tft.print("Please insert the lock. Press");    
        tft.setCursor(15,122);
        tft.print("continue to run the program.");

        drawContinueButton(BUTTON_RELEASED);

        _previousPage = DisplayPage::runProgram2; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws run program page 3. If this function is called repeatedly,
 *          the page will only be drawn once. The combination is not drawn 
 *          to the screen here, but the dashes between the combination
 *          numbers are drawn because they are not changing variables. 
 */
/*****************************************************************************/
void Display::drawOnce_runProgramPage3() {
    if(_previousPage != DisplayPage::runProgram3) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Run Program", 40);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);
        printTextCentered("Current combination:", 100);

        tft.setFont(&FreeSans18pt7b);

        tft.setCursor(124, 150); 
        tft.print("-"); 
        tft.setCursor(188, 150); 
        tft.print("-"); 

        _previousPage = DisplayPage::runProgram3; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws the updated combination if the value has changed. The dashes
 *          between the combination numbers will not be drawn here (drawn in
 *          drawOnce_runProgramPage3). 
 * @note    Since drawing to the tft display slows down the program, logic is 
 *          used to determine which numbers will be drawn. 
 * @param   firstPos    The first position that is to be printed to the display. 
 * @param   secondPos   The second position that is to be printed to the display. 
 * @param   thirdPos    The third position that is to be printed to the display. 
 */
/*****************************************************************************/
void Display::drawOnce_updatedCombination(char firstPos, char secondPos, char thirdPos) {
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0   
    tft.setFont(&FreeSans18pt7b);

    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(firstPos != _previousFirstPosition) {
        char firstPositionBuffer[2];
        sprintf(firstPositionBuffer, "%02d", firstPos); 
        tft.setCursor(78, 150);   
        tft.fillRect(78, 125, 38, 26, BLACK); 
        tft.print(firstPositionBuffer); 
        _previousFirstPosition = firstPos; 
    }
    if(secondPos != _previousSecondPosition) {
        char secondPositionBuffer[2];
        sprintf(secondPositionBuffer, "%02d", secondPos); 
        tft.setCursor(142, 150); 
        tft.fillRect(142, 125, 38, 26, BLACK); 
        tft.print(secondPositionBuffer);
        _previousSecondPosition = secondPos;   
    }
    if(thirdPos != _previousThirdPosition) {
        char thirdPositionBuffer[2];
        sprintf(thirdPositionBuffer, "%02d", thirdPos); 
        tft.setCursor(206, 150);  
        tft.fillRect(206, 125, 38, 26, BLACK); 
        tft.print(thirdPositionBuffer);  
        _previousThirdPosition = thirdPos;         
    }
}

/*****************************************************************************/
/**
 * @brief   Draws the results page. If this function is called repeatedly,
 *          the page will only be drawn once. 
 * @param   firstPos    The first position value that successfully opened the
 *          combination lock. This value is printed to the display. 
 * @param   secondPos   Same as firstPos, except for second position.
 * @param   thirdPos    Same as firstPos, except for third position.
 * @param   attemptNumber   The attempt number that successfully opened the
 *          combination lock. This value is printed to the display.  
 * @param   startTimeMillis The time (milliseconds) that the program started 
 *          running. This value is used to calculate the ellapsed time which
 *          is printed to the display. 
 */
/*****************************************************************************/
void Display::drawOnce_resultsPage(char firstPos, char secondPos, char thirdPos, unsigned int attemptNumber, unsigned long startTimeMillis) {
    if(_previousPage != DisplayPage::results) {  
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        tft.fillScreen(BLACK);
        printTextCentered("Results", 40);    
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);

        char positionsBuffer[40];   
        sprintf(positionsBuffer, "Successful combination : %02d-%02d-%02d", firstPos, secondPos, thirdPos); 
        tft.setCursor(15,100); 
        tft.print(positionsBuffer);

        unsigned long elapsedTimeMs = millis() - startTimeMillis;
        unsigned long allSeconds = elapsedTimeMs / 1000; 
        int elapsedHours = allSeconds / 3600; 
        int remainingSeconds = allSeconds % 3600; 
        int elapsedMinutes = remainingSeconds / 60; 
        int elapsedSeconds = remainingSeconds % 60; 
        char timeBuffer[30];   
        sprintf(timeBuffer, "Elapsed time : %02d:%02d:%02d", elapsedHours, elapsedMinutes, elapsedSeconds); 
        tft.setCursor(15,122);
        tft.print(timeBuffer); 

        //note that the second positon cannot be the same value as the first and third
        unsigned int maxAttempts = (NUMBER_OF_ZONES) * (NUMBER_OF_ZONES -1) * (NUMBER_OF_ZONES -1);   //10*9*9 = 810      
        char attemptsBuffer[40];   
        sprintf(attemptsBuffer, "Attempt number : %u out of %u", attemptNumber, maxAttempts);   
        tft.setCursor(15,144);
        tft.print(attemptsBuffer);  

        _previousPage = DisplayPage::results; 
    }
}

/*****************************************************************************/
/**
 * @brief   Draws the error page. If this function is called repeatedly,
 *          the page will only be drawn once. 
 */
/*****************************************************************************/
void Display::drawOnce_errorPage() {
    if(_previousPage != DisplayPage::error) {
        tft.setTextColor(WHITE);
        tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
        tft.setFont(&FreeSansBold12pt7b); 

        //This is important, because the libraries are sharing pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        
        tft.fillScreen(BLACK);
        printTextCentered("Error", 40);
        drawExitButton(BUTTON_RELEASED);
        tft.drawFastHLine(0,65, 320, CUSTOM_GREEN);

        tft.setFont(&FreeSans9pt7b);
        tft.setCursor(15,100); 
        tft.print("All combinations have been tried");  
        tft.setCursor(15,122);
        tft.print("without success. Possible fixes:"); 
        tft.setCursor(15,144);
        tft.print("  - inspect limit switch");
        tft.setCursor(15,166);
        tft.print("  - adjust first zone's starting");
        tft.setCursor(15,188);
        tft.print("    position if necessary");
        tft.setCursor(15,210);
        tft.print("  - recalibrate servo"); 

        _previousPage = DisplayPage::error; 
    }
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on the home page. 
 * @return  Returns the updated enumeration for the current page. 
 *          Options: 
 *          DisplayPage::home, DisplayPage::setup1, DisplayPage::runProgram1
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_homePage() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=60 && point.x<=260 && point.y>=100 && point.y<=150){    //setup button
            drawSetupButton(BUTTON_PRESSED);    
            while(ts.isTouching());   //wait until the button is released before continuing  
            return DisplayPage::setup1;
        } 
        else if(point.x>=60 && point.x<=260 && point.y>=170 && point.y<=220){   //run program button
            drawRunProgramButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::runProgram1;          
        }  
    } 
    return DisplayPage::home;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 1. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage1() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button
            drawBackButton(BUTTON_PRESSED);   
            while(ts.isTouching());   //wait until the button is released before continuing 
            return DisplayPage::home;
        } 
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){   //exit button
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::home;         
        }
        else if(point.x>=45 && point.x<=195 && point.y>=180 && point.y<=230){    //continue button 
            drawContinueButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::setup2;                    
        }
    } 
    return DisplayPage::setup1;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 2. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage2() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button
            drawBackButton(BUTTON_PRESSED);   
            while(ts.isTouching());   //wait until the button is released before continuing 
            return DisplayPage::setup1;
        } 
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){   //exit button
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::home;         
        }
        else if(point.x>=45 && point.x<=195 && point.y>=180 && point.y<=230){    //continue button 
            drawContinueButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::setup3;                    
        }
        else if(point.x>=230 && point.x<=310 && point.y>=85 && point.y<=135){    // yes button    
            drawStandardBlueButton("Yes", BUTTON_PRESSED, 230, 85, 80);  
            while(ts.isTouching()); 
            return DisplayPage::setup3;      
        }     
        else if(point.x>=230 && point.x<=310 && point.y>=180 && point.y<=230){    // no button    
            drawStandardBlueButton("No", BUTTON_PRESSED, 230, 180, 80); 
            while(ts.isTouching());            
            return DisplayPage::setup4; 
        } 
    } 
    return DisplayPage::setup2; 
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 3. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage3() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240);
        
        char firstZoneValue;   //for the first zone's center position  
        
        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button      
            drawBackButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::setup2; 
        }
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){    //exit button   
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());   
            return DisplayPage::home; 
        }   
        else if(point.x>=52 && point.x<=102 && point.y>=120 && point.y<=170){    // 0 button 

            firstZoneValue = 0 + CENTER_OFFSET; 
            if(firstZoneValue >= ZONE_OFFSET) firstZoneValue -= ZONE_OFFSET;  //to make sure the first zone is the lowest possible value
            EEPROM.update(FIRST_ZONE_EEPROM_ADDRESS, firstZoneValue);   //only writes to eeprom if the value is different  
            drawStandardBlueButton("0", BUTTON_PRESSED, 52, 120);    
            while(ts.isTouching());      
            return DisplayPage::setup4; 
        }     
        else if(point.x>=107 && point.x<=157 && point.y>=120 && point.y<=170){    // 1 button   
            firstZoneValue = 1 + CENTER_OFFSET;  
            if(firstZoneValue >= ZONE_OFFSET) firstZoneValue -= ZONE_OFFSET;  
            EEPROM.update(FIRST_ZONE_EEPROM_ADDRESS, firstZoneValue);     
            drawStandardBlueButton("1", BUTTON_PRESSED, 107, 120);    
            while(ts.isTouching());            
            return DisplayPage::setup4;
        }    
        else if(point.x>=162 && point.x<=212 && point.y>=120 && point.y<=170){    // 2 button 
            firstZoneValue = 2 + CENTER_OFFSET; 
            if(firstZoneValue >= ZONE_OFFSET) firstZoneValue -= ZONE_OFFSET;  
            EEPROM.update(FIRST_ZONE_EEPROM_ADDRESS, firstZoneValue); 
            drawStandardBlueButton("2", BUTTON_PRESSED, 162, 120);    
            while(ts.isTouching());               
            return DisplayPage::setup4;
        }  
        else if(point.x>=217 && point.x<=267 && point.y>=120 && point.y<=170){    // 3 button  
            firstZoneValue = 3 + CENTER_OFFSET; 
            if(firstZoneValue >= ZONE_OFFSET) firstZoneValue -= ZONE_OFFSET;  
            EEPROM.update(FIRST_ZONE_EEPROM_ADDRESS, firstZoneValue);     
            drawStandardBlueButton("3", BUTTON_PRESSED, 217, 120);    
            while(ts.isTouching());             
            return DisplayPage::setup4;
        }    
        else if(point.x>=52 && point.x<=102 && point.y>=175 && point.y<=225){    // 4 button  
            firstZoneValue = 4 + CENTER_OFFSET; 
            if(firstZoneValue >= ZONE_OFFSET) firstZoneValue -= ZONE_OFFSET; 
            EEPROM.update(FIRST_ZONE_EEPROM_ADDRESS, firstZoneValue);     
            drawStandardBlueButton("4", BUTTON_PRESSED, 52, 175);    
            while(ts.isTouching());             
            return DisplayPage::setup4;
        } 
        else if(point.x>=107 && point.x<=157 && point.y>=175 && point.y<=225){    // 5 button
            firstZoneValue = 5 + CENTER_OFFSET; 
            if(firstZoneValue >= ZONE_OFFSET) firstZoneValue -= ZONE_OFFSET;  
            EEPROM.update(FIRST_ZONE_EEPROM_ADDRESS, firstZoneValue);
            drawStandardBlueButton("5", BUTTON_PRESSED, 107, 175);    
            while(ts.isTouching());                
            return DisplayPage::setup4;
        }  
    }
    return DisplayPage::setup3; 
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 4. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage4() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240);

        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button          
            drawBackButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::setup3; 
        }
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){    //exit button   
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());   
            return DisplayPage::home; 
        }     
        else if(point.x>=230 && point.x<=310 && point.y>=85 && point.y<=135){    // yes button    
            drawStandardBlueButton("Yes", BUTTON_PRESSED, 230, 85, 80);  
            while(ts.isTouching());      
            return DisplayPage::setup5;
        }     
        else if(point.x>=230 && point.x<=310 && point.y>=180 && point.y<=230){    // no button    
            drawStandardBlueButton("No", BUTTON_PRESSED, 230, 180, 80); 
            while(ts.isTouching());            
            return DisplayPage::setup6; 
        }   
    }
    return DisplayPage::setup4;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 5. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage5() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240);

        if(point.x>10 && point.x<60 && point.y>10 && point.y<50){    //back button      
            drawBackButton(BUTTON_PRESSED); 
            while(ts.isTouching());    
            return DisplayPage::setup4;
        }
        else if(point.x>260 && point.x<310 && point.y>10 && point.y<50){    //exit button   
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::home;
        }
        else if(point.x>45 && point.x<195 && point.y>180 && point.y<230){    //continue button  
            drawContinueButton(BUTTON_PRESSED); 
            while(ts.isTouching());    
            unsigned char currentServoPos = servoControl.getCurrentPosition();   
            EEPROM.update(SERVO_BOTTOM_POSITION_EEPROM_ADDRESS, currentServoPos);   //only writes to eeprom if the value is different    
            return DisplayPage::setup6; 
        }             
        else if(point.x>260 && point.x<310 && point.y>85 && point.y<135){   //up button
            drawStandardBlueButton("Up", BUTTON_PRESSED, 260, 85);   
            while(ts.isTouching());      
            drawStandardBlueButton("Up", BUTTON_RELEASED, 260, 85);   
            servoControl.moveUpOneIncrement(); 
        }    
        else if(point.x>260 && point.x<310 && point.y>180 && point.y<230){   //down button
            drawStandardBlueButton("Dn", BUTTON_PRESSED, 260, 180 );
            while(ts.isTouching());   
            drawStandardBlueButton("Dn", BUTTON_RELEASED, 260, 180 );    
            servoControl.moveDownOneIncrement();  
        } 
    }
    return DisplayPage::setup5;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 6. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage6() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240);

        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button   
            drawBackButton(BUTTON_PRESSED); 
            while(ts.isTouching());        
            return DisplayPage::setup5; 
        }
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){    //exit button   
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::home; 
        }
        else if(point.x>=45 && point.x<=195 && point.y>=180 && point.y<=230){    //continue button 
            drawContinueButton(BUTTON_PRESSED); 
            while(ts.isTouching());           
            return DisplayPage::setup7; 
        } 
    }
    return DisplayPage::setup6;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 7. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage7() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240);

        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button 
            drawBackButton(BUTTON_PRESSED); 
            while(ts.isTouching());          
            return DisplayPage::setup6; 
        }
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){    //exit button   
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::home;
        }
        else if(point.x>=45 && point.x<=195 && point.y>=180 && point.y<=230){    //continue button  
            drawContinueButton(BUTTON_PRESSED); 
            while(ts.isTouching());          
            return DisplayPage::setup8; 
        }    
    }
    return DisplayPage::setup7;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on setup page 8. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_setupPage8() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240);

        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button 
            drawBackButton(BUTTON_PRESSED); 
            while(ts.isTouching());          
            return DisplayPage::setup7;
        }
        else if(point.x>=50 && point.x<=270 && point.y>=180 && point.y<=230){    //main menu button   
            drawMainMenuButton(BUTTON_PRESSED);  
            while(ts.isTouching());       
            return DisplayPage::home;
        } 
    }
    return DisplayPage::setup8;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on 
 *          run program page 1. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_runProgramPage1() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button
            drawBackButton(BUTTON_PRESSED);   
            while(ts.isTouching());   //wait until the button is released before continuing 
            return DisplayPage::home;
        } 
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){   //exit button
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::home;         
        }
        else if(point.x>=45 && point.x<=195 && point.y>=180 && point.y<=230){    //continue button 
            drawContinueButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::runProgram2;                    
        }
    } 
    return DisplayPage::runProgram1;   
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on 
 *          run program page 2. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_runProgramPage2() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=10 && point.x<=60 && point.y>=10 && point.y<=50){    //back button
            drawBackButton(BUTTON_PRESSED);   
            while(ts.isTouching());   //wait until the button is released before continuing 
            return DisplayPage::runProgram1;
        } 
        else if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){   //exit button
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::home;         
        }
        else if(point.x>=45 && point.x<=195 && point.y>=180 && point.y<=230){    //continue button 
            drawContinueButton(BUTTON_PRESSED); 
            while(ts.isTouching());     
            return DisplayPage::runProgram3;                    
        }
    } 
    return DisplayPage::runProgram2;
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on 
 *          run program page 3. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_runProgramPage3() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){   //exit button
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::home;         
        }
    } 
    return DisplayPage::runProgram3;   
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on the
 *          results page. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_resultsPage() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){   //exit button
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::home;         
        }
    } 
    return DisplayPage::results; 
}

/*****************************************************************************/
/**
 * @brief   Checks to see if any buttons have been pressed on the
 *          error page. 
 * @return  Returns the updated enumeration for the current page, depending
 *          on whether or not a button has been pressed. If no button has  
 *          been pressed, return the same page.
 */
/*****************************************************************************/
DisplayPage Display::monitorInputs_errorPage() {
    TSPoint point = ts.getPoint();  //Get touch point  
    if (point.z > ts.pressureThreshhold){ 
        point.x = map(point.x, TS_MINX, TS_MAXX, 0, 320);   
        point.y = map(point.y, TS_MINY, TS_MAXY, 0, 240); 
        if(point.x>=260 && point.x<=310 && point.y>=10 && point.y<=50){   //exit button
            drawExitButton(BUTTON_PRESSED); 
            while(ts.isTouching());  
            return DisplayPage::home;         
        }
    } 
    return DisplayPage::error; 
}

/*****************************************************************************/
/**
 * @brief   Prints texted so that it is centered on the screen. Similar to
 *          tft.print(). The X coordinate is calculated in this function.
 * @note    tft.setTextColor(), tft.setTextSize(), and tft.setFont() need
 *          to be called before this function, just like when 
 *          using tft.print().  
 * @note    This function was tested for FreeSans12pt7b font. Other fonts 
 *          may not be centered if they are used. 
 * @param   inputText   This is the text that will be printed to the display. 
 * @param   y   This is the Y coordinate at which the text will be printed. 
 */
/*****************************************************************************/
void Display::printTextCentered(const char inputText[], unsigned int y) {

    int16_t  x1, y1;
    uint16_t w, h, cursorPosition;

    //all we really need from this function is the width of the text
    tft.getTextBounds(inputText, 0, y, &x1, &y1, &w, &h); // x coordinate set to 0, but doesn't really matter because it will be calculated to center the text

    cursorPosition = (320/2) - (w/2);   //calculating the cursor position so that the text will be centered on the screen
    cursorPosition = cursorPosition - 2;  //offsetting X position by -2 seems to center the text better (at least for FreeSans12pt7b font)

    tft.setCursor(cursorPosition, y); 
    tft.print(inputText);
}

/*****************************************************************************/
/**
 * @brief   Draws the setup button. The button will have different colours
 *          depending on whether it is pressed or released.     
 * @param   buttonState The state of the button is passed into this function.
 *          Options: BUTTON_RELEASED, BUTTON_PRESSED       
 */
/*****************************************************************************/
void Display::drawSetupButton(bool buttonState) {
    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(buttonState == BUTTON_RELEASED) {
        tft.fillRoundRect(60, 100, 200, 50, 10, CUSTOM_GREEN);   
        tft.drawRoundRect(60, 100, 200, 50, 10, WHITE);   
    }
    else if(buttonState == BUTTON_PRESSED) {
        tft.fillRoundRect(60, 100, 200, 50, 10, BLACK);   
        tft.drawRoundRect(60, 100, 200, 50, 10, CUSTOM_GREEN);    
    }      
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
    tft.setFont(&FreeSans12pt7b);           
    printTextCentered("Setup", 132);
} 

/*****************************************************************************/
/**
 * @brief   Draws the run program button. The button will have different 
 *          colours depending on whether it is pressed or released.       
 * @param   buttonState The state of the button is passed into this function.
 *          Options: BUTTON_RELEASED, BUTTON_PRESSED       
 */
/*****************************************************************************/
void Display::drawRunProgramButton(bool buttonState) {
    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(buttonState == BUTTON_RELEASED) {
        tft.fillRoundRect(60, 170, 200, 50, 10, CUSTOM_GREEN);   
        tft.drawRoundRect(60, 170, 200, 50, 10, WHITE);   
    }
    else if(buttonState == BUTTON_PRESSED) {
        tft.fillRoundRect(60, 170, 200, 50, 10, BLACK);   
        tft.drawRoundRect(60, 170, 200, 50, 10, CUSTOM_GREEN);    
    }      
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
    tft.setFont(&FreeSans12pt7b);           
    printTextCentered("Run Program", 202);
}

/*****************************************************************************/
/**
 * @brief   Draws the back button. The button will have different colours
 *          depending on whether it is pressed or released.       
 * @param   buttonState The state of the button is passed into this function.
 *          Options: BUTTON_RELEASED, BUTTON_PRESSED       
 */
/*****************************************************************************/
void Display::drawBackButton(bool buttonState) {
    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(buttonState == BUTTON_RELEASED) {
        tft.fillRoundRect(10, 10, 50, 40, 10, CUSTOM_GREEN);  
        tft.drawRoundRect(10, 10, 50, 40, 10, WHITE); 
    }
    else if(buttonState == BUTTON_PRESSED) {
        tft.fillRoundRect(10, 10, 50, 40, 10, BLACK); 
        tft.drawRoundRect(10, 10, 50, 40, 10, CUSTOM_GREEN);    
    }          
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
    tft.setCursor(19,38);
    tft.setFont(&FreeSans18pt7b);                 
    tft.print("<-");
}

/*****************************************************************************/
/**
 * @brief   Draws the exit button. The button will have different colours
 *          depending on whether it is pressed or released.     
 * @param   buttonState The state of the button is passed into this function.
 *          Options: BUTTON_RELEASED, BUTTON_PRESSED       
 */
/*****************************************************************************/
void Display::drawExitButton(bool buttonState) {
    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(buttonState == BUTTON_RELEASED) {
        tft.fillRoundRect(260, 10, 50, 40, 10, RED);   
        tft.drawRoundRect(260, 10, 50, 40, 10, WHITE);  
    }
    else if(buttonState == BUTTON_PRESSED) {
        tft.fillRoundRect(260, 10, 50, 40, 10, BLACK);   
        tft.drawRoundRect(260, 10, 50, 40, 10, RED);     
    }      
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
    tft.setCursor(276, 39);  
    tft.setFont(&FreeSans18pt7b);                 
    tft.print("x");
}

/*****************************************************************************/
/**
 * @brief   Draws the continue button. The button will have different colours
 *          depending on whether it is pressed or released.       
 * @param   buttonState The state of the button is passed into this function.
 *          Options: BUTTON_RELEASED, BUTTON_PRESSED       
 */
/*****************************************************************************/
void Display::drawContinueButton(bool buttonState) {
    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(buttonState == BUTTON_RELEASED) {
        tft.fillRoundRect(45, 180, 150, 50, 10, CUSTOM_GREEN);   
        tft.drawRoundRect(45, 180, 150, 50, 10, WHITE);   
    }
    else if(buttonState == BUTTON_PRESSED) {
        tft.fillRoundRect(45, 180, 150, 50, 10, BLACK);   
        tft.drawRoundRect(45, 180, 150, 50, 10, CUSTOM_GREEN);    
    }      
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
    tft.setCursor(72,212);  //X cursor postion = (rect X coordinate + (rect width/2) - text width/2) -2    //Y cursor position = rect Y coordinate + 32    //formulas work for FreeSans12pt7b   //text width found with printTextInfoToSerial function
    tft.setFont(&FreeSans12pt7b);           
    tft.print("Continue");   //text width = 92 pixels    
}

/*****************************************************************************/
/**
 * @brief   Draws a standard blue button. The button will have different colours
 *          depending on whether it is pressed or released. 
 * @param   inputText   This is the text that will be printed as the button's
 *          label.        
 * @param   buttonState The state of the button is passed into this function.
 *          Options: BUTTON_RELEASED, BUTTON_PRESSED      
 * @param   rectX   The X coordinate that determines where the left side of the
 *          button is located on the display. 
 * @param   rectY   The Y coordinate that determines where the top of the button
 *          is located on the display.
 * @param   rectWidth   Determines the width of the button (in pixels). This
 *          parameter has a default value of 50 (look in header file) and 
 *          does not need to be specified.  
 */
/*****************************************************************************/
void Display::drawStandardBlueButton(const char inputText[], bool buttonState, int16_t rectX, int16_t rectY, int16_t rectWidth) {
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
    tft.setFont(&FreeSans12pt7b);   

    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    int16_t  x1, y1;
    uint16_t textWidth, textHeight;       
    tft.getTextBounds(inputText, 0, 0, &x1, &y1, &textWidth, &textHeight); 

    if(buttonState == BUTTON_RELEASED) {
        tft.fillRoundRect(rectX, rectY, rectWidth, 50, 10, BLUE);  
        tft.drawRoundRect(rectX, rectY, rectWidth, 50, 10, WHITE);   
    }
    else if(buttonState == BUTTON_PRESSED) {
        tft.fillRoundRect(rectX, rectY, rectWidth, 50, 10, BLACK); 
        tft.drawRoundRect(rectX, rectY, rectWidth, 50, 10, BLUE);    
    }  
    tft.setCursor((rectX + rectWidth/2 - textWidth/2) -2, rectY + 32);  //this line sets the cursor position with the correct offset to center the text with the button (works for FreeSans12pt7b only)                       
    tft.print(inputText);     
}

/*****************************************************************************/
/**
 * @brief   Draws the main menu button. The button will have different colours
 *          depending on whether it is pressed or released.       
 * @param   buttonState The state of the button is passed into this function.
 *          Options: BUTTON_RELEASED, BUTTON_PRESSED       
 */
/*****************************************************************************/
void Display::drawMainMenuButton(bool buttonState) {
    //This is important, because the libraries are sharing pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(buttonState == BUTTON_RELEASED) {
        tft.fillRoundRect(50, 180, 220, 50, 10, CUSTOM_GREEN);   
        tft.drawRoundRect(50, 180, 220, 50, 10, WHITE);   
    }
    else if(buttonState == BUTTON_PRESSED) {
        tft.fillRoundRect(50, 180, 220, 50, 10, BLACK);   
        tft.drawRoundRect(50, 180, 220, 50, 10, CUSTOM_GREEN);    
    }      
    tft.setTextColor(WHITE);
    tft.setTextSize(0); // Set text size. We are using custom font so you should always set text size as 0  
    tft.setFont(&FreeSans12pt7b);           
    printTextCentered("Go To Main Menu", 212);
}



