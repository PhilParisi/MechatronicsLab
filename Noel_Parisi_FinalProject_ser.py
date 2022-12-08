# Mechatronics - Jason Noel & Phil Parisi - 6Dec2022
# Final Project - Wheeled Robot


############### NOTES
# Commands for Task 1
	# i --> initialize
    # l --> left
    # r --> right
    # f --> forward
	# b --> back/reverse
    # s --> stop
	# a --> autonomous
    # z --> null
    

############### PACKAGES

import tkinter as tk
import serial
import time


############### SETUP

# Arduino Setup
#arduino = serial.Serial(port='COM3', baudrate=38400, timeout=0.05)
#valBytes = bytes('z','utf-8')       #Convert to bytes
#arduino.write(valBytes)

# Default Values 
commandChar = 'z' #Command to switch states DEFAULT "OFF" = 'a'
progMode = 'manual' # 2 modes: 'manual' and 'auto'
currentSpeed = 'slow' # 2 levels: 'slow' and 'fast'


############### FUNCTIONS
# must define functions before we call them

def initialize():
	global commandChar
	
	#Clear and Post Status Message in GUI
	outMsg = "System Armed"
	writeTextReadOnly(outMsg, motionBox)   
	writeTextReadOnly("Speed: slow", speedBox)
	writeTextReadOnly("Mode: manual", progModeBox)
	writeTextReadOnly("no reading", sensorBox)
		
	# Disable Start Button
	initializeTaskButton.configure(state = 'disable')
		
	# Enable Other Buttons
	stopTaskButton.configure(state = 'normal')
	forwardButton.configure(state = 'normal')
	backwardButton.configure(state = 'normal')
	turnRightButton.configure(state = 'normal')
	turnLeftButton.configure(state = 'normal')
	slowSpeedRadio.configure(state = 'normal')
	fastSpeedRadio.configure(state = 'normal')
	manualModeRadio.configure(state = 'normal')
	autoModeRadio.configure(state = 'normal')
		
	# Update Command
	commandChar = 'i'
	
	# Infinite While Loop
	runMainLoop()


def stop():
	global commandChar
	
	# Clear and Post Status Message in GUI
	outMsg = "Stopped"
	writeTextReadOnly(outMsg, motionBox)   
	
	# Update Command
	commandChar = 's'
	
	if progMode == "auto":
		manualModeRadio.select()
		updateProgMode()
 
def moveForward():
	global commandChar

	# Clear and Post Status Message in GUI
	outMsg = "Moving Forward"
	writeTextReadOnly(outMsg, motionBox)   
	
	# Update Command
	commandChar = 'f'	

def moveBackward():
	global commandChar

	# Clear and Post Status Message in GUI
	outMsg = "Moving Backward"
	writeTextReadOnly(outMsg, motionBox)   
	
	# Update Command
	commandChar = 'b'	
	
  
def turnRight():
	global commandChar
	
	# Clear and Post Status Message in GUI
	outMsg = "Turning Right"
	writeTextReadOnly(outMsg, motionBox)
	
	# Update Command
	commandChar = 'r'

        
def turnLeft():
	global commandChar
	
	# Clear and Post Status Message in GUI
	outMsg = "Turning Left"
	writeTextReadOnly(outMsg, motionBox)
	
	# Update Command
	commandChar = 'l'
  

def exitApp():
	# Stop the Motor	


	# Quit Application
	#main.quit(),#main.after(1000,lambda:main.destroy())
	main.destroy()
	quit()


def writeTextReadOnly(outMsg, widget):
	
	# keep the text boxes read only, no user writing
	widget.configure(state = 'normal')
	widget.delete('1.0', "end")
	widget.insert("end", outMsg)
	widget.configure(state = 'disabled')
	

def updateSpeed():
	# when the radio button for speed is changed, this command is run
	# the radioSpeed variable is automatically changed when the radio buttons hit
	# slow, radioSpeed = "slow"
	# fast, radioSpeed = "fast"
	global globalTimePWM	
	global currentSpeed
	
	if radioSpeed.get() == "slow":
		currentSpeed = 'slow'
	elif radioSpeed.get() == "fast":
		currentSpeed = 'fast'
	else:
		currentSpeed = 'slow' # default to slowest speed
	
	globalTimePWM = time.time()
	
	outMsg = "Speed: " + str(currentSpeed);
	writeTextReadOnly(outMsg, speedBox)
	
def setDefaultsGUI():
	global globalTimePWM
	
	# Set Radio Buttons
	fastSpeedRadio.deselect()
	slowSpeedRadio.select()
	manualModeRadio.select()
	autoModeRadio.deselect()

	# Set Statuses
	writeTextReadOnly(" ", speedBox)
	writeTextReadOnly("Click 'Initialize'!", motionBox)
	writeTextReadOnly(" ", progModeBox)
	writeTextReadOnly(" ", sensorBox)
	
	# Gray Out Buttons except Start
	stopTaskButton.configure(state = 'disable')
	forwardButton.configure(state = 'disable')
	backwardButton.configure(state = 'disable')
	turnRightButton.configure(state = 'disable')
	turnLeftButton.configure(state = 'disable')
	
	# Gray out all Radio Buttons
	slowSpeedRadio.configure(state = 'disable')
	fastSpeedRadio.configure(state = 'disable')
	manualModeRadio.configure(state = 'disable')
	autoModeRadio.configure(state = 'disable')
	
	globalTimePWM = time.time()

def updateProgMode():
	global progMode
	global commandChar
	motionMsg = " "
	
	if radioProgMode.get() == "manual":
		
		if progMode == "auto":
			writeTextReadOnly("Stopped", motionBox)
			commandChar = 's' # is this right?
			
		progMode = 'manual'
		commandToGUI = 1
		
	elif radioProgMode.get() == "auto":
		if progMode == "manual":
			writeTextReadOnly("Autonomous", motionBox)
			
		progMode = 'auto'
		commandToGUI = 0
		commandChar = 'a'
		
	else:
		progMode = 'manual'
		commandToGUI = 1
		commandChar = 's'
		writeTextReadOnly("Stopped")
	
	# Update GUI
	setAllGUIFeatures(commandToGUI)
	modeMsg = "Mode: " + str(progMode)
	writeTextReadOnly(modeMsg, progModeBox)


def setAllGUIFeatures(commandToGUI):
	if commandToGUI:
		val = 'normal'
	else:
		val = 'disable'
	
	# Set GUI Features to 'normal' or 'disable'
	forwardButton.configure(state = val)
	backwardButton.configure(state = val)
	turnRightButton.configure(state = val)
	turnLeftButton.configure(state = val)
	slowSpeedRadio.configure(state = val)
	fastSpeedRadio.configure(state = val)
	
		

def runMainLoop():
	counter = 0
	
	while True:

		# Update GUI Elements (replacement for main.mainloop() which blocks)
		main.update_idletasks()
		main.update()
		
		
		counter = counter + 1
			

	
############### GUI SETUP


# Create Window
main = tk.Tk()
main.title('RoboGUI')

# Add Title
mainTitle = tk.Label(main, text = "Communication and Control", font = ("Arial",11,"bold"))
blank1Label = tk.Label(main)

# Labels
selectSpeedLabel = tk.Label(main, text = "Speed", font = ("Arial", 12))
manualControlLabel = tk.Label(main, text = "Manual Control", font = ("Arial", 12))
statusLabel = tk.Label(main, text = "Status", font = ("Arial", 12))
selectModeLabel = tk.Label(main, text = "Mode", font = ("Arial", 12))
sensorLabel = tk.Label(main, text = "Sensor", font = ("Arial", 12))
blank2Label = tk.Label(main)

# Text Boxes
motionBox = tk.Text(main, width = 20, height = 1)
speedBox = tk.Text(main, width = 20, height = 1)
progModeBox = tk.Text(main, width = 20, height = 1)
sensorBox = tk.Text(main, width = 20, height = 1)

# Buttons
initializeTaskButton = tk.Button(main, text = "Initialize", width = 8, command = initialize)
forwardButton = tk.Button(main, text = 'Forward', width = 8, height = 2, command = moveForward)
backwardButton = tk.Button(main, text = 'Reverse', width = 8, height = 2, command = moveBackward)
turnRightButton = tk.Button(main, text = "Turn Right", width = 8, height = 2, command = turnRight)
turnLeftButton = tk.Button(main, text = "Turn Left", width = 8, height = 2, command = turnLeft)
stopTaskButton = tk.Button(main, text = "Stop", width = 8,command = stop)
exitAppButton = tk.Button(main, text = "Exit Program", command = exitApp)

# RadioButtons
radioSpeed = tk.StringVar()
slowSpeedRadio = tk.Radiobutton(main, text="Slow", variable = radioSpeed, value = "slow", command = updateSpeed)
fastSpeedRadio = tk.Radiobutton(main, text="Fast", variable = radioSpeed, value = "fast", command = updateSpeed)
radioProgMode = tk.StringVar()
manualModeRadio = tk.Radiobutton(main, text="Manual", variable = radioProgMode, value = "manual", command = updateProgMode)
autoModeRadio = tk.Radiobutton(main, text="Auto", variable = radioProgMode, value = "auto", command = updateProgMode)

##################### Layout
mainTitle.grid(row = 0, column = 0, columnspan = 5)
blank1Label.grid(row = 1, column = 0, columnspan = 5, pady = 1)

# Speed
selectSpeedLabel.grid(row = 2, column = 0, columnspan = 2, padx = 30)
slowSpeedRadio.grid(row = 3, column = 1, sticky = 'w')
fastSpeedRadio.grid(row = 4, column = 1, sticky = 'w')

# Mode
selectModeLabel.grid(row = 6, column = 0, columnspan = 2, padx = 30)
manualModeRadio.grid(row = 7, column = 1, sticky = 'w')
autoModeRadio.grid(row = 8, column = 1, sticky = 'w')

# Manual Control
manualControlLabel.grid(row = 2, column = 2, columnspan = 2, padx = 30)
initializeTaskButton.grid(row = 3, column = 2, rowspan = 2, sticky = 'nesw')
stopTaskButton.grid(row = 3, column = 3, rowspan = 2, sticky = 'nesw')
forwardButton.grid(row = 5, column = 2, rowspan = 2, sticky = 'nesw')
backwardButton.grid(row = 5, column = 3, rowspan = 2, sticky = 'nesw')
turnLeftButton.grid(row = 7, column = 2, rowspan = 2, sticky = 'nesw')
turnRightButton.grid(row = 7, column = 3, rowspan = 2, sticky = 'nesw')

# Status
statusLabel.grid(row = 2, column = 4, padx = 30)
motionBox.grid(row = 3, column = 4, sticky = 'ns', padx = 10)
speedBox.grid(row = 4, column = 4, sticky = 'ns')
progModeBox.grid(row = 5, column = 4, sticky = 'n', padx = 10)

# Sensor Reading
sensorLabel.grid(row = 7, column = 4)
sensorBox.grid(row = 8, column = 4, sticky = 'ns', padx = 10)

# Exit Button
blank2Label.grid(row = 10, column = 0, columnspan = 5, pady = 3)
exitAppButton.grid(row = 11, column = 0, columnspan = 5, sticky = 'ew')



######################## BEGIN ACTUAL PROGRAM

setDefaultsGUI() # run GUI defaults
main.mainloop() #start/open GUI