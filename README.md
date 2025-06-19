#A Simple 3-Notes Piano Project

This is a project that I've created to learn and use what I did learn before. Implementing Systick and Edge Interrupt to generate analog signal from digital value.
I'm using Systick and Edge Interrupt as a catalyst to make the MCU ( TM4C1294XL) generates some certain frequencies that we can hear, in this case Do, Re, and Mi notes.

#Features
- As per button can generate a certain frequency when pushed based on the original MCU frequency ( 16 MHz )
- Systick used to help maintain the frequency that the button generates
- Connected to audio jack female 3,5 mm so you can hear the sound with a lot of speaker varieties

#BOM
- 3 buttons
- Some jumpers to connect the GPIO to the breadboard ( you can design PCB if you wanted )
- 5 30k Ohm resistors
- 2 15k Ohm resistors
- 1 3,5mm female audio jack
- 1 TM4C1294XL from Texas Instrument

#How To use
- This code at MainCode.c is only compatible with TM4C1294XL
- You can change the frequency that the MCU generates at the GPIO Handler ( I wrote the formula there so do as you wish )
- Build the project
- Upload the code to your MCU ( here I'm using Keil Microvision)
- Build the circuit as per this example : 
![image](https://github.com/user-attachments/assets/ae1647bb-424b-4dbc-8340-e21e24d48c0f)
notes: Its only the example especially only the DAC part, you have to make sure it matches your design of GPIO for your buttons and DAC
- A simple piano is done


#Demo
- Soon to be uploaded
