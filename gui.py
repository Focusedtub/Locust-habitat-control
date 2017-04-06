import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import os.path

liveUpdating = None

plt.ion()

#fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.2)
ax = plt.subplot(131)
ax.set_title('Temperature vs Time')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Temperature (C)')
ax = plt.subplot(132)
ax.set_title('Humidity vs Time')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Humidity (%)')
ax = plt.subplot(133)
ax.set_title('Luminosity vs Time')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Luminosity (Lux)')


#l, = plt.plot(t, s, lw=2)

temporary = []
previousValue = ""
stringbuilder = ""
temp = []
hum = []
lum = []
timeStamptemp =[]
timeStamphum = []
timeStamplum = []
i_hum = 0
i_lum = 0
i_temp = 0
n_hum = 0
n_temp = 0
n_lum = 0
tenCounterhum = 5
tenCounterlum = 5
tenCountertemp = 5
tempisFloat = False
humisFloat = False
lumisFloat = False

nextValueisEndDecimal = False
finishedreading = False

scriptpath = os.path.dirname(__file__)
filename = os.path.join(scriptpath, 'log.txt')

class Index(object):
    ind = 0

    def next(self, event):
        liveUpdating = False
        #ydata = np.sin(2*np.pi*freqs[i]*t)
        #l.set_ydata(ydata)
        #plt.draw()

    def prev(self, event):
        self.ind -= 1
        i = self.ind % len(freqs)
        ydata = np.sin(2*np.pi*freqs[i]*t)
        l.set_ydata(ydata)
        plt.draw()




callback = Index()
                    #0.7, 0.05, 0.1, 0.075
                    #0.81, 0.05, 0.1, 0.075
axprev = plt.axes([.7, .05, 0.1, 0.075])
axnext = plt.axes([0.81, 0.05, 0.1, 0.075])
bnext = Button(axnext, 'Last Week')
bnext.on_clicked(callback.next)
bprev = Button(axprev, 'Real Time')
bprev.on_clicked(callback.prev)

plt.show()

def readDataintoArray():
    global nextValueisEndDecimal
    global stringbuilder
    global i_lum
    global i_hum
    global i_temp
    global n_hum
    global n_lum
    global n_temp
    global tenCounterhum
    global tenCounterlum 
    global tenCountertemp
    global hum
    global temp
    global lum
    global timeStamphum
    global timeStamplum
    global timeStamptemp
    
    with open(filename,"r") as f:
        #line= (f.readlines()[-1]).rstrip('\n')
        for line in f:
            line = line.rstrip('\n')
            if(nextValueisEndDecimal):
                stringbuilder += line
                nextValueisEndDecimal = False
            if(line == '-1'):
                try:
                    float(stringbuilder)
                    lumisFloat = True
                except ValueError:
                    lumisFloat = False
                if(lumisFloat):
                    lum.append(stringbuilder)
                    timeStamplum.append(i_lum)
                    i_lum += 1
                    print("lum : " + stringbuilder)
                stringbuilder = ""
            elif (line == '-2'):
                try:
                    float(stringbuilder)
                    tempisFloat = True
                except ValueError:
                    tempisFloat = False
                if(tempisFloat):
                    temp.append(stringbuilder)
                    timeStamptemp.append(i_temp)
                    i_temp += 1
                    print("temp : " + stringbuilder)
                strinbuilder = ""
            elif (line == '-3'):
                try:
                    float(stringbuilder)
                    humisFloat = True
                except ValueError:
                    humisFloat = False
                if(humisFloat):
                    hum.append(stringbuilder)
                    timeStamphum.append(i_hum)
                    i_hum += 1
                    print("hum : " + stringbuilder)
                stringbuilder = ""
            elif (line == '-4'):
                    stringbuilder = previousValue + "."
                    nextValueisEndDecimal = True
            previousValue = line
            if(len(hum) > 10):
                n_hum = n_hum+tenCounterhum
                #tenCounterhum += 5
                #hum =  hum[n_hum:]
                #timeStamphum =  timeStamphum[n_hum:]
            if(len(temp) > 10):
                n_temp = n_temp+tenCountertemp
                #tenCountertemp += 5
                #temp =  temp[n_temp:]
                #timeStamptemp =  timeStamptemp[n_temp:]
            if(len(lum) > 10):
                n_lum = n_lum+tenCounterlum
                #tenCounterlum += 5
                #lum =  lum[n_lum:]
                #timestamplum =  timeStamplum[n_lum:]

def updateData():
    with open(filename,"a") as f:
        f.write(str(7) +","+  str(1) +","+  str(2017) +","+  str(1) +","+  str(1) +","+ str(1) +","+  str(abs(int(timeStamp[len(timeStamp)-1])+1)) +","+ str(abs(int(temp[len(temp)-1])+1)) +","+ str(abs(int(hum[len(hum)-1])+1)) +","+str(abs(int(lum[len(lum)-1])+1)) +"\n")
    #print("updated data")

readDataintoArray()

while True:
    if  (liveUpdating == None):
        plt.subplot(131)
        plt.plot(timeStamptemp, temp)
        plt.subplot(132)
        plt.plot(timeStamphum,hum)
        plt.subplot(133)
        plt.plot(timeStamplum,)
        plt.pause(0.05)
        readDataintoArray()

while True:
    plt.pause(0.05)
