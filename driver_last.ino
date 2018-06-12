#include <SoftwareSerial.h>
//#include "Bluetooth.h"

const int buttonPin = 12;
const int ledPin = 8;

SoftwareSerial BTSerial(4, 5);

int inData;
int oldButtonState = LOW;
int x = 1;

int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

boolean isSent = true;
boolean sending = true;


int oldData = 'a';

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(8, OUTPUT);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // Get the current state of the button
  //int newButtonState = digitalRead(buttonPin);

  int ledAble = 0;

  //블루투스 모듈 -> 아두이노 -> 시리얼 모니터
  if (BTSerial.available()) { //데이터 수신 대기
    inData = BTSerial.read(); // 수신 데이터 읽기
    if (inData == 'o') {
      ledAble = 1;
      digitalWrite(ledPin, 1);
      sending = true;
    }
    if (inData == 'x') {
      ledAble = 0;
      digitalWrite(ledPin, 0);
      buttonPushCounter = 0;
      sending = false;

    }

    Serial.write(inData);
  }

  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      if (buttonPushCounter == 0) {
        Serial.print("buttonPushed");
      }

      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      digitalWrite(ledPin, 1);
      Serial.println(buttonPushCounter);

      sending = true;
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }

    
    // Delay a little bit to avoid bouncing
    delay(100);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  if(sending){
      BTSerial.write('o');
   }else{
      BTSerial.write('n');
      //Serial.println("i am sending n \n");
   }



  //시리얼 모니터 -> 아두이노 -> 브루투스 모둘
  if (Serial.available()) { //시니얼 모니터 확인
    char transmitchar = Serial.read(); //시리얼 모니터로 데이터 읽기

    //    digitalWrite(ledPin, 0);
    //    Serial.write("IN!");
    //BTSerial.write(transmitchar); //블루투스로 데이터 전송

    if (transmitchar == 'x') {
      digitalWrite(ledPin, 0);
      Serial.write("x!");
      buttonPushCounter = 0;
    } else if (transmitchar == 'o') {
      digitalWrite(ledPin, 1);
      Serial.write("o!");
      buttonPushCounter++;
    }
  }

  delay(1000);
}
