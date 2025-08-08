# Smart House Bluetooth Control System 🏠📱

An Arduino-based smart home automation system that enables remote control of LEDs and sensors through Bluetooth communication, featuring both command-line interface and web-based control panel.

---

## Table of Contents
1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Features](#features)
4. [Hardware Requirements](#hardware-requirements)
5. [Software Components](#software-components)
6. [Installation](#installation)
7. [Usage](#usage)
8. [API Documentation](#api-documentation)
9. [Web Interface](#web-interface)
10. [Development](#development)
11. [Contributing](#contributing)

---

## Overview

This project implements a complete smart house control system using Arduino microcontrollers and Bluetooth communication. The system allows users to:

- **💡 Control Digital Outputs**: Turn LEDs and other digital devices on/off remotely
- **📊 Monitor Analog Inputs**: Read sensor values (temperature, light, etc.)
- **🔧 Manage Digital Inputs**: Monitor switch states and button presses
- **🌐 Web Interface**: User-friendly web dashboard for device control
- **💻 Command Line**: Terminal-based interface for advanced users
- **📡 Bluetooth Communication**: Wireless communication via Bluetooth module

The system follows a client-server architecture where the Arduino acts as the server managing hardware, and a Linux client application handles user interaction and communication.

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     Smart House Control System                  │
├─────────────────────────────────────────────────────────────────┤
│                          Client Side (Linux)                    │
│  ┌─────────────────┐    ┌─────────────────┐    ┌──────────────┐ │
│  │ Web Interface   │    │ Command Shell   │    │ WebSocket    │ │
│  │ • HTML/CSS/JS   │    │ • Terminal UI   │    │ Server       │ │
│  │ • Real-time     │    │ • Direct Cmd    │    │ • Real-time  │ │
│  │   Control       │    │   Interface     │    │   Updates    │ │
│  └─────────────────┘    └─────────────────┘    └──────────────┘ │
│           │                       │                       │     │
│           └───────────────────────┼───────────────────────┘     │
│                                   │                             │
│  ┌─────────────────────────────────┼─────────────────────────┐   │
│  │            Client Application                           │   │
│  │  ┌─────────────────┐    ┌─────────────────┐            │   │
│  │  │ Bluetooth Comm. │    │ Serial Comm.    │            │   │
│  │  │ • RFCOMM        │    │ • USB/UART      │            │   │
│  │  │ • Device Pairing│    │ • Direct Cable  │            │   │
│  │  └─────────────────┘    └─────────────────┘            │   │
│  └─────────────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────────┤
│                        Communication Layer                      │
│         Bluetooth/Serial ◄──────── Protocol ────────►          │
├─────────────────────────────────────────────────────────────────┤
│                        Server Side (Arduino)                    │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                    Arduino Microcontroller                 │ │
│  │  ┌─────────────────┐    ┌─────────────────┐               │ │
│  │  │ UART Controller │    │ ADC Controller  │               │ │
│  │  │ • Serial Comm   │    │ • Analog Read   │               │ │
│  │  │ • Command Parse │    │ • 8-Channel     │               │ │
│  │  └─────────────────┘    └─────────────────┘               │ │
│  │  ┌─────────────────┐    ┌─────────────────┐               │ │
│  │  │ GPIO Controller │    │ EEPROM Storage  │               │ │
│  │  │ • Digital I/O   │    │ • Config Data   │               │ │
│  │  │ • 8 In + 8 Out  │    │ • Device Names  │               │ │
│  │  └─────────────────┘    └─────────────────┘               │ │
│  └─────────────────────────────────────────────────────────────┘ │
├─────────────────────────────────────────────────────────────────┤
│                          Hardware Layer                         │
│  ┌─────────────────┐    ┌─────────────────┐    ┌──────────────┐ │
│  │ Digital Outputs │    │ Digital Inputs  │    │ Analog       │ │
│  │ • 8 LEDs        │    │ • 8 Switches    │    │ Sensors      │ │
│  │ • Relays        │    │ • Buttons       │    │ • 8 Channels │ │
│  │ • Actuators     │    │ • Sensors       │    │ • 10-bit ADC │ │
│  └─────────────────┘    └─────────────────┘    └──────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

---

## Features

### 🎛️ **Device Control**
- **8 Digital Output Channels**: Control LEDs, relays, and other digital devices
- **8 Digital Input Channels**: Monitor switches, buttons, and digital sensors
- **8 Analog Input Channels**: Read sensor values with 10-bit precision
- **PWM Support**: Variable intensity control for compatible devices

### 📡 **Communication Options**
- **Bluetooth RFCOMM**: Wireless communication via Bluetooth module
- **Serial USB**: Direct wired connection via USB/UART
- **Configurable**: Switch between communication methods easily

### 🌐 **User Interfaces**
- **Web Dashboard**: Modern, responsive web interface with real-time updates
- **Command Shell**: Terminal-based interface for scripting and automation
- **WebSocket API**: Real-time bidirectional communication

### 🔧 **Advanced Features**
- **Custom Device Naming**: Assign meaningful names to channels
- **EEPROM Storage**: Persistent configuration storage on Arduino
- **Error Handling**: Comprehensive error reporting and recovery
- **Multi-platform**: Linux client with cross-platform potential

---

## Hardware Requirements

### Arduino Setup
- **Microcontroller**: Arduino Uno/Nano/Pro Mini (ATmega328P)
- **Bluetooth Module**: HC-05/HC-06 or compatible
- **Digital Outputs**: 8 LEDs or relay modules connected to Port B
- **Digital Inputs**: 8 switches/buttons connected to Port A
- **Analog Inputs**: Up to 8 analog sensors (ADC channels 0-7)

### Client System
- **Operating System**: Linux (Ubuntu/Debian recommended)
- **Bluetooth**: Bluetooth adapter with RFCOMM support
- **Development Tools**: GCC compiler, make, bluetooth libraries

---

## Software Components

### 📱 **Client Application** (`client/`)
- **`main.c`**: Application entry point with communication setup
- **`avr_client_functions.c`**: Core client functionality and command handling
- **`avr_client_serial.c`**: Serial communication implementation
- **`shell.c`**: Interactive command-line interface
- **`webserver.c`**: WebSocket server for web interface
- **`error.c`**: Error handling and reporting utilities

### 🤖 **Arduino Server** (`server/`)
- **`main.c`**: Arduino firmware main loop and hardware control
- **`uart.c`**: UART communication protocol
- **`serial.c`**: Serial data handling and parsing
- **`eeprom_functions.c`**: EEPROM storage management
- **`utils.h`**: Common utilities and definitions

### 🌐 **Web Interface** (`html/`)
- **`index.html`**: Main web dashboard interface
- **`smarthouse.css`**: Styling and responsive design
- **`smarthouse.js`**: JavaScript for WebSocket communication and UI interactions

---

## Installation

### Prerequisites

1. **Install development tools**
   ```bash
   sudo apt update
   sudo apt install gcc make libbluetooth-dev libwebsockets-dev
   ```

2. **Install AVR toolchain** (for Arduino development)
   ```bash
   sudo apt install avr-gcc avr-libc avrdude
   ```

3. **Configure Bluetooth permissions**
   ```bash
   sudo adduser $USER dialout
   sudo adduser $USER bluetooth
   # Reboot required after group changes
   ```

### Building the Project

1. **Clone the repository**
   ```bash
   git clone https://github.com/LorenzoCiarpa/SO-Smart-House-Bluetooth.git
   cd SO-Smart-House-Bluetooth
   ```

2. **Build Arduino firmware**
   ```bash
   cd server
   make
   # Upload to Arduino
   make main.hex
   ```

3. **Build client application**
   ```bash
   cd ../client
   make
   ```

---

## Usage

### Arduino Setup

1. **Connect hardware components**:
   - Connect LEDs to Port B (pins 8-13 on Arduino Uno)
   - Connect switches to Port A (analog pins A0-A7 as digital inputs)
   - Connect Bluetooth module (VCC, GND, TX→D0, RX→D1)
   - Connect analog sensors to ADC channels

2. **Upload firmware**:
   ```bash
   cd server
   avrdude -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -U flash:w:main.hex
   ```

### Client Application

#### Command Line Interface

```bash
cd client
./main
```

**Available Commands**:
```bash
# Set device name
set_name MySmartHouse

# Set custom channel names
set_channel_name switch_0 LivingRoomLight
set_channel_name analog_in_0 TemperatureSensor

# Control digital outputs
set_channel_value LivingRoomLight 1    # Turn on
set_channel_value LivingRoomLight 0    # Turn off

# Read sensor values
get_channel_value LivingRoomLight      # Read digital state
get_adc_channel_value TemperatureSensor # Read analog value

# List all channels
query_channels

# Get help
help
```

#### Web Interface

1. **Start web server mode** (modify `main.c` to enable web server)
2. **Open browser** and navigate to `http://localhost:9000`
3. **Set device name** using the input field
4. **Control devices** using the interactive buttons and sliders

### Bluetooth Configuration

1. **Pair Bluetooth module**:
   ```bash
   sudo bluetoothctl
   scan on
   pair 98:D3:51:F5:E0:93  # Replace with your module's MAC
   trust 98:D3:51:F5:E0:93
   ```

2. **Update MAC address** in `avr_client_functions.c`:
   ```c
   char dest[18] = "98:D3:51:F5:E0:93";  // Your Bluetooth module MAC
   ```

---

## API Documentation

### Communication Protocol

The system uses a simple text-based protocol over UART/Bluetooth:

#### Command Format
```
COMMAND DEVICE_NAME CHANNEL_NAME [VALUE]
```

#### Supported Commands

**Set Digital Output**:
```
set_channel_value MyDevice switch_0 1
Response: ack
```

**Read Digital Input**:
```
get_channel_value MyDevice digital_in_0
Response: digital_in_0 1
```

**Read Analog Input**:
```
get_adc_channel_value MyDevice analog_in_0
Response: analog_in_0 512
```

**Set Channel Name**:
```
set_channel_name switch_0 LivingRoomLight
Response: ack
```

**Error Response**:
```
Response: nack
```

### WebSocket API

#### Client → Server Messages
```javascript
// Set digital output
ws.send("set_channel_value_web DeviceName switch_0 1");

// Read digital input  
ws.send("get_channel_value_web DeviceName digital_in_0");

// Read analog input
ws.send("get_adc_channel_value_web DeviceName analog_in_0");
```

#### Server → Client Messages
```javascript
// Channel value update
"switch_0 1"
"analog_in_0 512"
```

---

## Web Interface

### Dashboard Features

- **🎛️ Device Controls**: Toggle switches and PWM sliders for each channel
- **📊 Real-time Updates**: Live sensor readings via WebSocket
- **🎨 Modern UI**: Clean, intuitive interface with CSS styling

### Usage Examples

1. **Device Naming**: Set a descriptive name for your Arduino device
2. **Switch Control**: Click buttons to toggle LED states (ON/OFF)
3. **PWM Control**: Use sliders for variable intensity control
4. **Sensor Monitoring**: View real-time analog sensor readings
5. **Status Indicators**: Visual feedback for all device states

---

## Development

### Project Structure
```
SO-Smart-House-Bluetooth/
├── client/                 # Linux client application
│   ├── main.c             # Application entry point
│   ├── avr_client_functions.c  # Core client functionality
│   ├── avr_client_serial.c     # Serial communication
│   ├── shell.c            # Command-line interface
│   ├── webserver.c        # WebSocket server
│   ├── error.c            # Error handling
│   └── Makefile           # Build configuration
├── server/                # Arduino firmware
│   ├── main.c             # Arduino main program
│   ├── uart.c             # UART communication
│   ├── eeprom_functions.c # EEPROM storage
│   ├── avr.mk             # AVR build configuration
│   └── Makefile           # Build configuration
├── html/                  # Web interface
│   ├── index.html         # Main dashboard
│   ├── smarthouse.css     # Styling
│   └── smarthouse.js      # JavaScript functionality
└── Report Progetto OS.pdf # Technical documentation
```

### Adding New Features

1. **New Commands**: Add functions to `avr_client_functions.c` and corresponding Arduino handlers
2. **Web Controls**: Extend `index.html` and `smarthouse.js` for new UI elements
3. **Hardware Support**: Modify Arduino firmware for additional sensors/actuators

### Debugging

- **Serial Monitor**: Use Arduino IDE or `screen /dev/ttyUSB0 9600` for debugging
- **Bluetooth Debug**: Check `rfcomm` connection status and pairing
- **Web Debug**: Use browser developer tools for WebSocket debugging

---

## Configuration

### Communication Mode Selection

In `client/main.c`, choose between communication modes:

```c
// For Bluetooth communication
if(avr_bluetooth_connection_init() == 0){
    fprintf(stderr, "Connection problem to the host\n");
    return EXIT_FAILURE;
}

// For Serial communication (uncomment instead)
/*
if(avr_connection_init() == 0){
    fprintf(stderr, "Connection problem to the host\n");
    return EXIT_FAILURE;
}
*/
```

### Interface Mode Selection

Choose between command-line and web interface:

```c
// For command-line interface
avr_client_loop();

// For web server interface (uncomment instead)
/*
if(webServerCreate() == -1){
    fprintf(stderr, "Errore nella socket\n");
    return EXIT_FAILURE;
};
*/
```

---


## License

This project was developed for Operating Systems coursework and is available for educational purposes.

---


**Note**: This system is designed for educational and prototyping purposes. For production use, implement additional security measures and error handling.