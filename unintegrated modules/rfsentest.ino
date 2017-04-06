
#define RF69_COMPAT 1
#include <JeeLib.h>
#include "DHT.h"
#include <SparkFunTSL2561.h>
#define DHTPIN 6 // this is the position of the data pin on the jeenode starting at I is 1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//LIGHT STUFF
SFE_TSL2561 light;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds
//END LIGHT STUFF
  unsigned int data0, data1;
  
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
 double real_lux;
 int lux_value;
 int luxDecimalTimesAHundred;

 double real_heat;
 int heat_value;
 int heatDecimalTimesAHundred;

 double real_hum;
 int hum_value;
 int humDecimalTimesAHundred;
 
 int seperatoluxheat;
 int seperatorheathum;
 int seperatorhumlux;
 int decimal;

 bool lux_decimal_exists = false;
 bool heat_decimal_exists = false;
 bool hum_decimal_exists = false;
 
void setup() {
 Serial.begin(115200);
 Serial.println("JeeNode Example 2");
 rf12_initialize(2, RF12_915MHZ, 212); // initialize RF, group 212, node 2
 dht.begin();
 //LIGHT STUFF
 light.begin();
 unsigned char ID;
   if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5X");
  }  else
  {
    byte error = light.getError();
   // printError(error);
  }
    gain = 0;
  unsigned char time = 2;

  // setTiming() will set the third parameter (ms) to the
  // requested integration time in ms (this will be useful later):
  
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);

  // To start taking measurements, power up the sensor:
  
  Serial.println("Powerup...");
  light.setPowerUp();
 //LIGHT STUFF

}
void loop() {
 seperatoluxheat = -1;
 seperatorheathum = -2;
 seperatorhumlux = -3;
 decimal = -4;


  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //LIGHT STUFF
if (light.getData(data0,data1))
  {
    // getData() returned true, communication was successful
    // To calculate lux, pass all your settings and readings
    // to the getLux() function.
    // The getLux() function will return 1 if the calculation
    // was successful, or 0 if one or both of the sensors was
    // saturated (too much light). If this happens, you can
    // reduce the integration time and/or gain.
    // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor

    
    // Perform lux calculation:

    good = light.getLux(gain,ms,data0,data1,lux);
    
    // Print out the results:
  }


  //END LIGHT STUFF
 if(good){
  real_lux = data0;
  }else{
 real_lux = 999;
  }
 lux_value = (int)real_lux;
 luxDecimalTimesAHundred = (real_lux - lux_value)*100;
 if(luxDecimalTimesAHundred != 0){
       lux_decimal_exists = true;
   }

if (!isnan(t)){
 real_heat = t;
}else{
   real_heat = 999;

  }
 heat_value = (int)real_heat;
 heatDecimalTimesAHundred = (real_heat - heat_value)*100;
 if(heatDecimalTimesAHundred != 0){
       heat_decimal_exists = true;
   }

if (!isnan(h)){
 real_hum = h;
}else{
  real_hum = 999;
  }
 hum_value = (int)real_hum;
 humDecimalTimesAHundred = (real_hum - hum_value)*100;
 if(humDecimalTimesAHundred != 0){
       hum_decimal_exists = true;
   }


Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.print("\n");
Serial.print("real hum: ");
Serial.print(real_hum);
Serial.print(" int hum: ");
Serial.print(hum_value);
Serial.print(" hum dec: ");
Serial.print(humDecimalTimesAHundred);
Serial.print(" real temp: ");
Serial.print(real_heat);
Serial.print(" int temp: ");
Serial.print(heat_value);
Serial.print(" temp dec: ");
Serial.print(heatDecimalTimesAHundred);
Serial.println("Luminosity: ");
Serial.print(lux);
Serial.print("real lum: ");
Serial.print(real_lux);
Serial.print(" int lum: ");
Serial.print(lux_value);
Serial.print("lum dec: ");
Serial.print(luxDecimalTimesAHundred);
Serial.print("Data 0: ");
Serial.print(data0);
Serial.print("Data1: ");
Serial.print(data1);
 //Serial.println(hum_value);
 //Serial.println(heat_value);
 while(!rf12_canSend()) rf12_recvDone(); // wait for any receiving to finish
 if(!lux_decimal_exists){
    rf12_sendStart(0, &lux_value,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &decimal,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &luxDecimalTimesAHundred,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &seperatoluxheat,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
 }else{
   Serial.println("double");
    rf12_sendStart(0, &lux_value,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &decimal,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &luxDecimalTimesAHundred,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &seperatoluxheat,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
  }
  if(!heat_decimal_exists){
    rf12_sendStart(0, &heat_value,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &seperatorheathum,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
 }else{
   Serial.println("double");
    rf12_sendStart(0, &heat_value,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &decimal,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &heatDecimalTimesAHundred,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &seperatorheathum,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
  }
  if(!hum_decimal_exists){
    rf12_sendStart(0, &hum_value,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &seperatorhumlux,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
 }else{
   Serial.println("double");
    rf12_sendStart(0, &hum_value,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &decimal,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &humDecimalTimesAHundred,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
    rf12_sendStart(0, &seperatorhumlux,sizeof(int)); // send the “lux_value” through RF to other JeeNode boards
    delay(500);
  }

}
 
