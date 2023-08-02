#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <ESP8266WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
Adafruit_Sensor *mpu_accel, *mpu_gyro;

File myFile;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    while (1);
  }

  mpu_accel = mpu.getAccelerometerSensor();
  mpu_gyro = mpu.getGyroSensor();

  if (!SD.begin(D8)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialization done.");

  // Open file and write header
  myFile = SD.open("data.csv", FILE_WRITE);
  if (myFile) {
    myFile.println("Timestamp,Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Y,Gyro_Z");
    myFile.close();
  }
}

void loop() {
  sensors_event_t accel_event, gyro_event;
  mpu_accel->getEvent(&accel_event);
  mpu_gyro->getEvent(&gyro_event);

  myFile = SD.open("data.csv", FILE_WRITE);
  if (myFile) {
    myFile.print(millis());
    myFile.print(",");
    myFile.print(accel_event.acceleration.x);
    myFile.print(",");
    myFile.print(accel_event.acceleration.y);
    myFile.print(",");
    myFile.print(accel_event.acceleration.z);
    myFile.print(",");
    myFile.print(gyro_event.gyro.x);
    myFile.print(",");
    myFile.print(gyro_event.gyro.y);
    myFile.print(",");
    myFile.println(gyro_event.gyro.z);
    myFile.close(); 
  } else {
    Serial.println("Error opening data.csv");
  }
  delay(10); 
}
