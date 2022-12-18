# Mechatronics - Jason Noel & Phil Parisi - 03Nov2022
# Lab 6 - Python to Arduino Communication & Control

############### NOTES
# Commands for Task 1
    # a --> start
    # b --> pause
    # c --> resume
    # d --> stop
    # z --> null
    

############### PACKAGES

import tkinter as tk
import serial
import time


############### SETUP

arduino = serial.Serial(port='COM3', baudrate=38400, timeout=0.05)

val_bytes = bytes('d','utf-8')       #Convert to bytes
arduino.write(val_bytes)

############### FUNCTIONS

def runTask1():
    # Clear and Post Status Message in GUI
    out_msg = "Running Task 1"
    status_box.delete('1.0', "end")
    status_box.insert("end", out_msg)    

    # ARDUINO
    command_char = 'a';
    pyToSerial(command_char);


def stopTask1():
    # Clear and Post Status Message in GUI
    out_msg = "Stopping Task 1"
    status_box.delete('1.0', "end")
    status_box.insert("end", out_msg)   
    
    # ARDUINO
    command_char = 'd';
    pyToSerial(command_char);
 
  
def pauseTask1():
    # Clear and Post Status Message in GUI
    out_msg = "Pausing Task 1"
    status_box.delete('1.0', "end")
    status_box.insert("end", out_msg)   

    # ARDUINO
    command_char = 'b';
    pyToSerial(command_char);
    
    
def resumeTask1():
    # Clear and Post Status Message in GUI
    out_msg = "Resuming Task 1"
    status_box.delete('1.0', "end")
    status_box.insert("end", out_msg)   
    
    # ARDUINO
    command_char = 'c';
    pyToSerial(command_char);
  
  
def runTask2():
    # Clear and Post Status Message in GUI
    out_msg = "Running Task 2"
    status_box.delete('1.0', "end")
    status_box.insert("end", out_msg)   
    
    # ARDUINO CODE HERE
    command_char = 'e';
    pyToSerial(command_char); 

def stopTask2():
    # Clear and Post Status Message in GUI
    out_msg = "Stopping Task 2"
    status_box.delete('1.0', "end")
    status_box.insert("end", out_msg)     
    
    # ARDUINO CODE HERE
    command_char = 'f';
    pyToSerial(command_char);
    
def pyToSerial(status_char):
    #send_val = input("Enter value to send: ") #Read user input
    val_bytes = bytes(status_char,'utf-8')       #Convert to bytes
    arduino.write(val_bytes)                  #Send to arduino 
    print('The encoded value sent to Arduino is: ' + str(val_bytes))

    time.sleep(0.05)               #Delay to allow for transmission
    
    #read_value = arduino.readline()#Read received value from Arduino      
    #print('Displaying received value as bytes')
    #print(read_value) 
    #DecodedString =read_value.decode();
    #print('Displaying decoded input');
    #print(DecodedString);


############### GUI SETUP



# Create Window
main = tk.Tk()
main.title('Lab6: Arduino Control Panel')

# Add Title
main_title = tk.Label(main, text = "Communication & Control", font = ("Arial",11,"bold"))
blank1_label = tk.Label(main)

# Labels
task1_label = tk.Label(main, text = "Task 1", font = ("Arial", 15, "italic"))
task2_label = tk.Label(main, text = "Task 2", font = ("Arial", 15, "italic"))
blank2_label = tk.Label(main)
startnum_label = tk.Label(main, text = "Start Number")
endnum_label = tk.Label(main, text = "End Number")

# Text Boxes
status_box = tk.Text(main, width = 50, height = 1)

# Buttons
runTask1_button = tk.Button(main, text = "Start", height = 2, width = 8, command = runTask1)
stopTask1_button = tk.Button(main, text = "Stop", height = 2, width = 8,command = stopTask1)
pauseTask1_button = tk.Button(main, text = "Pause", height = 2, width = 8,command = pauseTask1)
resumeTask1_button = tk.Button(main, text = "Resume", height = 2, width = 8,command = resumeTask1)
runTask2_button = tk.Button(main, text = "Start", height = 2, width = 8,command = runTask2)
stopTask2_button = tk.Button(main, text = "Stop", height = 2, width = 8,command = stopTask2)
endapp_button = tk.Button(main, text = "Quit Application", height = 2, command = main.quit)

# Layout
main_title.grid(row = 0, column = 0, columnspan = 5)
blank1_label.grid(row = 1, column = 0, columnspan = 5, pady = 1)

task1_label.grid(row = 2, column = 0, columnspan = 2)
runTask1_button.grid(row = 3, column = 0, sticky = 'nesw')
stopTask1_button.grid(row = 3, column = 1, sticky = 'nesw')
pauseTask1_button.grid(row = 4, column = 0, sticky = 'nesw')
resumeTask1_button.grid(row = 4, column = 1, sticky = 'nesw')

blank2_label.grid(row = 2, column = 2, padx = 10)

task2_label.grid(row = 2, column = 3, columnspan = 2)
runTask2_button.grid(row = 3, column = 3, sticky = 'nesw')
stopTask2_button.grid(row = 3, column = 4, sticky = 'nesw')

status_box.grid(row = 5, column = 0, columnspan = 5, sticky = 'nesw')
endapp_button.grid(row = 6, column = 0, columnspan = 5, sticky = 'nesw')


# Loop the Main Window
main.mainloop()
