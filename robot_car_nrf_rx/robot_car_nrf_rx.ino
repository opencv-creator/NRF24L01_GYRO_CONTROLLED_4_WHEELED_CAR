#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

char text[32] = "";

#define IN1 6
#define IN2 5
#define IN3 4
#define IN4 3
const int FORWARD=1;
const int REVERSE=2;
const int LEFT= 3;
const int RIGHT =4;
const int STOP =0;
int lastCommand =0;
int led =9;


int motorSpeed = 150;

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void reverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turn_left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turn_right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stop_motors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  pinMode(led,OUTPUT);
   digitalWrite(led,HIGH);
   delay(3000);
   digitalWrite(led,LOW);
  //Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  delay(3000);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  stop_motors();
  //Serial.println("motor code initialized");
}

void loop() {
  if (radio.available()) {

   
     digitalWrite(led,LOW);
    
    radio.read(&text, sizeof(text));
    //Serial.println(text);
    lastCommand = atoi(text);
    if(lastCommand >0)
    {
      digitalWrite(led,HIGH);
    }
   
   // Serial.println(lastCommand);
  }

  switch (lastCommand) {
    case FORWARD:
      forward();
      break;
    case REVERSE:
      reverse();
      break;
    case LEFT:
      turn_left();
      break;
    case RIGHT:
      turn_right();
      break;
    case STOP:
      stop_motors();
      break;
  }
}