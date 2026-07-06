#include <WiFiConfig.h>
// Handling Car JoyStick
const int VRX = 1; // Horizontal axis
const int VRY = 0; // Vertical axis
const int SW = 3;  // Joystick switch button
const int no_move= 8;
const int move= 10;

//ESP-NOW Communication
uint8_t receiverAddress[] = {0xa8, 0x46, 0x74, 0x5c, 0x1a, 0xc4};  //a8:46:74:5c:1a:c4
RemoteData data; 


void setup() {
  Serial.begin(115200); 
  wifi_setup(); 
  peer_setup(receiverAddress);  
  set_data_receiver(&data);

  pinMode(SW, INPUT_PULLUP); // Using internal pull-up resistor
  pinMode(VRY, INPUT);
  pinMode(VRX, INPUT);
  pinMode(move, OUTPUT); 
  pinMode(no_move, OUTPUT); 
}

void loop() {
  // Read analog values (0 to 4095 for ESP32)
  int xVal = analogRead(VRX);
  int yVal = analogRead(VRY);
  
  //converting to uint8_t type so that tranmission works with the WiFi Module
  uint8_t mapX = map(xVal, 0, 4095, 0, 255);
  uint8_t mapY = map(yVal, 0, 4095, 0, 255);

  // Read digital switch value LOW when pressed, HIGH when released
  int switchVal = digitalRead(SW);

  //transmitting RemoteData
  data.vx= mapX;
  data.vy= mapY;
  data.sw=switchVal; 

  //sending data to receiver
   send_data(receiverAddress, data);

  digitalWrite(move, LOW);
  digitalWrite(no_move,LOW);  
if(mapY< 150) 
	{ 
		Serial.println("Move Backward"); 
    digitalWrite(move, HIGH); 
	}

else if(mapY> 200) 
	{ 
		Serial.println("Move Forward"); 
    digitalWrite(move, HIGH); 
	}

else if(mapX< 150) 
	{ 
		Serial.println("Move Right"); 
    digitalWrite(move, HIGH); 
  }
else if(mapX> 200) 
	{ 
		Serial.println("Move Left"); 
    digitalWrite(move, HIGH); 
  }

else {Serial.println("NO MOVE");digitalWrite(no_move, HIGH); }

  Serial.println(switchVal); 
  delay(100); // Small delay to make the monitor readable
}