import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import os.path

liveUpdating = None

plt.ion()

fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.2)
#l, = plt.plot(t, s, lw=2)


temp = []
hum = []
lum = []
timeStamp =[]

scriptpath = os.path.dirname(__file__)
filename = os.path.join(scriptpath, 'test.txt')

class Index(object):
    ind = 0

    def next(self, event):
        liveUpdating = False
        print(liveUpdating)
        #ydata = np.sin(2*np.pi*freqs[i]*t)
        #l.set_ydata(ydata)
        #plt.draw()

    def prev(self, event):
        liveUpdating = False
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
bnext = Button(axnext, 'Next')
bnext.on_clicked(callback.next)
bprev = Button(axprev, 'Previous')
bprev.on_clicked(callback.prev)

plt.show()

def readDataintoArray():
    with open(filename,"r") as f:
        for line in f:
            data = line.rstrip('\n').split(',')
            timeStamp.append(data[6])
            temp.append(data[7])
            hum.append(data[8])
            lum.append(data[9])
            #print("updated Arrays")

def updateData():
    with open(filename,"a") as f:
        f.write(str(7) +","+  str(1) +","+  str(2017) +","+  str(1) +","+  str(1) +","+ str(1) +","+  str(abs(int(timeStamp[len(timeStamp)-1])+1)) +","+ str(abs(int(temp[len(temp)-1])+1)) +","+ str(abs(int(hum[len(hum)-1])+1)) +","+str(abs(int(lum[len(lum)-1])+1)) +"\n")
    #print("updated data")

readDataintoArray()
#liveUpdating = True
while True:
    print(liveUpdating)
    if  (liveUpdating == None):
        plt.subplot(131)
        plt.scatter(timeStamp, temp)
        plt.subplot(132)
        plt.scatter(timeStamp,hum)
        plt.subplot(133)
        plt.scatter(timeStamp,lum)
        plt.pause(0.05)
        updateData()
        readDataintoArray()

while True:
    plt.pause(0.05)
