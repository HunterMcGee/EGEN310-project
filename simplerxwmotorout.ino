// SimpleRx - the slave or the receiver

#include <SPI.h> //necessary library
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   7 //establishing pins
#define CSN_PIN  6
#define MODE 2

int motor1a = 9; //pin numbers set
int motor1b = 10;
int motor2a = 3;
int motor2b = 5;

int i = 0;

int output1 = 4;
int output2 = 4; //variables from transmission

// pin 3 = 1a is phase for motor 1
// pin 5 =1b is enable for motor 1
// pin 9 = 2a is phase for motor 2
// pin 10 =2b is enable for motor 2


const byte thisSlaveAddress[5] = {'R','x','A','A','A'}; //radio frequency

RF24 radio(CE_PIN, CSN_PIN); //setting up the radio

int dataReceived; // this must match dataToSend in the TX
bool newData = false;

//===========

void setup() {

    pinMode(motor1a,OUTPUT) ; //setting up the motors
    pinMode(motor1b,OUTPUT) ;   
    pinMode(motor2a,OUTPUT) ;   
    pinMode(motor2b,OUTPUT) ;   //we have to set PWM pin as output
    pinMode(MODE, OUTPUT);
    //digitalWrite(MODE, HIGH);
    Serial.begin(9600);  //more radio establishing
    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

//=============

void loop() { //loop calling the functions
    getData();
    pushData();
}

//==============

void getData() { //transfering data
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void pushData() {//recieving from controller
    if (newData == true) {
        output1 = ((dataReceived/ 10U) % 10) - 1;
        output2 = dataReceived-((1+output1)*10);
        Serial.println(dataReceived);
        Serial.println(output1);
        Serial.println(output2);
        motorlogic(output1, output2);
        newData = false;
        return 0;
    }
}

void motorlogic(int x, int y){ //functions for motor control via controller
    
      if( x == 4){  //Steady state
      analogWrite(motor1a, 0);
      digitalWrite(motor1b, LOW);
      
    }
     
      if(y == 4){  //Steady state
      analogWrite(motor2a, 0);
      digitalWrite(motor2b, LOW);

    }
    
     if(x < 4){
      digitalWrite(motor1b, HIGH);     
      analogWrite(motor1a, x*40);
    }
      if(x > 4){
     
      digitalWrite(motor1b, LOW);
      analogWrite(motor1a, (x-4)*40);     
      }

     if(y < 4){
      digitalWrite(motor2b, HIGH);     
      analogWrite(motor2a, y*40);
    }
     if(y > 4){
      digitalWrite(motor2b, LOW);     
      analogWrite(motor2a, (y-4)*40);
}


return 0;
}




