#include "IRremote.h"


// button puzzle initializations
int buttonPuzzleLedPin = 5;
int buttonApin = 9;
int buttonBpin = 8;
byte lastButtonState = LOW;

// ir puzzle initializations
int IRreceiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11
int IRoutput = 4;
IRrecv irrecv(IRreceiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
byte lastIRState = LOW;
String code[4] = {String("4"), String("9"), String("CH"), String("6")};
int codeSize = 4;
int IRcurrentIndex = 0;

// oprientation puzzle initializations
int OrReceiver = 12;
int OrOutput = 6;
byte lastOrState = HIGH;

void checkNextCodeElement(String elem) {
  if (code[IRcurrentIndex] == elem) {
    IRcurrentIndex++;
  } else {
    IRcurrentIndex = 0;
  }
}

byte translateIR() {
  switch(results.value){
  case 0xFFA25D: 
    checkNextCodeElement(String("CH")); 
    break;
  case 0xFFE21D:
    checkNextCodeElement(String("CH")); 
    break;
  case 0xFF629D: 
    checkNextCodeElement(String("CH")); 
    break;
  case 0xFF22DD:
    checkNextCodeElement(String("FAST BACK")); 
    break;
  case 0xFF02FD: 
    checkNextCodeElement(String("FAST FORWARD")); 
    break;
  case 0xFFC23D: 
    checkNextCodeElement(String("PAUSE")); 
    break;
  case 0xFFE01F:
    checkNextCodeElement(String("VOL-")); 
    break;
  case 0xFFA857:
    checkNextCodeElement(String("VOL+")); 
    break;
  case 0xFF906F: 
    checkNextCodeElement(String("EQ")); 
    break;
  case 0xFF9867:
    checkNextCodeElement(String("100+")); 
    break;
  case 0xFFB04F:
    checkNextCodeElement(String("200+")); 
    break;
  case 0xFF6897:
    checkNextCodeElement(String("0")); 
    return LOW;
  case 0xFF30CF: 
    checkNextCodeElement(String("1")); 
    break;
  case 0xFF18E7: 
    checkNextCodeElement(String("2")); 
    break;
  case 0xFF7A85: 
    checkNextCodeElement(String("3")); 
    break;
  case 0xFF10EF: 
    checkNextCodeElement(String("4")); 
    break;
  case 0xFF38C7: 
    checkNextCodeElement(String("5")); 
    break;
  case 0xFF5AA5: 
    checkNextCodeElement(String("6")); 
    break;
  case 0xFF42BD: 
    checkNextCodeElement(String("7")); 
    break;
  case 0xFF4AB5: 
    checkNextCodeElement(String("8")); 
    break;
  case 0xFF52AD: 
    checkNextCodeElement(String("9")); 
    break;
  case 0xFFFFFFFF:
    //Serial.println("miss");
    break; 
 }
 //Serial.println(IRcurrentIndex);
  if (IRcurrentIndex == codeSize) {
    //Serial.println("done");
    return HIGH;
  }
  return LOW;
}

void setup() 
{ 

  Serial.begin(9600);
  // button puzzle setup
  pinMode(buttonPuzzleLedPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  

  //ir puzzle setup
  irrecv.enableIRIn(); // Start the receiver
  pinMode(IRoutput, OUTPUT);
  digitalWrite(IRoutput, LOW);

  // or puzzle setup
  pinMode(OrOutput, OUTPUT);
  digitalWrite(OrOutput, LOW);
}

void loop() 
{
  delay(100);
  
  // get puzzle1 input
  byte currentButtonState = digitalRead(buttonPuzzleLedPin);

  // update button puzzle output
  if (digitalRead(buttonApin) == LOW) {
    digitalWrite(buttonPuzzleLedPin, HIGH);
  }
  if (digitalRead(buttonBpin) == LOW) {
    digitalWrite(buttonPuzzleLedPin, LOW);
  }
  
  // update state send puzzle1 state info to serial
  if(currentButtonState != lastButtonState) {
    lastButtonState = currentButtonState;
    if(currentButtonState == LOW) {
      Serial.println("p1_reset");
    } else {
      Serial.println("p1_complete");
    }
  }
  

  //get puzzle2 input
  byte currentIRState;
  if (irrecv.decode(&results)){
    currentIRState = translateIR();
    irrecv.resume(); // receive the next value 
  }

  // send puzzle2 state info to serial and update output
  if(currentIRState != lastIRState) {
    // update state
    lastIRState = currentIRState;

    // update output
    digitalWrite(IRoutput, currentIRState);
    
    // signal state update to serial
    if(currentIRState == LOW) {
      Serial.println("p2_reset");
    } else {
      Serial.println("p2_complete");
    }
  }

  // get puzzle3 input
  byte currentOrState = digitalRead(OrReceiver);
  // send puzzle3 state info to serial and update output  
  if(currentOrState != lastOrState) {
    // update state
    lastOrState = currentOrState;

    // update output
    digitalWrite(OrOutput, !currentOrState);
    
    // signal state update to serial
    if(!currentOrState == LOW) {
      Serial.println("p3_reset");
    } else {
      Serial.println("p3_complete");
    }
  }
  
  
}
