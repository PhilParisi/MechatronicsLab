#Automated Door Example
#Code to illustrate the implementation of a state transition diagram
#Author: M. Jouaneh
#Copyright M. Jouaneh 2021
#***********************************************************************

import tkinter as tk                #Import tkinter libray
import tkinter.ttk as ttk           #Needed for the progress bar
import time                         #Library need for time functions

window = tk.Tk()                    #Create a tkinter window                 
window.title("Automated Door Simulator - Copyright M. Jouaneh") #Set the window title 
window.geometry('500x300')          #Set up the window size in pixel units
ProgBarVal= tk.IntVar()             # Create a control variable to use in the progress bar

DoorValue = 300                     #Current value of progress bar or simulateddoor   
SimpleMode = False
State = "Lost"
NextState = "Start"
EntryOpenState = False
EntryWaitState = False
EntryCloseDoorState  = False
DoorCloseSensorValue = True 
DoorOpenSensorValue = False
SensorStatus = False
Factor = 0
StartTime1 = 0
StartTime2 = 0

#Create an information label about this program
InfoLabel = tk.Label(text="Move mouse in/out of the orange sensor pad area to activate the green door",
                     fg="black", bg="yellow", width = 62, height = 1)                
InfoLabel.place(x=15, y =50)        #Set the position of the label in the GUI

#Create sensor pad label
SensorPad = tk.Label(text="Sensor",fg="white", bg="orange", width = 20, height = 5)                
SensorPad.place(x=100, y =150)      #Set the position of the label in the GUI                                 
#Create a progress bar to simulate door/opening closing
DoorSimul = ttk.Progressbar(window, orient="horizontal",
             mode="determinate", maximum=300, value=0, variable = ProgBarVal)
DoorSimul.place(x=120, y=120)
ProgBarVal.set(DoorValue)           #Set the door to be fully closed

#Create a label to display current program state
StateLabel = tk.Label(text = "State",fg="black", bg="yellow",
                      width=20)     #Width is number of lower case chars
StateLabel.place(x= 320, y =200)    #Set the position of the label in the GUI   
    
def SensorOn(event):                #Function that handles the mouse entering the sensor pad
    global SensorStatus
    SensorStatus = True

def SensorOff(event):               #Function that handles the mouse leaving the sensor pad
    global SensorStatus
    SensorStatus = False
    
SensorPad.bind('<Enter>',SensorOn)  #Associate 'mouse enter' event with SensorOn function
SensorPad.bind('<Leave>',SensorOff) #Associate 'mouse leave' event with SensorOff function

def ReadTimer():                    #Read the timer function
    return(time.perf_counter())

#Define the function associated with the Start button
def StartTask():       
    global SimpleMode
    global DoorCloseSensorValue 
    global DoorOpenSensorValue
    global StartTime1    

    StartButton.place_forget()
    StartTime1 = ReadTimer()
    while (1 < 2):
        window.update() 
        DoorTask()
        if (ReadTimer()-StartTime1)>=0.1:
            StartTime1 = ReadTimer()
            StateLabel["text"]= State
            if SimpleMode == False:
                DoorOpenClose()
                
#Define the function associated with the Exit button            
def Exitf():
	quit()
    
#Define Start button and associated function
StartButton = tk.Button(text="Start",command = StartTask, width = 10)
#Set the position of the Start button in the GUI
StartButton.place(x= 320, y =120)                                            
#Define Exit button and associated function
ExitButton = tk.Button(text="Exit",command = Exitf, width = 10)
#Set the position of the Exit button in the GUI
ExitButton.place(x= 320, y =160)        

def DoorTask():                         #Function that implements the state transition
    global State
    global NextState
    global WaitInterval
    global EntryOpenState
    global EntryWaitState
    global EntryCloseDoorState 
    global StartTime2

    State = NextState                   #Update the State variable     

    if State == "Start":
        if (DetectionSensor() == True):
            NextState = "OpenDoor"  

    if State == "OpenDoor":
        if EntryOpenState == False:     #Executes only in first entry to the state
            SetDoor(1)                  #A function that causes the door to open 
            EntryOpenState = True       #Set the Entry Flag to true
    
        if IsDoorOpenSensorOn() == True:#Check if the door has opened fully
            NextState = "DoorIsOpen"
            EntryOpenState = False      #Reset Entry Flag        

    if State == "DoorIsOpen":
        if DetectionSensor() == False:  #Stay in this state while sensor is on
            NextState = "Wait"
        
    if State == "Wait":
        if EntryWaitState == False:
            StartTime2 = ReadTimer()    #Record the start time
            EntryWaitState = True       #Reset Entry Flag
       
        if DetectionSensor() == True: 
            NextState = "DoorIsOpen"
            EntryWaitState = False      #Reset Entry Flag
                
        if ((ReadTimer() - StartTime2) >= 3):  #Check if 3 seconds wait interval has elapsed
            NextState = "CloseDoor"
            EntryWaitState = False      #Reset Entry Flag

    if State == "CloseDoor":
        if EntryCloseDoorState == False:
            SetDoor(0)                  #Close the door
            EntryCloseDoorState = True
            
        if DetectionSensor() == True:
            SetDoor(1)                  #Open the Door
            NextState = "OpenDoor"
            EntryCloseDoorState = False #Reset Entry Flag
        
        if IsDoorClosedSensorOn() == True: 
            NextState = "Start"         #Go back to Start state
            EntryCloseDoorState = False #Reset Entry Flag
 
def DetectionSensor():                  #Function that returns the status of the sensor pad       
    global SensorStatus
    
    return SensorStatus
    
def IsDoorOpenSensorOn():               #Function that returns the status of the DoorOpenSensor
    global DoorOpenSensorValue
    global DoorValue
    if DoorValue == 0:
        return True
    else:
        return False  
    
def IsDoorClosedSensorOn():            #Function that returns the status of the DoorClosedSensor 
    global DoorValue
    global DoorClosedSensorValue
    if DoorValue == 300:
        return True
    else:
        return False
        
def SetDoor(Activate):
    global SimpleMode
    global DoorValue
    global Factor
    global DoorCloseSensorValue 
    global DoorOpenSensorValue
        
    if SimpleMode:                     #Simple Mode Operation
        if Activate == 1:
            DoorValue = 0
            ProgBarVal.set(DoorValue)            
        else:
            DoorValue = 300
            ProgBarVal.set(DoorValue)                            
    else:                              #Timed mode    
        if Activate == 1:
            Factor = -1                #Open the door
            DoorCloseSensorValue = False
        elif Activate == 0:
            Factor = 1                 #Close the door
            DoorOpenSensorValue = False
        else:
            Factor = 0                 #No action
 
def DoorOpenClose():                   #This function runs every TimeUptateInterval
    global Factor
    global DoorCloseSensorValue 
    global DoorOpenSensorValue
    global DoorValue
    
    ProgBarMaxValue = 300
    ProgBarMinValue = 0
    DoorValue = DoorValue + Factor * 10#Compute the current position of the sliding door

    if DoorValue >= ProgBarMaxValue:
        DoorValue = ProgBarMaxValue
        DoorCloseSensorValue = True
    elif DoorValue <= ProgBarMinValue:
        DoorValue =ProgBarMinValue 
        DoorOpenSensorValue = True

    ProgBarVal.set(DoorValue)          #Set the progress bar value

window.mainloop()                      #start the event loop


