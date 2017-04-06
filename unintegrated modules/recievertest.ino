#define RF69_COMPAT 1
#include <JeeLib.h>
Port port = Port(1);
int arrayValues[100];
int i = 0;
int whichValue = 0;
void setup() {
 Serial.begin(115200);
 //Serial.println("JeeNode RF");
 rf12_initialize(3, RF12_915MHZ, 212); // initialize RF: group 212, node 3
 port.mode(OUTPUT);
 
}
void loop() {
 int readinvalue;
 double readinvaluedouble;
 //Serial.println("...");

 if ( rf12_recvDone()) { // wait for any receiving to finish
     // received data will be placed in “rf12_data”
     memcpy(&readinvalue, (int *)rf12_data, sizeof(int)); // copy “rf12_data” to lux_value
     //arrayValues[i] = readinvalue;
     Serial.print("#S|TOFILE|["); 

     Serial.print(readinvalue);
     Serial.println("]#");

     /*if (arrayValues[i] == -1) {
      whichValue = 1;
       //Serial.println(arrayValues[i-1]);
       Serial.println("previous value was lux");
      }else if (arrayValues[i] == -2){
        whichValue = 2;
         //Serial.println("previous value was heat");
      }else if(arrayValues[i] == -3){
        whichValue = 3;
          //Serial.println("previous value was hum");
      }else if(arrayValues[i] == -4){
          Serial.println("decimal detected");
          if(whichValue == 1){
           //    Serial.println("Lux : ");
            }
          Serial.print("value before decimal ");
          Serial.print(arrayValues[i-1]);
          Serial.print(".");
      }else if(arrayValues[i-1] == -4){
          Serial.print("value after decimal ");
          Serial.print(arrayValues[i-2]);
          Serial.println("");
        }else if (readinvalue > 0){
              Serial.print("positive values :");
             Serial.print(readinvalue);
            Serial.println("");
          }else if (readinvalue < 0){
            
            Serial.print("negative Values :");
            Serial.print(readinvalue);
            Serial.println("");
            }
        
        else {
                          Serial.println(readinvalue);   
          }*/
    // i++;
     //if(i == 99){
       //   i = 0;
        //}
 }
 delay(50);
} 
