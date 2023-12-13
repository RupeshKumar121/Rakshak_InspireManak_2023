#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include <MPU6050.h>

static const uint32_t GPSBaud = 9600;
MPU6050 mpu;

class Buzzer{
  public:
    int outPin;
    int state = 0;

    Buzzer(int x){
      outPin = x;
      pinMode(x,OUTPUT);
    }

    void buzzOnn(){
      state = 1;
      digitalWrite(outPin,HIGH);
    }
    void buzzOff(){
      state = 1;
      digitalWrite(outPin,LOW);
    }
    void toggle(){
      if(state == 1){
        state = 0;
        digitalWrite(outPin,LOW);
      }
      else{
        state = 1;
        digitalWrite(outPin,HIGH);
      }
    }


};

class SimModule{
  public:
    int RxPin;
    int TxPin;
    int ResetPin;

    SimModule(int x,int y){
      RxPin = x;
      TxPin = y;
    }

    int makemissedcall(String num){
      SoftwareSerial Sim_SerialConnection(RxPin,TxPin);
      Sim_SerialConnection.begin(9600);
      delay(500);
      Sim_SerialConnection.println("ATD+91" + num + ";");
      delay(11000);
      Sim_SerialConnection.println("ATH");
    }

    int sendsms(String num, String msg){
      SoftwareSerial Sim_SerialConnection(RxPin,TxPin);
      Sim_SerialConnection.begin(9600);
      delay(500);
      Sim_SerialConnection.println("AT+CSMP=17,167,0,0");
      delay(500);
      Sim_SerialConnection.println("AT+CMGF=1");
      delay(500);
      Sim_SerialConnection.println("AT+CMGS=\"" + num + "\"");
      delay(500);
      Sim_SerialConnection.print(msg);
      delay(500);
      Sim_SerialConnection.write(26);


    }
};

class GpsModule{
  public:
    int RxPin;
    int TxPin;
    String latitude;
    String longitude;
    double speed;


    GpsModule(int x,int y){
      RxPin = x;
      TxPin = y;
    }

    void SetValues(){
      TinyGPSPlus gps;
      SoftwareSerial GPS(RxPin, TxPin);
      GPS.begin(GPSBaud);

      boolean newdata = false;
      for(unsigned long start = millis(); millis() - start < 1000;){
        while(GPS.available()){
          if(gps.encode(GPS.read())){
            latitude = String(gps.location.lat(), 6);
            longitude = String(gps.location.lng(), 6);
            speed = gps.speed.kmph();
            newdata = true;
            break;
          }
        }
      }
      if(newdata){
        newdata = false;
      }
    }
};

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  delay(500);
  checkSettings();
  delay(1000);
}

void loop() {
  
  Vector normAccel = mpu.readNormalizeAccel();
  float ax = normAccel.XAxis;
  float ay = normAccel.YAxis;
  float az = normAccel.ZAxis;
  float accelerationMagnitude = sqrt(ax * ax + ay * ay + az * az);
  Serial.println(accelerationMagnitude);
  
  if(accelerationMagnitude > 20){
    Buzzer buzz1(7);
    buzz1.toggle();
    GpsModule gps(5,4);
    gps.SetValues();
    SimModule Sim800(3,2);
    Serial.println("making call..");
    delay(2000);
    buzz1.toggle();
    Sim800.makemissedcall("9472771845");
    Serial.println("sending sms.....");
    String msg = "XYZ has suffered an accident: http://maps.google.com/?q="+gps.latitude+","+gps.longitude;
    Sim800.sendsms("9472771845",msg);
  }
 


}