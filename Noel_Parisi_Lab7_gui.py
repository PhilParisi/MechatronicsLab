# Mechatronics - Jason Noel & Phil Parisi - 14Nov2022
# Lab 7 - Stepper Motor Control

############### NOTES
# Commands for State Transition (JASON DEFINE)
    # a --> 
    # b --> 
    # c --> 
    # ...
    
# States for State Transition (JASON DEFINE)
	# 0 -->
	# 1 -->
	# 2 -->
	# ...

############### PACKAGES

import tkinter as tk
import time


############### FUNCTIONS
# must define functions before we call them

def startTask():
    # Clear and Post Status Message in GUI
	out_msg = "Armed. Motor Stopped"
	writeTextReadOnly(out_msg, direction_box)   

	# Disable Start Button
	startTask_button.configure(state = 'disable')
	# Enable Other Buttons
	stopTask_button.configure(state = 'normal')
	moveRight_button.configure(state = 'normal')
	moveLeft_button.configure(state = 'normal')
	
    # MORE CODE HERE
	command_char = 'a'


def stopTask():
    # Clear and Post Status Message in GUI
	out_msg = "Motor Stopped"
	writeTextReadOnly(out_msg, direction_box)   
    
    # MORE CODE HERE
	command_char = 'd'
 
  
def moveRight():
    # Clear and Post Status Message in GUI
    out_msg = "Moving Right"
    writeTextReadOnly(out_msg, direction_box)  

    # MORE CODE HERE
    command_char = 'b'

        
def moveLeft():
    # Clear and Post Status Message in GUI
	out_msg = "Moving Left"
	writeTextReadOnly(out_msg, direction_box)
	
	# MORE CODE HERE
	command_char = 'c'
  

def exitApp():
	# Stop the Motor	
	# NEED CODE HERE to stop the motor

	# Quit Application
	#main.quit(),#main.after(1000,lambda:main.destroy())
	main.destroy()


def writeTextReadOnly(out_msg, widget):
	# keep the text boxes read only, no user writing
	widget.configure(state = 'normal')
	widget.delete('1.0', "end")
	widget.insert("end", out_msg)
	widget.configure(state = 'disabled')
	

def updateSpeed():
	# when the radio button for speed is changed, this command is run
	# the radioSpeed variable is automatically changed when the radio buttons hit
		# slow, radioSpeed = "slow"
		# medium, radioSpeed = "medium"
		# fast, radioSpeed = "fast"
		
	global currentSpeed
	if radioSpeed.get() == "slow":
		currentSpeed = 1
	elif radioSpeed.get() == "medium":
		currentSpeed = 2
	elif radioSpeed.get() == "fast":
		currentSpeed = 3
	else:
		currentSpeed = 1 # default to slowest speed
		
	#out_msg = str(currentSpeed);
	writeTextReadOnly(radioSpeed.get(), speed_box)
	
def setDefaultsGUI():
	# Set Radio Buttons
	fastSpeed_radio.deselect(), mediumSpeed_radio.deselect(), slowSpeed_radio.select();

	# Set Statuses
	writeTextReadOnly("Slow", speed_box)
	writeTextReadOnly("Click 'Start'", direction_box)
	writeTextReadOnly("Left Limit OFF", leftLimit_box)
	writeTextReadOnly("Right Limit OFF", rightLimit_box)
	
	# Gray Out Buttons except Start
	stopTask_button.configure(state = 'disable')
	moveRight_button.configure(state = 'disable')
	moveLeft_button.configure(state = 'disable')

############### GUI SETUP


# Create Window
main = tk.Tk()
main.title('Lab7: Stepper Stage Project')

# Add Title
main_title = tk.Label(main, text = "Stepper Motor Control ", font = ("Arial",11,"bold"))
blank1_label = tk.Label(main)

# Labels
selectSpeed_label = tk.Label(main, text = "Speed", font = ("Arial", 12))
motor_label = tk.Label(main, text = "Control Motor", font = ("Arial", 12))
status_label = tk.Label(main, text = "Status", font = ("Arial", 12))
blank2_label = tk.Label(main)

# Text Boxes
direction_box = tk.Text(main, width = 20, height = 1)
direction_box.configure(state = 'disabled')
speed_box = tk.Text(main, width = 20, height = 1)
speed_box.configure(state = 'disabled')
leftLimit_box = tk.Text(main, width = 20, height = 1)
leftLimit_box.configure(state = 'disabled')
rightLimit_box = tk.Text(main, width = 20, height = 1)
rightLimit_box.configure(state = 'disabled')

# Buttons
startTask_button = tk.Button(main, text = "Start", width = 8, command = startTask)
moveRight_button = tk.Button(main, text = "Go Right", width = 8, height = 2, command = moveRight)
moveLeft_button = tk.Button(main, text = "Go Left", width = 8, height = 2, command = moveLeft)
stopTask_button = tk.Button(main, text = "Stop", width = 8,command = stopTask)
exitApp_button = tk.Button(main, text = "Exit Program", command = exitApp)

# RadioButtons
radioSpeed = tk.StringVar()
slowSpeed_radio = tk.Radiobutton(main, text="Slow", variable = radioSpeed, value = "slow", command = updateSpeed)
mediumSpeed_radio = tk.Radiobutton(main, text="Medium", variable = radioSpeed, value = "medium", command = updateSpeed)
fastSpeed_radio = tk.Radiobutton(main, text="Fast", variable = radioSpeed, value = "fast", command = updateSpeed)

### Layout
main_title.grid(row = 0, column = 0, columnspan = 5)
blank1_label.grid(row = 1, column = 0, columnspan = 5, pady = 1)

# Speed
selectSpeed_label.grid(row = 2, column = 0, columnspan = 2, padx = 30)
slowSpeed_radio.grid(row = 3, column = 1, sticky = 'w')
mediumSpeed_radio.grid(row = 4, column = 1, sticky = 'w')
fastSpeed_radio.grid(row = 5, column = 1, sticky = 'w')

# Control Motor
motor_label.grid(row = 2, column = 2, columnspan = 2, padx = 30)
startTask_button.grid(row = 3, column = 2, rowspan = 2, sticky = 'nesw')
stopTask_button.grid(row = 3, column = 3, rowspan = 2, sticky = 'nesw')
moveLeft_button.grid(row = 5, column = 2, rowspan = 2, sticky = 'nesw')
moveRight_button.grid(row = 5, column = 3, rowspan = 2, sticky = 'nesw')

# Status
status_label.grid(row = 2, column = 4, padx = 30)
direction_box.grid(row = 3, column = 4, sticky = 'ns', padx = 10)
speed_box.grid(row = 4, column = 4, sticky = 'ns')
leftLimit_box.grid(row = 5, column = 4, sticky = 'ns', padx = 10)
rightLimit_box.grid(row = 6, column = 4, sticky = 'n', padx = 10)

blank2_label.grid(row = 9, column = 0, columnspan = 5, pady = 3)

#status_box.grid(row = 5, column = 0, columnspan = 5, sticky = 'nesw'
exitApp_button.grid(row = 10, column = 0, columnspan = 5, sticky = 'ew')




######################## BEGIN ACTUAL PROGRAM

# Initialize needed variables and parameters
global currentSpeed
setDefaultsGUI()

	
# Infinite While Loop
while True:

	# Update GUI Elements (replacement for main.mainloop() which blocks)
	main.update_idletasks()
	main.update()
	
	# Control Code Here [JASON!]
	
	
	
	
	
	# Sanity Check
	print("hello world")
	time.sleep(0.1)
	