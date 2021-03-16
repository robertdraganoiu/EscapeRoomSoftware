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

// oprientation puzzle initializations
int OrReceiver = 12;
int OrOutput = 6;
byte lastOrState = HIGH;

byte translateIR() {
  switch(results.value){
  case 0xFF6897: return LOW; // button 0
  case 0xFF38C7: return HIGH; // button 5 
  }
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
