#include <WiFiConfig.h>

// sensor pins 
const int trigPin = 7;
const int echoPin = 6;


//Driver left side
const int M1_IN1 = 8; // Connects to L293D Pin 2 (1A)
const int M1_IN2 = 10; // Connects to L293D Pin 7 (2A)
const int M1_EN  = 3; // New Speed Control Pin for Motor 1

// --- Motor 2 (Right Side) ---
const int M2_IN3 = 0; // Connects to L293D Pin 10 (3A)
const int M2_IN4 = 1; // Connects to L293D Pin 15 (4A)
const int M2_EN  = 4; // New Speed Control Pin for Motor 2


RemoteData data;
///Mode State Variables
bool autoMode = false;
bool lastSwState = HIGH; // Joystick button is HIGH when released (INPUT_PULLUP)

//auto mode
enum RoverState { DRIVING_FORWARD, OBSTACLE_STOP, BACKING_UP, TURNING };
RoverState currentState = DRIVING_FORWARD;
unsigned long stateStartTime = 0;



void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200); 
   Serial.println("--- Starting Rover Integration Test ---");
   pinMode(M1_IN1 , OUTPUT);
   pinMode(M1_IN2, OUTPUT);
   pinMode(M2_IN3, OUTPUT);
   pinMode(M2_IN4, OUTPUT);

   //sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

   // Initialize ESP-NOW Configuration
  wifi_setup();
  set_data_receiver(&data);


  //start with stopped car 
   stop();
}

void loop() {
  // check for Mode Toggle 
  if (data.sw == LOW && lastSwState == HIGH) {
    autoMode = !autoMode; // Toggle mode
    Serial.print("MODE CHANGED! Auto Mode is now: ");
    Serial.println(autoMode ? "ON (Autonomous)" : "OFF (Manual Joystick)");
    stop(); // Safely stop motors during mode transition
    delay(200); //  delay
  }
  lastSwState = data.sw; // Save state for next iteration

  // 2. Execute Mode Logic
  if (autoMode) {
    long distance = readDistance();
    Serial.print("Auto Mode -> Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 5 && distance < 20) { // Obstacle detected closer than 20cm
      Serial.println("Obstacle Alert!");
      stop();
      delay(300);
      reverse();
      turnRight();
      forward(); 
    } else {
      forward(); // Path is clear, move forward
    }
    delay(50); 
    
  } else {
    // --- MANUAL DRIVE MODE (Joystick) ---
    if (data.vy > 200) { 
      forward();
    }
    else if (data.vy < 50) { 
      reverse();
    }
    else if (data.vx < 50) { 
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
}

// --- Helper function to read Ultrasonic Sensor ---
long readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseIn with a 25000us timeout so it doesn't lock up the code if no echo is received
  long duration = pulseIn(echoPin, HIGH, 25000); 
  
  if (duration == 0) {
    return 999; // Return a large distance if sensor times out (clear path)
  }
  
  long distance = duration / 58.0; // Conversion to cm
  return distance;
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