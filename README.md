# SERIAL command parser for ATMEGA328

This project implement a **serial command parser** for ATMEGA328 (arduino).

It is simple and powerfull.

**Objective**: parser and execute commands that arrived from serial

**_Characteristics:_**

- Compile using Microchip Studio for ATMEGA328P microcontroller (same for arduino)
- Send by Serial one string until 20 characters 
- It is necessary start with **<** and finish with **>**
- Permit use of backspace case necessary
- test if the command is valid
- Possible 2 mode execute command: _one shoot_ or _continuos_
- commands possibles in this example = LED_ON, LED_OFF, LED_BLINK, HELP
- internal commands = STOP_FUNCTION (for one shoot command), INVALID
- This example use LED Pin in Arduino UNO (PORTB5 or 13)
