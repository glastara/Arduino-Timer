# Arduino-Timer

## Overview
This project is a simple countdown timer using an Arduino, an LCD display, and a buzzer. It allows users to set a countdown timer, start/pause/resume it, and receive an audible alert when the timer reaches zero. The timer can be set up to a maximum of 10 hours.

## Features
- Set a timer in minutes (up to 600 minutes / 10 hours)
- Start, pause, and resume the timer
- Reset functionality to set a new time
- LCD display to show countdown time
- Audible buzzer alert when the timer ends
- Onboard LED toggle on start button (due to using pin 13 --> using pin 8 caused erratic behavior)

## Components Required
- Arduino board (Uno, Mega, etc.)
- 16x2 LCD with I2C module or direct pin connection
- Buzzer
- Push buttons (3x) for set, start/pause/resume, and reset
- Resistors (if needed for button debouncing)
- Connecting wires and breadboard

## Pin Configuration
| Component     | Pin Number |
|--------------|-----------|
| LCD RS       | 12        |
| LCD E        | 11        |
| LCD D4       | 5         |
| LCD D5       | 4         |
| LCD D6       | 3         |
| LCD D7       | 2         |
| Buzzer       | 9         |
| Set Button   | 7         |
| Start Button | 13        |
| Reset Button | 6         |

## How It Works
1. The LCD initially displays `Set Timer:` and `0:00:00`.
2. Press the **Set Button** to increment the timer (by minutes, looping after 600 minutes).
3. Press the **Start Button**:
   - First press starts the countdown.
   - Second press pauses the countdown.
   - Third press resumes the countdown.
4. Press the **Reset Button** to stop the timer and reset to `0:00:00`.
5. When the timer reaches zero, the LCD displays `Time's Up!` and the buzzer emits a series of tones.

## Code Explanation
- Uses the `LiquidCrystal` library to control the 16x2 LCD.
- Implements button debouncing using `delay(200)`.
- Manages timer states (`running`, `paused`, `initialized`) to handle different button presses.
- Uses `millis()` for non-blocking timing.
- Generates a buzzer alert with increasing tone frequency upon completion.

## Installation & Usage
1. Install the `LiquidCrystal` library in your Arduino IDE if not already available.
2. Connect the components as per the pin configuration table.
3. Upload the provided code to your Arduino board.
4. Use the buttons to interact with the timer.

## Future Improvements
- Add a rotary encoder for easier time selection.
- Use an I2C LCD to reduce pin usage.
- Implement a low-power mode to conserve energy.

## License
This project is open-source and available under the MIT License.

