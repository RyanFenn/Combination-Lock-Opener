
#ifndef DISPLAY_H
#define DISPLAY_H

//Either ARDUINO_MEGA_ENV or CUSTOM_BOARD_ENV will be defined in the platformio.ini file, depending on which environment is being used
#ifdef ARDUINO_MEGA_ENV
  #define LCD_CS A3   
  #define LCD_CD A2   
  #define LCD_WR A1    
  #define LCD_RD A0  
  #define LCD_RESET A4      

  //for touch screen
  #define YP A2   //same as LCD_CD pin     //needs to be an analog pin
  #define XM A3   //same as LCD_CS pin     //needs to be an analog pin
  #define YM 8    //same as LCD_D0 pin     //digital pin 
  #define XP 9    //same as LCD_D1 pin     //digital pin  

#elif CUSTOM_BOARD_ENV
  #define LCD_CS A10    
  #define LCD_CD A9    
  #define LCD_WR 34   
  #define LCD_RD 35   
  #define LCD_RESET 39    

  //for touch screen 
  #define YP A9   //same as LCD_CD pin     //needs to be an analog pin
  #define XM A10  //same as LCD_CS pin     //needs to be an analog pin
  #define YM 23   //same as LCD_D0 pin     //digital pin 
  #define XP 22   //SAME as LCD_D1 pin     //digital pin 

#endif

#define TS_MINX 128
#define TS_MINY 110
#define TS_MAXX 952
#define TS_MAXY 910

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define CUSTOM_GREEN 0x05EC

#define BUTTON_RELEASED 0
#define BUTTON_PRESSED 1

//initialized to values that will never be equivalent to firstPosition, secondPosition, or thirdPosition (these variables will be assigned different values when the program is running)
#define PREVIOUS_POSITION_INIT_VALUE 99

enum class DisplayPage { 
    notAssigned,
    home, 
    results, 
    error, 
    setup1, 
    setup2, 
    setup3, 
    setup4,
    setup5,
    setup6,
    setup7,
    setup8,
    runProgram1, 
    runProgram2, 
    runProgram3
};   

class Display {
public:
    void init(); 
    void reconfig(); 

    void drawOnce_homePage(); 
    void drawOnce_setupPage1(); 
    void drawOnce_setupPage2();
    void drawOnce_setupPage3();
    void drawOnce_setupPage4();
    void drawOnce_setupPage5();
    void drawOnce_setupPage6();
    void drawOnce_setupPage7();
    void drawOnce_setupPage8();

    void drawOnce_runProgramPage1();
    void drawOnce_runProgramPage2();
    void drawOnce_runProgramPage3();
    void drawOnce_updatedCombination(char firstPos, char secondPos, char thirdPos); 

    void drawOnce_resultsPage(char firstPos, char secondPos, char thirdPos, unsigned int attemptNumber, unsigned long startTimeMillis); 
    void drawOnce_errorPage(); 

    DisplayPage monitorInputs_homePage();
    DisplayPage monitorInputs_setupPage1();
    DisplayPage monitorInputs_setupPage2();
    DisplayPage monitorInputs_setupPage3();
    DisplayPage monitorInputs_setupPage4();
    DisplayPage monitorInputs_setupPage5();
    DisplayPage monitorInputs_setupPage6();
    DisplayPage monitorInputs_setupPage7();
    DisplayPage monitorInputs_setupPage8();

    DisplayPage monitorInputs_runProgramPage1();
    DisplayPage monitorInputs_runProgramPage2();
    DisplayPage monitorInputs_runProgramPage3();

    DisplayPage monitorInputs_resultsPage(); 
    DisplayPage monitorInputs_errorPage(); 

private:
    void printTextCentered(const char inputText[], unsigned int y); 

    void drawSetupButton(bool buttonState); 
    void drawRunProgramButton(bool buttonState); 
    void drawBackButton(bool buttonState); 
    void drawExitButton(bool buttonState); 
    void drawContinueButton(bool buttonState);    
    void drawStandardBlueButton(const char inputText[], bool buttonState, int16_t rectX, int16_t rectY, int16_t rectWidth = 50); 
    void drawMainMenuButton(bool buttonState); 

    DisplayPage _previousPage;
    char _previousFirstPosition, _previousSecondPosition, _previousThirdPosition; 
};  

#endif

