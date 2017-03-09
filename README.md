# Locust-habitat-control
The files here contain the necessary programs needed to run the habitat control.
It also contains current tasks and extra information on function

####Current Identifier list
1. Send time
2. Send time up
3. Send time down
4. Time reset

###Current send time format
time_t variable type
#####File Format
time_stamp,temp,humd,lumi

####Windows Time-long format
h:mm:ss tt
####Windows Date-short format
DD:MM:YYYY

###Current task List
Josh P.
- [x]Project design rough/*                 */Jan 24, 2017
- [x]Create github/*                        */Jan 26, 2016
- [x]Record time/*                          */Jan 24, 2017
- [x]Change-time capabilities/*             */Jan 26, 2017
- [x]Optimize send time format/*            */Feb 02, 2017
- [x]Time management/*                      */Feb 09, 2017
- [x]Project design presentation/*          */Feb 16, 2017
- [x]Write to file/*                        */Mar 02, 2017
- [x]Read time from gobetwino/*             */Mar 07, 2017
- [x]Read from GUI/*                        */Mar 09, 2017
- [x]Technical report/*                     */Mar 09, 2017
- [ ]integrate

Ian B.
- []

Noah K.
- []

Saad N.
- []

Mohammed A.
- []

Siyu Z.
- []


###Current Function list
#### Computer Control
- timeRoll: Tests to see if the hour needs to be reset to 0 or 23. Returns time in hours.
- getTime: Gets current time from the computer. returns time in a time_t format.
- sendTime: Sends a request to BC to send their current time
- timeUp: Sends a request to BC to change their time up by 1 hour, also updates own reference of BC's time
- timeDOwn: Sends a request to BC to change their time down by 1 hour, also updates own reference of BC's time
- tReset: Sends a request to BC to change their time to the current time.
- recData: Receives data that has been sent by BC. Returns what was received.

#### Bug Control
- sync: Gets current time from the CC. Returns time_t variable.
- tReset: Resets the current time to CC current time.
- sendTime: Sends a request to CC to send their current time.
- recTime: Recieves current time from CC. Returns a TimeElements struct. 
- ccTimeSend: Sends the current time in time_t variable to CC
- digitalClockDisplay: displays the current time to the serial port.
- printDigits: inputs an integer, and formats it into a time digit.
- timeUp: Changes current time up by 1 hour
- timeDown: Changes current time down by 1 hour
