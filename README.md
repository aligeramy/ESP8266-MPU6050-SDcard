# Plotting Motion Data on a SD Card

This is a step-by-step guide on how to log accelerometer/gyroscope data from an **`MPU6050`** onto a **`SD Card`** Module using an Arduino **`ESP8266`**.

# Hardware Requirements

1. [ESP8266](https://lastminuteengineers.com/electronics/esp8266-projects/) microcontroller
2. [MPU-6050](https://lastminuteengineers.com/mpu6050-accel-gyro-arduino-tutorial/) accelerometer and gyroscope module
3. [Micro SD Card Module](https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/)
4. Micro SD Card
5. Jumper Wires
6. Breadboard (optional)

# Software Requirements

1. Arduino IDE (you can download it from https://www.arduino.cc/en/software)
2. Arduino Libraries:
    - ESP8266WiFi.h (for ESP8266 functionalities)
    - Wire.h (for I2C communication)
    - SPI.h (for SD card module)
    - SD.h (for SD card module)
    - MPU6050.h (for MPU-6050 sensor)

Note: To install the libraries, open your Arduino IDE, go to Sketch > Include Library > Manage Libraries, and search and install them.

# Hardware Setup

1. Connect the **ESP8266** to the **MPU-6050**:
    - VCC to 3V
    - GND to GND
    - SDA to D2 (GPIO 4)
    - SCL to D1 (GPIO 5)
2. Connect the **ESP8266** to the **SD Card Module**:
    - VCC to 3V
    - GND to GND
    - MISO to D6 (GPIO 12)
    - MOSI to D7 (GPIO 13)
    - SCK to D5 (GPIO 14)
    - CS to D8 (GPIO 15)
        
    ![alt text]([http://url/to/img.png](https://i.ibb.co/wMrVvhK/v2.png))
    

# Software Setup

Here is a sketch that you can upload to your ESP8266. This code will read accelerometer/gyroscope data from the MPU-6050 and save the data to a file on the SD card.

This code reads the raw and normalized values of acceleration and gyroscope data from the MPU-6050 module and writes them to the `data.csv` file on the SD card. The data is written every second.

```jsx
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
  delay(1000);
}

```

Now each row in your CSV file represents one reading with the timestamp (milliseconds since the program started), accelerometer readings in X, Y, Z order, and gyro readings in X, Y, Z order. The data can be plotted directly from this CSV file.

# **Uploading the Code**

1. Connect the D1 Mini to your computer via a USB cable.
2. Open the Arduino IDE.
3. Select the correct board and port in the Tools menu (Board: "WeMos D1 R2 & mini", Port: your D1 Mini's port).
4. Paste the provided code into a new sketch.
5. Click on the Upload button.

Please ensure your connections are correct, as incorrect connections may lead to failure in data logging or could potentially harm your devices. Also, note that the Arduino library names and their setup functions may vary, depending on the libraries you use.

# How to power the thing

Materials needed:

1. WeMos D1 Mini ESP8266
2. MPU6050 Accelerometer/Gyroscope
3. SD Card Module
4. LM7805 Voltage Regulator
5. Two 10uF Electrolytic Capacitors
6. 9V Battery with battery clip
7. Breadboard and Jumper Wires

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/933c3815-7f9f-49fd-9f88-ef908ccce2b7/Untitled.png)

Instructions:

**1. Connect the 9V Battery to the LM7805 Voltage Regulator:**

- Connect the positive (red wire) from the 9V battery clip to the Input pin of the LM7805.
- Connect the negative (black wire) from the 9V battery clip to the Ground pin of the LM7805.

 **2. Connect Capacitors to the LM7805:**

- Connect a 10uF capacitor between the Input and Ground pins of the LM7805. Connect the longer lead (positive) of the capacitor to the Input pin, and the shorter lead (negative) to the Ground pin.
- Connect another 10uF capacitor between the Output and Ground pins of the LM7805. Again, connect the longer lead (positive) of the capacitor to the Output pin, and the shorter lead (negative) to the Ground pin.

**3. Connect the LM7805 to the WeMos D1 Mini:**

- Connect the Output pin of the LM7805 to the 5V pin of the WeMos D1 Mini.
- Connect the Ground pin of the LM7805 to a GND pin on the WeMos D1 Mini.

**4. Connect the WeMos D1 Mini to the MPU6050 and the SD Card Module:**

- Connect the SDA and SCL pins of the MPU6050 to the D2 (SDA) and D1 (SCL) pins of the WeMos D1 Mini respectively.
- Connect the VCC of the MPU6050 to the 3.3V pin of the WeMos D1 Mini, and the GND of the MPU6050 to a GND pin of the WeMos.
- Connect the CS, MOSI, MISO, and CLK pins of the SD Card Module to the D8 (CS), D7 (MOSI), D6 (MISO), and D5 (CLK) pins of the WeMos D1 Mini respectively.
- Connect the VCC of the SD Card Module to the 5V pin of the WeMos D1 Mini, and the GND of the SD Card Module to a GND pin of the WeMos.

Remember that the LM7805 can get warm, so it's important to avoid touching it while the circuit is powered. For long term use or higher current applications, you may need to attach a heat sink to the LM7805 to help it dissipate heat.

Also note that a 9V battery may not last very long for such an application due to the power demands of the WeMos D1 Mini and the attached peripherals. You may need to replace or recharge the battery quite frequently.
