#include <Arduino.h>
#include <WiFi.h>
#include <ESP32WebServer.h>
#include<Arduino.h>
#define Current_sensor_1 23
#define Voltage_sensor_1 22
#define Current_sensor_2 01
#define Voltage_sensor_2 03
#define Current_sensor_3 21
#define Voltage_sensor_3 19
#define Current_sensor_4 18
#define Voltage_sensor_4 05
#define voltage_sensor_solar 17
#define solar_switch 04

// #include <WebServer.h>
// typedef WebServer ESPWebServer;

float R1 = 29700.0;
float R2 = 7400.0;

const char *ssid = "KU-Hackfest";
const char *password = "hackfest@2023";

const int currentSensorPin_Prosumer = Current_sensor_1;  // Change to your actual current sensor pin
const int voltageSensorPin_Prosumer = Voltage_sensor_1;

const int currentSensorPin_Consumer1 = Current_sensor_2;
const int voltageSensorPin_Consumer1 = Voltage_sensor_2;

const int currentSensorPin_Consumer2 = Current_sensor_3;
const int voltageSensorPin_Consumer2 = Voltage_sensor_3;

const int currentSensorPin_Consumer3 = Current_sensor_4;
const int voltageSensorPin_Consumer3 = Voltage_sensor_4;

const int relayPin_Prosumer = 32;
const int relayPin_Consumer1 = 33;
const int relayPin_Consumer2 = 25; 
const int relayPin_Consumer3 = 26;       // Change to your actual relay pin



ESP32WebServer server(80);

// HTML page as a string
// char html_template[] PROGMEM = R"=====(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>Sensor and Relay Control</title>
// </head>
// <body>
//   <h1>Sensor and Relay Control</h1>
//   <p>Current: <span id="current"></span>%</p>
//   <p>Voltage: <span id="voltage"></span>V</p>
//   <button onclick="turnOnRelay()">Turn On Relay</button>
//   <button onclick="turnOffRelay()">Turn Off Relay</button>

//   <script>
//     function updateSensorData() {
//       fetch('/poll')
//         .then(response => response.json())
//         .then(data => {
//           document.getElementById('current').innerText = data.current;
//           document.getElementById('voltage').innerText = data.voltage;
//         })
//         .catch(error => console.error('Error fetching sensor data:', error));
//     }

//     function turnOnRelay() {
//       fetch('/relay/on')
//         .then(response => {
//           updateSensorData();  // Update sensor data after turning the relay on
//         });
//     }

//     function turnOffRelay() {
//       fetch('/relay/off')
//         .then(response => {
//           updateSensorData();  // Update sensor data after turning the relay off
//         });
//     }

//     setInterval(updateSensorData, 1000);  // Poll every 2 seconds for sensor data
//   </script>
// </body>
// </html>
// )=====";

void setup() {
  Serial.begin(115200);

  pinMode(Voltage_sensor_1, INPUT);
  pinMode(Current_sensor_1, INPUT);
  pinMode(Voltage_sensor_2, INPUT);
  pinMode(Current_sensor_2, INPUT);
  pinMode(Voltage_sensor_3, INPUT);
  pinMode(Current_sensor_3, INPUT);
  pinMode(Voltage_sensor_4, INPUT);
  pinMode(Current_sensor_4, INPUT);
  pinMode(voltage_sensor_solar,INPUT);
  pinMode(solar_switch,OUTPUT);

  pinMode(relayPin_Prosumer, OUTPUT);
  digitalWrite(relayPin_Prosumer, HIGH);
  delay(100);
  pinMode(relayPin_Consumer1, OUTPUT);
  digitalWrite(relayPin_Consumer2, HIGH);
  delay(100);
  pinMode(relayPin_Consumer3, OUTPUT);
  digitalWrite(relayPin_Consumer3, HIGH);
  delay(100);
  pinMode(relayPin_Consumer3, OUTPUT);
  digitalWrite(relayPin_Consumer3, HIGH);

    // Set the relay off initially

  WiFi.begin("KU-Hackfest", "hackfest@2023");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // server.on("/", HTTP_GET, []() {
  //   server.send(200, "text/html", html_template);
  // });

  server.on("/poll", HTTP_GET, []() {
    float currentReading_Prosumer = analogRead(currentSensorPin_Prosumer);
    float voltageReading_Prosumer = analogRead(voltageSensorPin_Prosumer);

    float currentReading_Consumer1 = analogRead(currentSensorPin_Consumer1);
    float voltageReading_Consumer1 = analogRead(voltageSensorPin_Consumer1);

    float currentReading_Consumer2 = analogRead(currentSensorPin_Consumer2);
    float voltageReading_Consumer2 = analogRead(voltageSensorPin_Consumer3);

    float currentReading_Consumer3 = analogRead(currentSensorPin_Consumer3);
    float voltageReading_Consumer3 = analogRead(voltageSensorPin_Consumer3);



    // Convert analog readings to actual values (adjust based on your sensor's specifications)
    // float current = map(currentReading, 0, 1023, 0, 100);  // Current sensor scaling
    // float voltage = map(voltageReading, 0, 1023, 0, 5);     // Voltage sensor scaling

    String response = "{\"current_prosumer\": " + String(currentReading_Prosumer) + ",\"voltageReading_Prosumer\": " + String(voltageReading_Prosumer) + ", \"CurrentReading_Consumer1\":" + String(currentReading_Prosumer) + ",\"VoltageReading_Consumer1\":" + String(voltageReading_Prosumer) + ",\"CurrentReading_Consumer2\":" + String(currentReading_Consumer2) + ",\"VoltageReading_Consumer2\":" + String(voltageReading_Consumer2) + ",\"CurrentReading_Consumer3\":" + String(currentReading_Consumer3) + ",\"VoltageReading_Consumer3\":" + String(voltageReading_Consumer3) + "}";
    server.send(200, "application/json", response);
  });

  server.on("/relay12_ON", HTTP_GET, []() {
    digitalWrite(relayPin_Prosumer, LOW);
    delay(100);
    digitalWrite(relayPin_Consumer1, LOW);
    delay(100);
    Serial.println("Relay_Prosumer and Relay_Customer1 ON");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Prosumer and Relay_Customer1 turned ON");
  });

  server.on("/relay12_OFF", HTTP_GET, []() {
    digitalWrite(relayPin_Prosumer, HIGH);
    delay(100);
    digitalWrite(relayPin_Consumer1, HIGH);
    delay(100); 
    Serial.println("Relay_Prosumer and Relay_Customer1 OFF");
    delay(100); // Turn the relay off
    server.send(200, "text/plain", "Relay turned OFF");
  });

  server.on("/relay23_ON", HTTP_GET, []() {
    digitalWrite(relayPin_Consumer2, LOW);
    delay(100);
    digitalWrite(relayPin_Consumer3, LOW);
    delay(100);
    Serial.println("Relay_Customer2 and Relay_Customer3 ON");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Customer2 and Relay_Customer3 turned ON");
  });

  server.on("/relay23_OFF", HTTP_GET, []() {
    digitalWrite(relayPin_Consumer2, HIGH);
    delay(100);
    digitalWrite(relayPin_Consumer3, HIGH);
    delay(100);
    Serial.println("Relay_Customer2 and Relay_Customer3 OFF");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Customer2 and Relay_Customer3 turned OFF");
  });

  server.on("/relay1_ON", HTTP_GET, []() {
    digitalWrite(relayPin_Prosumer, LOW);
    delay(100);
    Serial.println("Relay_Prosumer ON");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Prosumer turned ON");
  });

  server.on("/relay1_OFF", HTTP_GET, []() {
    digitalWrite(relayPin_Prosumer, HIGH);
    delay(100);
    Serial.println("Relay_Prosumer OFF");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Prosumer turned OFF");
  });

  server.on("/relay2_ON", HTTP_GET, []() {
    digitalWrite(relayPin_Consumer1, LOW);
    delay(100);
    Serial.println("Relay_Customer1 ON");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Customer1 turned ON");
  });

  server.on("/relay2_OFF", HTTP_GET, []() {
    digitalWrite(relayPin_Consumer1, HIGH);
    delay(100);
    Serial.println("Relay_Customer1 OFF");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Customer1 turned OFF");
  });

  server.on("/relay3_ON", HTTP_GET, []() {
    digitalWrite(relayPin_Consumer2, LOW);
    delay(100);
    Serial.println("Relay_Customer2 ON");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Customer2 turned ON");
  });

  server.on("/relay3_OFF", HTTP_GET, []() {
    digitalWrite(relayPin_Consumer2, HIGH);
    delay(100);
    Serial.println("Relay_Customer2 OFF");  // Turn the relay on
    server.send(200, "text/plain", "Relay_Customer2 turned OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
  float vtg_sensorValue_1 = analogRead(Voltage_sensor_1);
  float Current_sensorValue_1 = analogRead(Current_sensor_1);
  float vtg_sensorValue_2 = analogRead(Voltage_sensor_2);
  float Current_sensorValue_2 = analogRead(Current_sensor_2);
  float vtg_sensorValue_3 = analogRead(Voltage_sensor_3);
  float Current_sensorValue_3 = analogRead(Current_sensor_3);
  float vtg_sensorValue_4 = analogRead(Voltage_sensor_4);
  float Current_sensorValue_4 = analogRead(Current_sensor_4);
  float voltage_sensor_solar_value = analogRead(voltage_sensor_solar);

  // Convert the analog reading (which goes from 0 - 4095) to a voltage (0 - 3.3V): for sensor_1 

  float voltage_1 = (vtg_sensorValue_1 * 3.3)/ 4096;
  voltage_1 = voltage_1 / (R2/(R1+R2)) ;
  

  float voltage_2 = (vtg_sensorValue_2 * 3.3)/ 4096;
  voltage_2 = voltage_2 / (R2/(R1+R2)) ;
  

  float voltage_3 = (vtg_sensorValue_3 * 3.3)/ 4096;
  voltage_3 = voltage_3 / (R2/(R1+R2)) ;
  

  float voltage_4 = (vtg_sensorValue_4 * 3.3)/ 4096;
  voltage_4 = voltage_4 / (R2/(R1+R2)) ;
  
  float solar_voltage = (voltage_sensor_solar_value * 3.3)/ 4096;
  solar_voltage = solar_voltage / (R2/(R1+R2)) ;
  


  //  current sensed 30 amp sensor 
  float voltage_1_1 = (Current_sensorValue_1 * 5)/ 4095;
  float current_1 = (voltage_1_1-2.4)/0.100;
  current_1 = current_1 - 10.30;

  float voltage_1_2 = (Current_sensorValue_2 * 5)/ 4095;
  float current_2 = (voltage_1_2-2.4)/0.100;
  current_2 = current_2 - 10.30;

  float voltage_1_3 = (Current_sensorValue_3 * 5)/ 4095;
  float current_3 = (voltage_1_3-2.4)/0.100;
  current_3 = current_3 - 10.30;

  float voltage_1_4 = (Current_sensorValue_4 * 5)/ 4095;
  float current_4 = (voltage_1_4-2.4)/0.100;
  current_4 = current_4 - 10.30;

// setting current sensed -ve to zero

  if (current_1<=1){
    current_1 = 0;
  }

  if (current_2<=1){
    current_2 = 0;
  }

  if (current_3<=1){
    current_3 = 0;
  }

  if (current_4<=1){
    current_4 = 0;
  }

  
  // displaying voltage sensed
  Serial.print("Voltage_1 = ");
  Serial.print(voltage_1);
  Serial.println(" V");
  Serial.print("Voltage_2 = ");
  Serial.print(voltage_2);
  Serial.println(" V");
  Serial.print("Voltage_3 = ");
  Serial.print(voltage_3);
  Serial.println(" V");
  Serial.print("Voltage_4 = ");
  Serial.print(voltage_4);
  Serial.println(" V");
   Serial.print("solar_voltage = ");
  Serial.print(solar_voltage);
  Serial.println(" V");



// displaying the current sensed 
  Serial.print("current_1 = ");
  Serial.print(current_1);
  Serial.println(" A");

  Serial.print("current_2 = ");
  Serial.print(current_2);
  Serial.println(" A");

  Serial.print("current_3 = ");
  Serial.print(current_3);
  Serial.println(" A");

  Serial.print("current_4 = ");
  Serial.print(current_4);
  Serial.println(" A");

  Serial.println(Current_sensorValue_1);
  Serial.println(Current_sensorValue_2);
  Serial.println(Current_sensorValue_3);
  Serial.println(Current_sensorValue_4);



//for solar charging the battery  our relay module is active Low
if (solar_voltage <=11){
  Serial.println("Solar Charging is OFF!");
  digitalWrite(solar_switch,HIGH);
}
if (solar_voltage >12){
  Serial.println("Solar Charging is ON!");
  digitalWrite(solar_switch,LOW); 
}
  delay(5000); // Wait for a second
}

// relay testing 


// # define relaytest  23
// void setup(){
//   pinMode(relaytest , OUTPUT);
//   delay(1000);
// }

// void loop (){
//   Serial.begin(115200);
//   digitalWrite(relaytest,HIGH);
//   delay(2000);// wait for 2 seconds
//   digitalWrite(relaytest,LOW);
//   delay(2000);//wait for another 2 seconds
// }
