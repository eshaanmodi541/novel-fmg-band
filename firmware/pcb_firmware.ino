#include <Wire.h>
#include "SparkFun_BMI270_Arduino_Library.h"

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 

// NOTE: I havent really introduced RTOS tasks too much since the teensy is a monster
// in the future I will implement tasks in the same loop so i dont get any too long delays while writing to the OLED or SD
// I have left the OLED pretty useless for now I do not know what I am going to do with it
// this basicly just tests all the functions. main model training will happen on my laptop anyways...

int button_a_pin = 2;
int button_b_pin = 3;

int red_led = 4;
int blue_led = 5;

// SPI pins

int cd_sd = 10;
int mosi = 11;
int miso = 12;
int sck = 13;

File myFile;
int file_num = 0;
int row_counter = 0;

// I2C

int scl = 19;
int sda = 18;

// FSR

int fsr1_pin = 14;
int fsr1 = 0;
int fsr2_pin = 15;
int fsr2 = 0;
int fsr3_pin = 16;
int fsr3 = 0;
int fsr4_pin = 17;
int fsr4 = 0;

int fsr5_pin = 20;
int fsr5 = 0;
int fsr6_pin = 21;
int fsr6 = 0;
int fsr7_pin = 22;
int fsr7 = 0;
int fsr8_pin = 23;
int fsr8 = 0;

//OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// IMU

BMI270 imu;

void setup() {
  Serial.begin(9600);

  while (!Serial);
  Serial.println("Started Serial");

  SPI.begin();

  pinMode(button_a_pin, INPUT);
  pinMode(button_b_pin, INPUT);

  while(imu.beginI2C(BMI2_I2C_PRIM_ADDR) != BMI2_OK)
  {
    Serial.println("Error: BMI270 not connected, check wiring and I2C address!");
    delay(1000);
  }
  
  if (!SD.begin(cd_sd)) {
    Serial.println("SD INIT FAILED!");
    return;
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 INNIT FAILED"));
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println(F("OLED WORKS!"));

  display.display();
}

void loop() {

  if (myFile) {
    imu.getSensorData();

    float x = imu.data.gyroX;
    float y = imu.data.gyroY;
    float z = imu.data.gyroZ;

    fsr1 = analogRead(fsr1_pin);
    Serial.print(fsr1 + '\t');
    fsr2 = analogRead(fsr2_pin);
    Serial.print(fsr2 + '\t');
    fsr3 = analogRead(fsr3_pin);
    Serial.print(fsr3 + '\t');
    fsr4 = analogRead(fsr4_pin);
    Serial.print(fsr4 + '\t');
    fsr5 = analogRead(fsr5_pin);
    Serial.print(fsr5 + '\t');
    fsr6 = analogRead(fsr6_pin);
    Serial.print(fsr6 + '\t');
    fsr7 = analogRead(fsr7_pin);
    Serial.print(fsr7 + '\t');
    fsr8 = analogRead(fsr8_pin);
    Serial.print(fsr8 + '\t');

    unsigned long timeStamp = millis();

    myFile.print(timeStamp);
    myFile.print(",");
    myFile.print(fsr1);
    myFile.print(",");
    myFile.print(fsr2);
    myFile.print(",");
    myFile.print(fsr3);
    myFile.print(",");
    myFile.print(fsr4);
    myFile.print(",");
    myFile.print(fsr5);
    myFile.print(",");
    myFile.print(fsr6);
    myFile.print(",");
    myFile.print(fsr7);
    myFile.print(",");
    myFile.print(fsr8);
    myFile.print(",");
    myFile.print(x);
    myFile.print(",");
    myFile.print(y);
    myFile.print(",");
    myFile.println(z);

    row_counter += 1;

    if (row_counter % 100 == 0) {
      myFile.flush(); 
    }
  }

  if (digitalRead(button_a_pin) == HIGH) {
    String name = "DATA_" + String(file_num) + ".csv";

    myFile = SD.open(name.c_str(), FILE_WRITE);
    myFile.println("timestamp,fsr1,fsr2,fsr3,fsr4,fsr5,fsr6,fsr7,fsr8,imux,imuy,imuz");
    Serial.println("CREATED NEW FILE: " + name);
    myFile.flush();

    file_num++;
    delay(500);
  }

}
