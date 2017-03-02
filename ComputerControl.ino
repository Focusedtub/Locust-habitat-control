//This is the receiving end of the jeenode.
//should receive information from its counterpart
//and then send the information to the UI program
//
//Note: BC = Bug Control
#define RF69_COMPAT 1
#include<TimeLib.h>
#include<JeeLib.h>

int identifier = 0,timeDif=0, writeCount=0,loopCounter=100,fileMonth=month();
String fileTime = month()+"-"+year();
int BCTime [6];//Current time formatting
               //day,month,year,hour minutes, seconds

void setup() {
  //Serial setup
  Serial.begin(19200);
  //command line notifier
  Serial.println("Computer control module start");

  //initialize BCTime with current CCTime
  BCTime[0] = day();
  BCTime[1] = month();
  BCTime[2] = year();
  BCTime[3] = hour();
  BCTime[4] = minute();
  BCTime[5] = second();
  
  
  //activate the RF module
  rf12_initialize(12,RF12_915MHZ,212);

  //initialize time
  setTime(12,0,0,26,1,2017);
  
  //set autosync
  setSyncProvider(getTime());
  setSyncInterval(10);
}

void loop() {
  //placeholder variables
  int temp,humid,lum;
  //proper used
  int sBit,diff,test;
  

  //checking for what data type is being read from the RF module
  if(rf12_recvDone())
  {
    memcpy(&identifier,(int*)rf12_data,sizeof(int));

    if(identifier == 1)
    {
      sendTime();
    }
  }

  
  
  //reading in data from the USB port
  if(loopCounter==50||loopCounter==100)
  {
    if(Serial.available()>0)//port has data to read
    {
      for(int i=0; i!=Serial.available();i++)//for each bit available to read
      {
        sBit = Serial.read();//up for revision, may require different
                             //method for multiple bytes
      }
    
      //interpret the byte
      if(sBit ==2)
      {
        //time up
        timeUp();
      }else if(sBit == 3)
      {
        //time down
        timeDown();
      }else if(sBit == 4)
      {
        //reset time
        tReset();
      }else
      {
        //what?
      }
    }
  }

  //Writing current data to the file for GUI processing
  //current write time 10 seconds
  if(loopCounter ==100)
  {
    if(fileMonth!=month())
    {
      fileMonth =month();
    }
    if(Serial.available()==0)
    {
      Serial.println("#|TOFILE|[");
      //timestamp: hour:minute:second
      Serial.print(hour());
      Serial.print(":");
      Serial.print(minute());
      Serial.print(":");
      Serial.print(second());
      Serial.print(", ");
      //temperature
      Serial.print(temp);
      Serial.print(",");;
      //humidity
      Serial.print(humid);
      Serial.print(",");
      //luminosity
      Serial.print(lum);
      //end of output
      Serial.print("]#");
    }
    loopCounter=0;
    writeCount++;
  }

  
  //current delay set as 0.1 second/s
  delay(100);
}


//func timeRoll() tests to see if the hour has looped onto itself from 
int timeRoll()
{
  int Test,diff;
  if(hour()+timeDif<0)
  {
    diff = hour()+timeDif;
    Test = 24+diff;
  }else if(hour()+timeDif>23)
  {
    diff = hour()+timeDif-23;
    Test = 0+diff;
  }else
  {
    Test = hour()+timeDif;
  }

  return Test;
}

//function getTime gets the current time from the computer system
//its connected to
time_t getTime()
{
  time_t t;
  if(Serial.available()==0)
  {
    Serial.write(1);
  }
  
  while(true)
  {
    if(Serial.available()>0)
    {
      t = Serial.read(); 
      break;
    }
  }
  return t;


  //note commented code will be for GUI
  /*//create TimeElements variable
  tm total;

  //get the current time from the computer
  //note: this is specifically for a windows computer
  //if it is running another OS, will need to be changed
  SYSTEMTIME st;
  GetSystemTime(&st);

  //placing current time into package
  total.Year = st.wYear;
  total.Month = st.wMonth;
  total.Day = st.wDay;
  total.Hour = st.wHour;
  total.Minute = st.wMinute;
  total.Second = st.wSecond;

  return makeTime(total);*/
}

//func sendTime tells sends the current time to BC
void sendTime()
{
  time_t i;
  bool c=true;
  i = now();
  while(c==true) 
  {
    if(rf12_canSend())
    {
      rf12_sendStart(0,&i,sizeof(int));
    }
    rf12_recvDone();//reactivates reciever
  }
}

//func TimeUp tells BC to change their time up by 1 hour
void timeUp(){
  bool c =true;
  timeDif++;

  if(BCTime[3]==23)
  {
    BCTime[3]=0;
  }else
  {
    BCTime[3]++;
  }
  
  while(c==true) 
  {
    if(rf12_canSend())
    {
      rf12_sendStart(0,&c,2);
      c=false;
    }
    rf12_recvDone();//reactivates reciever
  }
}

//func TimeDown tells BC to change their time down by 1 hour
void timeDown(){
  bool c =true;
  timeDif--;

  if(BCTime[3]==0)
  {
    BCTime[3]=23;
  }else
  {
    BCTime[3]--;
  }
  
  while(c==true) 
  {
    if(rf12_canSend())
    {
      rf12_sendStart(0,&c,3);
      c=false;
    }
    rf12_recvDone();//reactivates reciever
  }
}

//func tReset tells BC to change their time to the current time
void tReset(){
  bool c =true;

  BCTime[0] = day();
  BCTime[1] = month();
  BCTime[2] = year();
  BCTime[3] = hour();
  BCTime[4] = minute();
  BCTime[5] = second();
  
  while(c==true) 
  {
    if(rf12_canSend())
    {
      rf12_sendStart(0,&c,4);
      c=false;
    }
    rf12_recvDone();//reactivates reciever
  }
}

//func recData receives data from BC and checks if it is valid.
//if it is not, it attempts to get it again
//returns the data received
int recData(){
  bool c=true;
  int data =0;

  while(c==true)
  {
    if(rf12_recvDone())//waiting on receiving data from BC
    {
      //output to serial for testing
      memcpy(&data,(int*)rf12_data,sizeof(int));
      Serial.println(data);
      
      if(rf12_crc!=0)//if the data isn't reliable
      {
        data = recData();
      }
      
      c=false;
    }
    
  }
  return data;
}
