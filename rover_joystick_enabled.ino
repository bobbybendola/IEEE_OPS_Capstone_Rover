#include <WiFiConfig.h>

//Driver left side
const int M1_IN1 = 8; // Connects to L293D Pin 2 (1A)
const int M1_IN2 = 10; // Connects to L293D Pin 7 (2A)
const int M1_EN  = 3; // New Speed Control Pin for Motor 1

// --- Motor 2 (Right Side) ---
const int M2_IN3 = 0; // Connects to L293D Pin 10 (3A)
const int M2_IN4 = 1; // Connects to L293D Pin 15 (4A)
const int M2_EN  = 4; // New Speed Control Pin for Motor 2


RemoteData data;



void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200); 
   Serial.println("--- Starting Rover Integration Test ---");
   pinMode(M1_IN1 , OUTPUT);
   pinMode(M1_IN2, OUTPUT);
   pinMode(M2_IN3, OUTPUT);
   pinMode(M2_IN4, OUTPUT);

   // Initialize ESP-NOW Configuration
  wifi_setup();
  set_data_receiver(&data);


  //start with stopped car 
   stop();
}

void loop() {
if (data.vy > 200) { 
    forward();
  }
  else if (data.vy < 50) { // Adjusted threshold for full backward stick
    reverse();
  }
  else if (data.vx < 50) { // Adjusted threshold for full right stick
    turnRight();
  }
  else if (data.vx > 200) { 
    turnLeft();
  }
  else {
    stop();
  }

  delay(50); // Delay for smoothness

}

void forward(){
  Serial.println("Driving Straight Forward...");
  analogWrite(M1_EN, 200); 
  analogWrite(M2_EN, 200); 
  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, LOW);   
  digitalWrite(M2_IN4, HIGH);  
}

void reverse(){
  Serial.println("Driving Straight Backward...");
  analogWrite(M1_EN, 200); 
  analogWrite(M2_EN, 200); 
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, HIGH);
  digitalWrite(M2_IN3, HIGH);   
  digitalWrite(M2_IN4, LOW);  
}

void turnRight(){
  Serial.println("Turning Right...");
  analogWrite(M1_EN, 150); 
  analogWrite(M2_EN, 150); 
  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, HIGH);  
  digitalWrite(M2_IN4, LOW);   
}

void turnLeft(){
  Serial.println("Turning Left...");
  analogWrite(M1_EN, 150); 
  analogWrite(M2_EN, 150); 
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, HIGH);
  digitalWrite(M2_IN3, LOW);   
  digitalWrite(M2_IN4, HIGH);  
}

void stop(){
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, LOW);
  digitalWrite(M2_IN4, LOW);
  analogWrite(M1_EN, 0);
  analogWrite(M2_EN, 0);
}