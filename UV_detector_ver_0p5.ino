//Include all needed libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define the number of averages and Arduino's ADC resolution
int AverageNum=10000;
int bitsResolution=14;
//********************************************************************
// No real need to change code below for routine work
//********************************************************************

// Define OLED dimensions and I2C address
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Initialize display with I2C address 0x3C (or 0x3D)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define button used for blanking and its initial state
const int buttonPin = 10;
int buttonState = 0; 

//Define several variable to store data
unsigned long ADCoutput;
unsigned long ADCref;
float currentTime;

//Setup block. Executed once at power-up
void setup() {
  // Establish serial data transfer over USB and set the transfer rate in bits
    Serial.begin(115200);
  // Initialize the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  //Initialize a pin to monitor Blank signals
  pinMode(buttonPin, INPUT);
  delay(1000);
  pinMode(buttonPin, INPUT_PULLUP);

  //set Arduino's ADC resolution 
  analogReadResolution(bitsResolution);

  // Display a start-up message
  display.clearDisplay();
  display.setTextSize(2);             // Font size 2
  display.setTextColor(SSD1306_WHITE); // White text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("UV detect."));
  display.println(F("ver. 0.5"));
  display.display();
  delay(1000); // Wait 1 second
}

//Main loop. Runs continuously
void loop() {

// Real analog input to collect data from amplified photodiode
  ADCoutput=0;
    for (int i=0; i<AverageNum; i++) {
      ADCoutput+=analogRead(A0);
    }
  // Read state on Blank button. If pressed, update voltage  
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    ADCref = ADCoutput;
  }
  //int sensorValue = analogRead(A2);
  double transmission = (double)ADCoutput / ADCref;
  double sensorValue = -log10(transmission);

 // record current Arduino time 
  currentTime = millis()/1000.0;
 // Update OLED display
  display.clearDisplay();
  display.setTextSize(3);             // Font zise 3
  display.setTextColor(SSD1306_WHITE); // White text
  display.setCursor(0,0);             // Start at top-left corner

  display.println(sensorValue, 4);  // show Absorbance with 4 decimal places 
  display.display();                // Update OLED display

  Serial.print(currentTime);        // send current time via serial interface to computer
  Serial.print(", ");               // add a comma to separate from next value
  Serial.print(sensorValue, 6);     // send asborbance value with 6 decimal places
  Serial.print(", ");               // add a comma to separate from next value 
  Serial.print(ADCoutput);          // send the sum of digital voltage reading. This is optional but very helpful for setting gain and troubleshooting
  Serial.print(", ");               // add a comma to separate from next value 
  Serial.println(ADCref);           // send the sum of digital voltage reading. This is optional but very helpful for setting gain and troubleshooting
 
  // Optional delay to slow down data stream
  //delay(200);
}