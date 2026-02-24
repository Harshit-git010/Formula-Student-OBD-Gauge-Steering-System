#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define segment pins (A to G and DP if needed)
#define A 14
#define B 15
#define C 16
#define D 17
#define E 18
#define F 19
#define G 20

// Create instances of the OLED displays
#define OLED1_ADDR 0x3C  // Address for first OLED (Speed and RPM)
#define OLED2_ADDR 0x3D  // Address for second OLED (Coolant and Gear)

Adafruit_SH1106G display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SH1106G display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int CAN_CS_PIN = 10; //pin used for 10 CAN CS
MCP_CAN CAN(CAN_CS_PIN);

int speed = 0;

void setup() {
 Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);



  // Initialize CAN bus
  if (CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN bus initialized");
  } else {
    Serial.println("CAN bus failed to initialize");
    while (1);
  }

   // Initialize OLED1
  if(!display1.begin(OLED1_ADDR)) {
  
    for(;;);
  }
  display1.clearDisplay();
  display1.setTextSize(4);
  display1.setTextColor(SH110X_WHITE);
  display1.setCursor(0,0);
  display1.print("Speed ");
  display2.setCursor(20,0);
  display1.print("RPM"); 
  display1.display();

  // Initialize OLED2
  if(!display2.begin(OLED2_ADDR)) {
    
    for(;;);
  }
  display2.clearDisplay();
  display2.setTextSize(3);
  display2.setTextColor(SH110X_WHITE);
  display2.setCursor(0,0);
  display2.print("Coolant Temp");
  display2.display();

delay(2000);
}

void clearDisplay() {
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

// display neutral 

void display0() {
 clearDisplay();
  digitalWrite(E, HIGH); 
  digitalWrite(G, HIGH);
  digitalWrite(C, HIGH);
}


// Function to display the number '1'
void display91() {
  clearDisplay();  // Clear all segments first
  digitalWrite(B, HIGH);  // Turn on segments B
  digitalWrite(C, HIGH);  // Turn on segments C
}

// Function to display the number '2'
void display92() {
  clearDisplay();
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(D, HIGH);
}

// Function to display the number '3'
void display93() {
  clearDisplay();
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
}

// Function to display the number '4'
void display94() {
  clearDisplay();
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
}

// Function to display the number '5'
void display95() {
  clearDisplay();
  digitalWrite(A, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
}

// Function to display the number '6'
void display96() {
  clearDisplay();
  digitalWrite(A, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
}


void loop() {

  speed = analogRead(A0) / 10;

  unsigned long canId;
  unsigned char len = 0;
  unsigned char buf[8];



  // Check if a CAN message is available
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&canId, &len, buf);

    // Example CAN IDs for RPM, coolant temp, gear position

    if (canId == 0x7DF) {  // RPM CAN ID 0x7EA 0x7EF
      int rpm = (buf[0] << 8) | buf[1];  // Combine two bytes for RPM
      display1.clearDisplay();
      display1.setCursor(30,0);
      display1.print(speed);
      display1.print("  ");
      display1.setCursor(10,30);
      display1.print(rpm);
      display1.display();

Serial.print(rpm);

      digitalWrite(13, rpm > 3000 ? HIGH : LOW);
      digitalWrite(12, rpm > 4000 ? HIGH : LOW);
      digitalWrite(11, rpm > 5000 ? HIGH : LOW);
      digitalWrite(9, rpm > 6000 ? HIGH : LOW);
      digitalWrite(8, rpm > 7000 ? HIGH : LOW);
      digitalWrite(7, rpm > 8000 ? HIGH : LOW);
      digitalWrite(6, rpm > 9000 ? HIGH : LOW);
      digitalWrite(5, rpm > 10000 ? HIGH : LOW);
      digitalWrite(4, rpm > 11000 ? HIGH : LOW);
    } 

   if (canId == 0x7E8) {  // Coolant Temp CAN ID
      int coolantTemp = buf[0];  // Single byte for temperature
      display2.clearDisplay();
      display2.setTextSize(7);
      display2.setCursor(0,10);
      display2.print(coolantTemp);
      display2.display();
   
Serial.print(coolantTemp);
   }

   if (canId == 0x7E9) { // Gear position CAN ID
      unsigned char data = buf[0];

switch (data) {
        case 1:
          display91();
          break;
        case 2:
          display92();
          break;
        case 3:
          display93();
          break;
        case 4:
          display94();
          break;
        case 5:
          display95();
          break;
        case 6:
          display96();
          break;
        default:
          display0(); // display neutral if no valid number is received
          break;

Serial.print(data);



   delay(50);
      } 
    }
  }
}

//try this code if this code didn't work

/*

#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define CAN_CS_PIN 10
#define CAN_INT_PIN 2  // Interrupt pin for MCP2515

// Define segment pins (A to G)
#define A 14
#define B 15
#define C 16
#define D 17
#define E 18
#define F 19
#define G 20

// OLED addresses
#define OLED1_ADDR 0x3C
#define OLED2_ADDR 0x3D

Adafruit_SH1106G display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SH1106G display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MCP_CAN CAN(CAN_CS_PIN);  // MCP2515 CAN controller
volatile bool messageAvailable = false;  // Flag to check if a CAN message is received

void setup() {
  Serial.begin(9600);
  
  // Set up pins for the 7-segment display and LEDs
  pinMode(A, OUTPUT); pinMode(B, OUTPUT); pinMode(C, OUTPUT);
  pinMode(D, OUTPUT); pinMode(E, OUTPUT); pinMode(F, OUTPUT); pinMode(G, OUTPUT);

  // Set up CAN interrupt pin
  pinMode(CAN_INT_PIN, INPUT);

  // Initialize CAN bus
  if (CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN bus initialized");
  } else {
    Serial.println("CAN bus failed to initialize");
    while (1);
  }

  // Attach interrupt for CAN message reception
  attachInterrupt(digitalPinToInterrupt(CAN_INT_PIN), canMessageReceived, FALLING);

  // Initialize OLED1
  if (!display1.begin(OLED1_ADDR)) {
    Serial.println("OLED1 initialization failed");
    for (;;) {}  // Hang if failed
  }
  display1.clearDisplay();
  display1.setTextSize(4);
  display1.setTextColor(SH110X_WHITE);
  display1.setCursor(0, 0);
  display1.print("Speed ");
  display1.setCursor(20, 30);
  display1.print("RPM");
  display1.display();

  // Initialize OLED2
  if (!display2.begin(OLED2_ADDR)) {
    Serial.println("OLED2 initialization failed");
    for (;;) {}  // Hang if failed
  }
  display2.clearDisplay();
  display2.setTextSize(3);
  display2.setTextColor(SH110X_WHITE);
  display2.setCursor(0, 0);
  display2.print("Coolant Temp");
  display2.display();
}

void loop() {
  if (messageAvailable) {
    messageAvailable = false;  // Reset the flag after message is read

    unsigned long canId;
    unsigned char len = 0;
    unsigned char buf[8];

    CAN.readMsgBuf(&canId, &len, buf);

    if (canId == 0x7E8) {  // RPM CAN ID (example)
      int rpm = (buf[0] << 8) | buf[1];  // Decode RPM from two bytes
      display1.clearDisplay();
      display1.setCursor(30, 0);
      display1.print(rpm);
      display1.display();
    }

    if (canId == 0x7E9) {  // Coolant Temp CAN ID
      int coolantTemp = buf[0];  // Single byte for temperature
      display2.clearDisplay();
      display2.setCursor(0, 10);
      display2.print(coolantTemp);
      display2.display();
    }
  }
}

void canMessageReceived() {
  messageAvailable = true;  // Set flag when a CAN message is available
}
/*