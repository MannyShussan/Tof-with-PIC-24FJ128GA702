# Time-of-Flight (ToF) Sensor with PIC24FJ128GA702

This repository contains the source code and resources for interfacing a Time-of-Flight (ToF) sensor with the PIC24FJ128GA702 microcontroller. This project is designed to measure distances using the ToF sensor and display the results on an LCD or transmit them via UART.

## Features

- Interface with a Time-of-Flight sensor to measure distance.
- Display measured values on an LCD.
- Option to transmit data via UART for external applications.
- Written in C for the PIC24FJ128GA702 microcontroller.

## Getting Started

### Prerequisites

To work with this project, you will need:

1. **Hardware**:
   - PIC24FJ128GA702 microcontroller.
   - A Time-of-Flight (ToF) sensor (e.g., VL53L0X or equivalent).
   - LCD display (optional).
   - UART interface for data transmission (optional).
   - Breadboard and connecting wires.
   - Power supply for the circuit.
   
2. **Software**:
   - MPLAB X IDE.
   - XC16 compiler for PIC24F series.

### Setting Up the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/MannyShussan/Tof-with-PIC-24FJ128GA702.git
   ```

2. Open the project in MPLAB X IDE.

3. Connect the hardware components:
   - Wire the ToF sensor to the I2C pins of the PIC24FJ128GA702.
   - Connect the LCD (if used) to the appropriate data and control pins.
   - Ensure UART connections are made if data transmission is required.

4. Configure the microcontroller pins as specified in the source code.

5. Build and upload the firmware to the PIC24FJ128GA702.

### Running the Project

1. Power on the circuit.
2. Observe the measured distances displayed on the LCD or transmitted via UART.
3. Adjust the sensor position as needed to test its range and accuracy.

## Repository Structure

- **src/**: Contains the source code files.
- **include/**: Header files for the project.
- **docs/**: Documentation files (e.g., circuit diagrams and explanations).

## Contributing

Contributions are welcome! If you find issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

- Special thanks to the creators and maintainers of the PIC24FJ128GA702 microcontroller and related tools.
- Inspired by the practical applications of ToF sensors in distance measurement and automation.

---

For further questions or support, feel free to contact the repository owner.

