/**
code where the button is pressed and it runs through a for loop nested in a while loop.
State 2 is sent when button is clicked. 
*/

// Add necessary libraries
#include <esp_now.h>  // To access the ESP-NOW functions
#include <WiFi.h>     // To add Wi-Fi capabilities on ESP32

int state;

//Pins where the button is attached
int b1Pin = 21;


//Pins for where the lights are attached
int b1Light = 12;
int b2Light = 13;
int b3Light = 32;
int b4Light = 33;

//Button States
int b1State = 0;


unsigned long previousMillis = 0;
const long printInterval = 250;


//Array with the indices of the lights
int Lights[4] = {b1Light, b2Light, b3Light, b4Light};


//Array holding current state of the lights
int currentLine[4] = {0,0,0,0};

//sum is how it is being checked if all stations have been "cleared"
int sum=0;

void reset();


// Save the MAC Address in an array named broadcastAddress
uint8_t DCMotor[] = {0x6c, 0xc8, 0x40, 0x87, 0xc8, 0xd8}; // MAC address of DC Motor, currently not added to physical demo
uint8_t Computer[] = {0x6c, 0xc8, 0x40, 0x8A, 0xA8, 0x14}; 
uint8_t buttonModule[] = {0x6c, 0xc8, 0x40, 0x88, 0x48, 0x20}; 
uint8_t solder2[] = {0x6c, 0xc8, 0x40, 0x4c, 0x63, 0x8c}; 
uint8_t solder3[] = {0xEC, 0xE3, 0x34, 0x6B, 0x65, 0xDC}; 

//State to send
typedef struct stateSending {
  int device;
  int State;
} stateSending;


stateSending data;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr),
           "%02X:%02X:%02X:%02X:%02X:%02X",
           info->des_addr[0], info->des_addr[1], info->des_addr[2],
           info->des_addr[3], info->des_addr[4], info->des_addr[5]);
}


void sendData(int state)
{
  //saying where to send and where the memory location of what we are sending starts. Passes size of data to give the cut off for where the data ends
  data.State = state;
  data.device = 0;
  esp_err_t result = esp_now_send(DCMotor, (uint8_t *) &data, sizeof(data));
  esp_err_t result2 = esp_now_send(Computer, (uint8_t *) &data, sizeof(data));
  esp_err_t result3 = esp_now_send(buttonModule, (uint8_t *) &data, sizeof(data));
  esp_err_t result4 = esp_now_send(solder2, (uint8_t *) &data, sizeof(data));
  esp_err_t result5 = esp_now_send(solder3, (uint8_t *) &data, sizeof(data));
}


// Structure must match sender
typedef struct struct_message {
  int device;
  int state;
} struct_message;

struct_message incomingData;

// Callback function when data is received
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
    memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
    // Send as JSON
    Serial.print("{\"State\":");
    Serial.print(incomingData.state);
    Serial.println("}");
    //Checking if a button has been pressed.
      if (incomingData.state==1)
      {
        digitalWrite(b1Light, HIGH);
        delay(2000);
        yield();
        digitalWrite(b1Light, LOW);
      }

      if (incomingData.state== 2)
        {
        int lightArray[4]= { b1Light, b2Light, b3Light, b4Light};
        int j = 0;
        while (j<3)
          {
            for (int i=0; i<4; i++)
            {
              digitalWrite(lightArray[i], HIGH);
              delay(1250);
              yield();
              digitalWrite(lightArray[i], LOW);
            }
            j=j+1;
          }
        }

      if (incomingData.state == 3)
        {
        digitalWrite(b3Light, HIGH);
        delay(5000);
        yield();
        digitalWrite(b3Light, LOW);
        }

      if (incomingData.state == 4)
        {
        digitalWrite(b4Light, HIGH);
        delay(5000);
        yield();
        digitalWrite(b4Light, LOW);
        }
}


void setup() {
  Serial.begin(115200);
  pinMode(b1Pin, INPUT_PULLUP);
  pinMode(b1Light, OUTPUT);
  pinMode(b2Light, OUTPUT);
  pinMode(b3Light, OUTPUT);
  pinMode(b4Light, OUTPUT);



  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);




    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      //Serial.println("Error initializing ESP-NOW");
      return;
    }

    // Register the send callback
    esp_now_register_send_cb(OnDataSent);

    // Register peer 1 (DC Motor)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, DCMotor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add DCMotor peer");
    return;
  }

  // Register peer 2 (Computer)
  esp_now_peer_info_t peerInfo2 = {};
  memcpy(peerInfo2.peer_addr, Computer, 6);
  peerInfo2.channel = 0;
  peerInfo2.encrypt = false;

  if (esp_now_add_peer(&peerInfo2) != ESP_OK) {
    Serial.println("Failed to add Computer peer");
    return;
  }

  // Register peer 3 (Solder1)
  esp_now_peer_info_t peerInfo3 = {};
  memcpy(peerInfo3.peer_addr, buttonModule, 6);
  peerInfo3.channel = 0;
  peerInfo3.encrypt = false;

  if (esp_now_add_peer(&peerInfo3) != ESP_OK) {
    Serial.println("Failed to add Solder1 peer");
    return;
  }

  // Register peer 4 (Solder2)
  esp_now_peer_info_t peerInfo4 = {};
  memcpy(peerInfo4.peer_addr, solder2, 6);
  peerInfo4.channel = 0;
  peerInfo4.encrypt = false;

  if (esp_now_add_peer(&peerInfo4) != ESP_OK) {
    Serial.println("Failed to add Computer peer");
    return;
  }

  // Register peer 5 (Solder3)
  esp_now_peer_info_t peerInfo5 = {};
  memcpy(peerInfo5.peer_addr, solder3, 6);
  peerInfo5.channel = 0;
  peerInfo5.encrypt = false;

  if (esp_now_add_peer(&peerInfo5) != ESP_OK) {
    Serial.println("Failed to add Solder5 peer");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  //getting the Pin State

  b1State = !(digitalRead(b1Pin));
  if (b1State == LOW)
  {
    Serial.println("sending Data");
    sendData(2);
    int lightArray[4]= { b1Light, b2Light, b3Light, b4Light};
    int j = 0;
    while (j<3)
      {
        for (int i=0; i<4; i++)
        {
          digitalWrite(lightArray[i], HIGH);
          delay(1250);
          digitalWrite(lightArray[i], LOW);
        }
        j=j+1;
      }
  }
  delay(300);
}



