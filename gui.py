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
    global previousValue
    
    with open(filename,"r") as f:
        #line= (f.readlines()[-1]).rstrip('\n')
        for line in f:
            line = line.rstrip('\n')
            if(nextValueisEndDecimal):
                stringbuilder += line
                nextValueisEndDecimal = False
            if(line == '-1'):
                try:
                    #print(stringbuilder)
                    float(stringbuilder)
                    lumisFloat = True
                    #print(lumisFloat)
                except ValueError:
                    lumisFloat = False
                    #print(stringbuilder)
                if(lumisFloat and (stringbuilder != '999')):
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
                if(tempisFloat and (stringbuilder != '999')):
                    temp.append(stringbuilder)
                    timeStamptemp.append(i_temp)
                    i_temp += 1
                    #print("temp : " + stringbuilder)
                strinbuilder = ""
            elif (line == '-3'):
                try:
                    float(stringbuilder)
                    humisFloat = True
                except ValueError:
                    humisFloat = False
                if(humisFloat and (stringbuilder != '999')):
                    hum.append(stringbuilder)
                    timeStamphum.append(i_hum)
                    i_hum += 1
                    #print("hum : " + stringbuilder)
                stringbuilder = ""
            elif (line == '-4'):
                    stringbuilder = previousValue + "."
                    nextValueisEndDecimal = True
            previousValue = line

readDataintoArray()

while True:
    if  (liveUpdating == None):
        plt.subplot(131)
        plt.scatter(timeStamptemp, temp)
        plt.subplot(132)
        plt.scatter(timeStamphum,hum)
        plt.subplot(133)
        plt.scatter(timeStamplum,lum)
        plt.pause(0.05)
        readDataintoArray()

while True:
    plt.pause(0.05)
