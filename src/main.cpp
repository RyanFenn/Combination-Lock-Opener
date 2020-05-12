
#include <Arduino.h>
#include <EEPROM.h> 
#include "ServoControl.h"
#include "StepperControl.h"
#include "Display.h" 
#include "Algorithm.h"  

DisplayPage currentPage = DisplayPage::home;

//these position variables are assigned proper values when the program starts
char firstPosition = NO_POSITION_ASSIGNED;
char secondPosition = NO_POSITION_ASSIGNED; 
char thirdPosition = NO_POSITION_ASSIGNED; 

unsigned int attemptsCounter = 0; 
unsigned long startTimeMs = 0;   

StepperControl stepperControl; 
ServoControl servoControl; 
Algorithm algorithm;   
Display display; 

void setup() {
  servoControl.init(); 
  stepperControl.init(); 
  algorithm.init(&firstPosition, &secondPosition, &thirdPosition); 
  display.init();  
}

void loop() {
  if(currentPage == DisplayPage::home) {
    display.drawOnce_homePage(); 
    stepperControl.disableStepperMotor(); 
    currentPage = display.monitorInputs_homePage(); 
    if(currentPage == DisplayPage::runProgram1) {
      servoControl.reconfig(); 
      stepperControl.reconfig(); 
      algorithm.reconfig();  
      display.reconfig();  
      firstPosition = NO_POSITION_ASSIGNED;  
      secondPosition = NO_POSITION_ASSIGNED;   
      thirdPosition = NO_POSITION_ASSIGNED; 
      attemptsCounter = 0; 
    }
  }
  else if(currentPage == DisplayPage::runProgram1) {
    display.drawOnce_runProgramPage1();
    currentPage = display.monitorInputs_runProgramPage1(); 
    if(currentPage == DisplayPage::runProgram2) {      
      servoControl.moveBottomPosition();    
    } 
  }  
  else if(currentPage == DisplayPage::runProgram2) {
    display.drawOnce_runProgramPage2();
    currentPage = display.monitorInputs_runProgramPage2();  
    if(currentPage == DisplayPage::runProgram3) {    //(re)initializations that need to be done before the program (re)starts running go here        
      startTimeMs = millis(); 
      stepperControl.enableStepperMotor();   
    }
  }  
  else if(currentPage == DisplayPage::runProgram3) {
    AlgorithmState state = algorithm.run(&attemptsCounter);   
    if(state == AlgorithmState::complete) {
      currentPage = DisplayPage::results; 
    }
    else if(state == AlgorithmState::error) {
      currentPage = DisplayPage::error; 
    }
    else if(state == AlgorithmState::running) {
      display.drawOnce_runProgramPage3();
      display.drawOnce_updatedCombination(firstPosition, secondPosition, thirdPosition);  
      currentPage = display.monitorInputs_runProgramPage3();
    }
  }  
  else if(currentPage == DisplayPage::setup1) {
    display.drawOnce_setupPage1(); 
    currentPage = display.monitorInputs_setupPage1();
    if(currentPage == DisplayPage::setup2) {
      servoControl.reconfig(); 
      servoControl.moveBottomPosition(); 
    } 
  }
  else if(currentPage == DisplayPage::setup2) {
    display.drawOnce_setupPage2(); 
    currentPage = display.monitorInputs_setupPage2(); 
  }
  else if(currentPage == DisplayPage::setup3) {
    display.drawOnce_setupPage3(); 
    currentPage = display.monitorInputs_setupPage3(); 
  }  
  else if(currentPage == DisplayPage::setup4) {
    display.drawOnce_setupPage4(); 
    currentPage = display.monitorInputs_setupPage4(); 
  }
  else if(currentPage == DisplayPage::setup5) {
    display.drawOnce_setupPage5(); 
    currentPage = display.monitorInputs_setupPage5(); 
  }
  else if(currentPage == DisplayPage::setup6) {
    display.drawOnce_setupPage6(); 
    currentPage = display.monitorInputs_setupPage6(); 
  }
  else if(currentPage == DisplayPage::setup7) {
    display.drawOnce_setupPage7(); 
    currentPage = display.monitorInputs_setupPage7(); 
  }
  else if(currentPage == DisplayPage::setup8) {
    display.drawOnce_setupPage8(); 
    currentPage = display.monitorInputs_setupPage8(); 
  }
  else if(currentPage == DisplayPage::results) {
    display.drawOnce_resultsPage(firstPosition, secondPosition, thirdPosition, attemptsCounter, startTimeMs); 
    currentPage = display.monitorInputs_resultsPage(); 
  }
  else if(currentPage == DisplayPage::error) {
    display.drawOnce_errorPage(); 
    currentPage = display.monitorInputs_errorPage(); 
  }
}


