//Driver left side
const int M1_IN1 = 8; // Connects to L293D Pin 2 (1A)
const int M1_IN2 = 10; // Connects to L293D Pin 7 (2A)
const int M1_EN  = 3; // New Speed Control Pin for Motor 1

// --- Motor 2 (Right Side) ---
const int M2_IN3 = 0; // Connects to L293D Pin 10 (3A)
const int M2_IN4 = 1; // Connects to L293D Pin 15 (4A)
const int M2_EN  = 4; // New Speed Control Pin for Motor 2

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200); 
    Serial.println("--- Starting Rover Integration Test ---");
   pinMode(M1_IN1 , OUTPUT);
   pinMode(M1_IN2, OUTPUT);
   pinMode(M2_IN3, OUTPUT);
   pinMode(M2_IN4, OUTPUT);
// Initialize with all gates shut (Stopped)
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, LOW);
  digitalWrite(M2_IN4, LOW);
}

void loop() {
// 1. Move Straight Forward at 80% Speed
  Serial.println("Driving Straight Forward...");
  analogWrite(M1_EN, 200); 
  analogWrite(M2_EN, 200); 
  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, LOW);   
  digitalWrite(M2_IN4, HIGH);  
  delay(2000);

  // 2. Stop 
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, LOW);
  digitalWrite(M2_IN4, LOW);
  delay(1000);

  // 3. SOFTWARE FIX: Controlled Right Turn at ~40% Speed
  // Dropping the PWM prevents the wheels from losing traction and spinning out!
  Serial.println("Smoothly Turning Right...");
  analogWrite(M1_EN, 105); 
  analogWrite(M2_EN, 105); 
  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, HIGH);  
  digitalWrite(M2_IN4, LOW);   
  delay(800); // Shorter delay since it turns accurately now

  // 4. Stop before repeating
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN3, LOW);
  digitalWrite(M2_IN4, LOW);
  delay(2000);
}
