# Smart Medicine Reminder Box Using 8051

## Project overview
The Smart Medicine Reminder Box is an 8051-based project designed to remind users about scheduled medicine doses. It uses a DS1307 real-time clock (RTC), a 16×2 LCD, three LEDs, a buzzer, and a push button.

The circuit was designed and simulated in EWSIM. The current simulation runs with fixed reminder timings.

## Problem statement
People may forget to take medicines at the required time. This project demonstrates how a microcontroller can use time information and visual and audible alerts to provide medicine reminders.

## Objectives
- Design a medicine reminder circuit using the 8051 microcontroller.
- Use a real-time clock to provide time information.
- Display status and reminder messages on an LCD.
- Provide visual indications using LEDs.
- Provide an audible alert using a buzzer.
- Demonstrate the circuit in EWSIM.

## Components and functions

| Component | Function |
|---|---|
| **8051 microcontroller** | Runs the program and controls the LCD, LEDs, and buzzer. |
| **DS1307 RTC** | Provides time information to the microcontroller over SDA and SCL. |
| **16×2 LCD** | Displays status and medicine reminder messages. |
| **Red, yellow, and green LEDs** | Provide visual indications controlled by the program. The exact meaning of each color depends on the program. |
| **Buzzer** | Gives an audible indication when activated by the program. |
| **Push button** | Provides a user input to the circuit. |
| **330 Ω resistors** | Limit current through the LEDs. |
| **4.7 kΩ resistors** | Pull up the RTC’s SDA and SCL lines. |
| **5 V rails and ground** | Provide power connections in the simulated circuit. |

## Circuit connections

### 8051 to LCD
- **P2.0–P2.7** connect to LCD data pins **D0–D7**.
- **P3.0** connects to LCD **RS**.
- **P3.1** connects to LCD **Enable (E)**.
- LCD **RW** is connected to ground.

### 8051 to DS1307
- **P1.0** connects to **SDA**.
- **P1.1** connects to **SCL**.
- Both communication lines use **4.7 kΩ pull-up resistors** to the 5 V rail.

### 8051 to alerts and input
- **P1.2** connects to the yellow LED.
- **P1.3** connects to the green LED.
- **P1.4** connects to the red LED.
- **P1.5** connects to the buzzer.
- **P3.2** connects to the push button.

## How the program works

The intended program flow is:

1. **Start the system:** The 8051 begins executing the loaded program and prepares the LCD and its input/output pins.
2. **Get the current time:** The 8051 communicates with the DS1307 using the SDA and SCL lines. The DS1307 supplies the current time.
3. **Check the reminder time:** The program compares the clock time with the reminder times stored in its instructions.
4. **Show a reminder:** When a reminder condition is met, the program displays a medicine message on the LCD. The supplied HEX file contains text for morning, afternoon, and night medicine reminders.
5. **Activate an alert:** The program can control the LEDs and buzzer through the connected 8051 pins.
6. **Read the push button:** The circuit connects the button to the 8051 as an input. Its exact effect depends on the program.

### Simplified logic

```text
Start
  ↓
Initialize the LCD and 8051 pins
  ↓
Read the time from the DS1307
  ↓
Does the time match a medicine reminder?
  ├─ No → Read the time again
  └─ Yes
       ↓
     Display the reminder message
       ↓
     Activate the programmed LED and buzzer
       ↓
     Continue checking the time
```

This is a functional explanation of the circuit and the messages present in the HEX file. The exact comparisons, timing values, LED meanings, and button behavior cannot be verified because the supplied C source file is empty.

## Coding and program files
The EWSIM configuration loads `8051-1/HELLO.hex` in HEX mode.

A **HEX file** contains compiled instructions that the microcontroller can execute. It is not the editable C program.

- `8051-1/HELLO.hex` is the compiled program used by the simulation.
- `8051-1.c` is the supplied C source file, but it is currently empty.
- Because the original C source is unavailable, the program cannot be explained line by line or edited to change reminder timings.

## Project files

| File | Description |
|---|---|
| `8051-1.c` | Supplied C source file; currently empty. |
| `circuit.sim2` | EWSIM circuit design containing the components and wiring. |
| `project.json` | EWSIM project configuration that identifies the program file. |
| `8051-1/HELLO.hex` | Compiled program loaded by the simulated 8051; included in the ZIP archive. |
| `circuit project.png` | Screenshot of the EWSIM circuit. |
| `circuit-project.zip` | ZIP archive of the project files. |

## Software used
- **EWSIM** — circuit design and simulation.
- **8051 HEX program** — loaded into the simulated microcontroller.

## Applications
The project demonstrates an assistive reminder concept that could be useful for people who need regular medicine reminders, including older adults and people managing multiple medicines. It can also help students learn about microcontrollers, RTC communication, LCD interfacing, and embedded-system simulation.

This is an educational simulation, not a medical device. Users should follow the medication schedule provided by a qualified healthcare professional.

## Current status and limitations
- The circuit runs in EWSIM.
- Reminder timings are fixed in the current setup.
- The supplied C source file is empty, so the original program logic cannot be reviewed or edited.
- The project has been demonstrated in simulation; this README does not claim that physical hardware was built or tested.

## How to open the simulation
1. Download and extract `circuit-project.zip`.
2. Keep the extracted project files together.
3. Open `circuit.sim2` in EWSIM.
4. Run the simulation.

## Possible future improvements
- Add the original C source code.
- Make reminder times configurable.
- Define the specific meaning of each LED indication.
- Add or document the push button’s function.
- Build and test the circuit using physical components.
