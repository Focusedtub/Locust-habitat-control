/* This is the bug habitat control module.
 * List of functionality is as follows:
 * Track time
 * sync time with counterpart
 * change time (received from outside source)
 * 
 * 
 * note: CC = ComputerControl
 */
#define RF69_COMPAT 1
#include<TimeLib.h>
#include<JeeLib.h>

//lCount counts the current amount of times loop has passed
int lCount = 100;
int identifier = 0;

void setup() {
  TimeElements ccTimetm;
  
  //initialize serial port
  Serial.begin(19200);
  //command line notifier
  Serial.println("Bug Habitat Control Start");
  
  //activate the RF module
  rf12_initialize(12,RF12_915MHZ,212);

  //initialize time
  setTime(12,0,0,24,01,2017);
  //initial sync
  sendTime();
  ccTimetm =recTime();
  //repeating sync
  setSyncProvider(sync());
  setSyncInterval(10);
  
  setTime(ccTimetm.Hour,ccTimetm.Minute,ccTimetm.Second,ccTimetm.Day,ccTimetm.Month,1970+ccTimetm.Year);                        
}

void loop() {
  lCount++;

  //polling for signal
  if(rf12_recvDone())
  {
    memcpy(&identifier,(int*)rf12_data,sizeof(int));
    if(identifier ==1)
    {
      //send time
    }
    if(identifier == 2)
    {
      //change time up
      timeUp();
    }
    if(identifier == 3)
    {
      //change time down
      timeDown();
    }
    if(identifier == 4)
    {
      //Full reset
      tReset();
    }
  }
  

  //delay set currently as 0.1 second/s
  delay(100);
}

//func sync will get the current time from its counterpart module
//Current sync function only syncs minutes and seconds
time_t sync(){
  TimeElements ccTimetm;

  sendTime();
  ccTimetm = recTime();

  return makeTime(ccTimetm);
}

//func tReset resets the time to the CC time
void tReset()
{
  TimeElements ccTimetm;

  sendTime();
  ccTimetm = recTime();

  setTime(ccTimetm.Hour,ccTimetm.Minute,ccTimetm.Second,ccTimetm.Day,ccTimetm.Month,ccTimetm.Year);
}

//func sendTime tells CC to send their time
void sendTime()
{
  bool c=true;
  int i =1;
  
  while(c==true) 
  {
    if(rf12_canSend())
    {
      rf12_sendStart(0,&i,sizeof(int));//tells CC to send Time
      c=false;// turns off loop to exit
      rf12_recvDone();//reactivates reciever
    }
  }
}

//func recTime receives current time from CC and checks if it is valid.
//if it is not, it attempts to get it again
//returns the time received in int
TimeElements recTime(){
  bool c=true;
  time_t ccTime;
  TimeElements tm;

  while(c==true)
  {
    if(rf12_recvDone())//waiting on receiving data from CC
    {
      //output to serial for testing
      memcpy(&ccTime,(time_t*)rf12_data,sizeof(int));
      //Serial.println(breakTime(cctime,tm));
      breakTime(ccTime,tm);
      
      if(rf12_crc!=0)//if the data isn't reliable
      {
        tm = recTime();
      }
      
      return tm;
      c=false;
    }
  }
}

//func ccTimeSend() sends the current time in time_t format to cc
void ccTimeSend()
{
  time_t t;
  while(true)
  {
    if(rf12_canSend())
    {
      rf12_sendStart(0,&t,sizeof(int));
      rf12_recvDone();
    }
  }
}

//func digitalClockDisplay display the current
//time of the arduino on the serial port.
void digitalClockDisplay(){
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

//func printDigits outputs a proper format for
//digital numbers to serial
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


//func timeUp changes the hour of time up by 1 hour
//whenever called
void timeUp(){
  adjustTime(3600);
}

//func timeDown changes the hour of time down
//by 1 hour whenever called
void timeDown(){
  adjustTime(-3600);
}
