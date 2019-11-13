#include <IRremote.h>
#include <Stepper.h>
#include <stdio.h>

const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
// for your motor

//Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
Stepper myStepper(stepsPerRevolution, 11, 9, 10, 8);

//IR remote controller 설정
int RECV_PIN = 2;
int stepmotor = 1024;
IRrecv irrecv(RECV_PIN);
decode_results results;

bool flag = false;

char ssid[] = "maker";      // your network SSID (name)
char pass[] = "maker1234";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) { ; } // wait for serial port connect.
  irrecv.enableIRIn(); // Start the receiver

  myStepper.setSpeed(50); // set the speed at 60 rpm:
  Serial.println(results.value, HEX);
}

void rotate(decode_results results, int motor){
  if(strcmp(results.value,"FF30CF")){
    Serial.print("IN!!");
    myStepper.step(motor);
    delay(1000);
  }
  else{
    Serial.print("OUT!!");
  }
     
}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)) {
    
    Serial.println(results.value, HEX);
    if(!flag) {
      rotate(results,stepmotor);
      flag = true;
    }
    else {
      rotate(results,-stepmotor);
      flag = false;
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
