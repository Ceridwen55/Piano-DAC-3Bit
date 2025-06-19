# 🎹 A Simple 3-Note Piano Project
This is a simple project built to reinforce my learning of embedded systems concepts like SysTick and Edge Interrupts. The goal is to use a TM4C1294XL microcontroller to generate analog audio signals (tones) from digital values, allowing it to produce three musical notes: Do, Re, and Mi.

#Features
Generates a specific frequency (Do, Re, Mi) when each button is pressed.

Uses SysTick to maintain accurate timing for signal generation.

Utilizes Edge Interrupts to detect button presses.

Connects to a standard 3.5mm audio jack, compatible with various speakers or headphones.

Built on the TM4C1294XL microcontroller (Texas Instruments).

#Bill of Materials (BOM)
1 × TM4C1294XL microcontroller

3 × Push buttons

5 × 30kΩ resistors

2 × 15kΩ resistors

1 × 3.5mm female audio jack

Jumper wires (for GPIO to breadboard)

Breadboard (or design a PCB if preferred)

# How to Use
⚠Note: This project is compatible only with the TM4C1294XL MCU.

Clone or download the project.

Open MainCode.c in Keil uVision.

(Optional) You can modify the output frequency by adjusting the values in the GPIO Handler. The formula is commented in the code.

Build and upload the code to the MCU.

Assemble the circuit on a breadboard:

![image](https://github.com/user-attachments/assets/3871d191-b5ab-456a-bae0-855eff9a8801)
(note: make sure your GPIO and DAC match your own configuration).

Plug in your speakers or headphones to the audio jack.

Press the buttons to play notes — your simple piano is ready! 🎶

# Demo
Coming soon...


# Author
Made with Love and Head-Scratches for learning purposes.
