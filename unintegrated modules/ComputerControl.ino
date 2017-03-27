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
  if(Serial.available()>0)
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
      //timestamp: day,month,year,hour,minute,second
      Serial.print(day());
      Serial.print(",");
      Serial.print(month());
      Serial.print(",");
      Serial.print(year());
      Serial.print(",");
      Serial.print(hour());
      Serial.print(",");
      Serial.print(minute());
      Serial.print(",");
      Serial.print(second());
      Serial.print(",");
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

  loopCounter++;
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

//func ascToInt converts an ascii number to an integer
int ascToInt(char a)
{
  int temp;
  temp =int(a);

  if(temp==47)
  {
    return -1;// for /
  }else if(temp==48)
  {
    return 0;
  }else if(temp==49)
  {
    return 1;
  }else if(temp==50)
  {
    return 2;
  }else if(temp==51)
  {
    return 3;
  }else if(temp==52)
  {
    return 4;
  }else if(temp==53)
  {
    return 5;
  }else if(temp==54)
  {
    return 6;
  }else if(temp==55)
  {
    return 7;
  }else if(temp==56)
  {
    return 8;
  }else if(temp==57)
  {
    return 9;
  }else if(temp==58)
  {
    return -2; // for :
  }else
  {
    return -3; //for anything else
  }
}

//function getTime gets the current time from the computer system
//its connected to
time_t getTime()
{
  tmElements_t tempTime;
  String timeRead,dateRead;
  bool c=true;

  while(true)
  {
    if(Serial.availableForWrite()>0)
    {//output command to gobetwino for send time
      Serial.println("#S|T|[]#");
      while(true)
      {
        if(Serial.available()>0)
        {
          Serial.println("it works!");
          timeRead = Serial.readString();
          Serial.println(timeRead);
          
          delay(1000);
          break;
        }
      }
      break;
    }
  }
  
  //date recieving
  while(true)
  {
    if(Serial.availableForWrite()>0)
    {//output command to gobetwino for send time
      Serial.println("#S|D|[]#");
      while(true)
      {
        if(Serial.available()>0)
        {
          Serial.println("it works!");
          dateRead = Serial.readString();
          Serial.println(dateRead);
          
          delay(1000);
          break;
        }
      }
      break;
    }
  }


  //time computing
  //6:19:19 PM
  //0123456789
  if(isSpace(timeRead.charAt(7)))//given the hour is only 1 digit long
  {
    if(timeRead.charAt(8)=='P')//given its PM
    {
      tempTime.Hour = ascToInt(timeRead.charAt(0))+12;  //18
    }else{
      tempTime.Hour = ascToInt(timeRead.charAt(0));     //6
    }
    //minutes and seconds are always two digits
    tempTime.Minute = 10*ascToInt(timeRead.charAt(2))+ascToInt(timeRead.charAt(3));
    tempTime.Second = 10*ascToInt(timeRead.charAt(5))+ascToInt(timeRead.charAt(6));
  }else{//hour is two digits long
    if(timeRead.charAt(9)=='P')//PM
    {
      if(ascToInt(timeRead.charAt(1))!=2)//not 12
      {
        tempTime.Hour = 10*ascToInt(timeRead.charAt(0))+ascToInt(timeRead.charAt(1))+12;
      }else{
        tempTime.Hour = 10*ascToInt(timeRead.charAt(0))+ascToInt(timeRead.charAt(1));
      }
    }else{
      if(ascToInt(timeRead.charAt(1))!=2)//not 12
      {
        tempTime.Hour = 10*ascToInt(timeRead.charAt(0))+ascToInt(timeRead.charAt(1));
      }else{
        tempTime.Hour = 0;//using a 24 hour clock with 12:00 = 00:00
      }
    }
    tempTime.Minute = 10*ascToInt(timeRead.charAt(3))+ascToInt(timeRead.charAt(4));
    tempTime.Second = 10*ascToInt(timeRead.charAt(6))+ascToInt(timeRead.charAt(7));        
  }

  //date computing
  //3/23/2017
  //012345678
  if(dateRead.charAt(1)=='/')//month is 1 digit long
  {
    tempTime.Month = ascToInt(dateRead.charAt(0));
    
    if(dateRead.charAt(3)=='/')//day is only 1 digit
    {
      tempTime.Day = ascToInt(dateRead.charAt(2));
      tempTime.Year = 1000*ascToInt(dateRead.charAt(4))+
                       100*ascToInt(dateRead.charAt(5))+
                        10*ascToInt(dateRead.charAt(6))+
                           ascToInt(dateRead.charAt(7))-1970;
    }else{//day is two digits
      tempTime.Day = 10*ascToInt(dateRead.charAt(2))+ascToInt(dateRead.charAt(3));
      tempTime.Year = 1000*ascToInt(dateRead.charAt(5))+
                       100*ascToInt(dateRead.charAt(6))+
                        10*ascToInt(dateRead.charAt(7))+
                           ascToInt(dateRead.charAt(8))-1970;
    }
  }else{
    tempTime.Month = 10*ascToInt(dateRead.charAt(0))+ascToInt(dateRead.charAt(1));
    
    if(dateRead.charAt(4)=='/')//day is only 1 digit
    {
      tempTime.Day = ascToInt(dateRead.charAt(3));
      tempTime.Year = 1000*ascToInt(dateRead.charAt(5))+
                       100*ascToInt(dateRead.charAt(6))+
                        10*ascToInt(dateRead.charAt(7))+
                           ascToInt(dateRead.charAt(8))-1970;
    }else{//day is two digits
      tempTime.Day = 10*ascToInt(dateRead.charAt(3))+ascToInt(dateRead.charAt(4));
      tempTime.Year = 1000*ascToInt(dateRead.charAt(6))+
                       100*ascToInt(dateRead.charAt(7))+
                        10*ascToInt(dateRead.charAt(8))+
                           ascToInt(dateRead.charAt(9))-1970;
    }
  }

  return makeTime(tempTime);
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
