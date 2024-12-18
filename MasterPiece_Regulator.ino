//THIS CODE IS ORIGINALLY WRITTEN BY CARA NUNEZ CALLED 'PressureSensorControlv2'. THIS IS A MODIFICATION BY MAIA HIRSCH TO CONTROL A SINGLE QB3 DEVICE AND READ DATA FROM ONE PRESSURE SENSOR.  
//THIS VERSION IS FROM OCOTOBER 25, 2024
//VERSION 2 This code is working perfectly. 

//// CONSTANTS ////
const float Vsupply = 5.0;      // Supply voltage (assuming 5V)
const float Pmax = 30.0;        // Maximum pressure in psi (adjust according to sensor specs)
const float Pmin = 0.0;         // Minimum pressure in psi (adjust according to sensor specs)
const float Offset_voltage = 0; // Offset voltage (adjust according to sensor calibration)

//// DECLARING ALL VARIABLES ////
int sensorPin = A0; //Input pin connected to analog pin A0 to read pressure sensor data 
float pressureReading; //variable to store pressure reading from sensor 
unsigned long time; //variable to store current time in miliseconds 

//// QB3 CONTROL VARIABLES //// 
int QB3Pin = 3; //PMW pin for controlling the QB3 device 
double QB3Pressure = 0; //variable to store current pressure value for QB3 
static double deltaP = 0.1; //increment for pressure changes 
static double BIT_RES = 255; //represents the bit resolution for the analog output, assuming an 8-bit range. used to scale vaues to the full range of an 8-bit PWM output (0-255) when controlling devices like the QB3

////FLAGS ////
//Flag to control whether to continuoysly print pressure data
static bool printPressure = false; 
static char keyboardIn; //stores the last character received from the keyboard

void setup() {
  ////SERIAL COMMUNICATION////
  Serial.begin(9600); //Stores the last character received from the keyboard

  //Set the sensor pin and QB3 control pin 
  pinMode(sensorPin,INPUT); //Set the sensor pin to be an input 
  pinMode(QB3Pin, OUTPUT); // SET THE QB3 control pin to output 

  //Initialize QB3 with zero pressure 
  analogWrite(QB3Pin, 0); //Start with 0 PMW signal 

  //Display initial controls 
  displayControls(); 
}

void loop() {
  //Check for incoming serial data 
  if (Serial.available()){
    keyboardIn = Serial.read(); //Read the input character 
    //Increase or decrease pressure based on input 
    if (keyboardIn == 'q'){ //if 'q' is pressed, increase the pressure
    QB3Pressure += deltaP;
    }
    else if (keyboardIn == 'a'){ //if 'a' is pressed, decrease the pressure
    QB3Pressure -= deltaP;
    
      if (QB3Pressure < 0){
        QB3Pressure = 0; // Ensure pressure does not go below zero
      }
  }
  // Normalize the pressure for PWM output 
  double normalizedV = QB3Pressure / 10.0; //Assuming the maximum pressure is 10 psi 
  int outputPWM = normalizedV * BIT_RES; 

  //Set the PWM signal to the QB3
  analogWrite(QB3Pin, outputPWM); 

  //Print the current pressure to the serial monitor 
  Serial.print("Pressure set to:"); 
  Serial.println(QB3Pressure); 

  //Additional controls 
  if (keyboardIn == '0'){
    //Reset pressure to zero 
    QB3Pressure = 0; 
    analogWrite(QB3Pin, 0);
    Serial.println("Presure reset to zero.");
  }
  else if(keyboardIn == 't'){
    // Toggle continuous pressure printout
    printPressure = !printPressure; 
  }
  else if (keyboardIn == '=') {
    // Display the availabe controls 
    displayControls(); 

  }
  }

  //If printPressure is enabled, read and print the sensor data 
  if (printPressure){
    time = millis(); //Get the current time 
    pressureReading = analogRead(sensorPin); //Read the analog value from the sensor 

    // Convert the raw reading to a voltage 
    float voltage = pressureReading * (Vsupply / 1023.0); // Calculate the voltage from the sensor reading
    voltage -= Offset_voltage; //Adjust for the offset voltage 

    // Calculate the pressure using the transfer function from the data sheet 
    float actualPressure = ((voltage - (0.10*Vsupply)) / (0.8 * Vsupply)) * (Pmax - Pmin) + Pmin;
    actualPressure = actualPressure  - 14.4; //Adjust for atmospheric pressure in PSI 

    // Print the time, raw sensor reading, and actual pressure value
    Serial.print(time);
    Serial.print(" ms, Raw: ");
    Serial.print(pressureReading);
    Serial.print(", Pressure:");
    Serial.println(actualPressure);
  }
}
//Function to display controls on the serial monitor
void displayControls() {
  Serial.println("Controls:");
  Serial.println("Q: Increase pressure by 0.1 psi");
  Serial.println("A: Decrease pressure by 0.1 psi");
  Serial.println("0: Reset pressure to zero");
  Serial.println("T: Toggle continuous pressure printout");
  Serial.println("=: Display controls");
  Serial.println("");
}