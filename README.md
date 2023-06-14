# SafetyWarnSystem

![System](/Docs/Images/Overview.png)

Introducing our Safety Warn System, a comprehensive home security solution designed to keep you and your loved ones safe at all times.

At the heart of the Safety Warn System is the Base Station, serving as the centralized hub for all sensor communication. This device is easily installed in your home, acting as the control center for your security network. Equipped with radio frequency technology, the Base Station seamlessly interacts with the array of sensors connected to it, ensuring real-time monitoring and instant notifications.

The Safety Warn System includes four essential sensors, each playing a crucial role in safeguarding your home. The Smoke and Gas Alarm sensor ensures early detection of smoke and potentially harmful gases, promptly alerting you to potential fire hazards or gas leaks. The Water Detector sensor is designed to identify any water leaks or floods, enabling you to take immediate action and prevent costly water damage. Lastly, the Door Open Detector sensor provides reliable surveillance, instantly notifying you whenever a door or window is opened, securing your home against intrusions.

When any of the sensors detect an issue, they immediately communicate with the Base Station, relaying the crucial information. The Base Station acts as the liaison, promptly relaying notifications to you through a user-friendly website and a dedicated Telegram bot. This allows you to conveniently access the Base Station from anywhere, giving you complete control and situational awareness of your home security.

Through the intuitive website and Telegram bot, you can monitor the status of each sensor, view real-time alerts, and access historical data. Additionally, you have the ability to customize your notifications based on your preferences, ensuring that you receive the right information at the right time.

## Hardware

### Microcontroller

![Controller](/Docs/Images/Adafruit.png)

This adafruit board is a very useful microcontroller because of the following reasons:

- It has a built in RFM module
- It has a battery port for direct battery operation support
- Its form is very small, smaller than the 2 previous microcontrollers
- Adafruit works with the Arduino IDE and with this supports all Arduino libraries

Each microcontroller will have a button for resetting the sensor to factory settings. The following was chosen because it short circuits two connections on press, allowing the use of an interrupt to wake the sensor up during sleep. I did not compare other buttons, since there aren’t many criteria for the button, so any cheap button will do.

![Sensor](/Docs/Images/Sensor.png)

### Smoke Detector and Gas Detector

![GasSensor](/Docs/Images/GasSensor.png)

The smoke detector needs three different hardware components:
- MQ-135 Sensor has the capabilities of detecting: Benzene, alcohol, smoke in the air or to measure the air quality.
- MQ-5 Sensor has the capabilities for measuring the concentration of liquefied petroleum gases (LPG), carbon monoxide (CO), methane (CH4) and hydrogen (H2) in the air.
- A buzzer to indicate the detection of smoke or a dangerous gas with a loud beeping sound.

The smoke detector also needs to have a button with the functionality of turning off the buzzer alarm manually when it goes off. For this purpose, the button on the microcontroller of the sensor module will be used. 

### Water sensor

![WaterSensor](/Docs/Images/GasSensor.png)

## Door sensor

![WaterSensor](/Docs/Images/DoorSensor.png)

## Communication

Before we could begin with the implementation of the sensors, we had to decide how the communication would be done between the sensors and the base station. We came up with the following solution.

### Packet Format

| idSensor      | idBase        | packetID      | packetType    | DataLength    | Data          |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| uint8_t       | uint8_t       | uint8_t       | uint8_t       | uint8_t       | variable      |

- idSensor    -       	unique id for sensor
- isBase      -       	unique id for Base Station
- packetID    -       	unique packet id for when a message is answered
- packetType 	-       	The type of data being sent
- DataLength 	-	        Indicates packet length
- Data        -       	Packet data

### Packet types

| Packettype    | idSensor      | idBase        | packetID      | packetType    | Data          |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| CONNECT       | 0             | 0             | uint8_t       | 1             | Sensor Type*  |
| CONNACK       | 0             | 0             | uint8_t       | 2             | ID for Sensor*|
|               |               |               |               | 3             |               |
| ACK           | uint8_t       | uint8_t       | uint8_t       | 4             |      -        |
| SEND          | uint8_t       | uint8_t       | uint8_t       | 5             | See Send data |
| PING          | uint8_t       | uint8_t       | uint8_t       | 6             |      -        |
| DISCONNECT    | uint8_t       | uint8_t       | uint8_t       | 7             |      -        |

*uint8_t
Sensor Type: 
- 1 - Water sensor
- 2 - Smoke detector
- 3 - Door sensor

### Send data

Water Sensor

| Sensor type    | Submerged (uint8_t)  |
| -------------- | -------------------- |
| 1              | 0 or 1               |

Smoke Sensor

| Sensor type | Gas detection - time turned off (uint8_t) minutes  | Smoke sensor (uint16_t) ppm | Gas sensor (uint16_t) ppm |
| ----------- | -------------------------------------------------- | --------------------------- | ------------------------- |
| 2           | n                                                  | n                           | n                         |

Door Sensor

| Sensor type    | Door state (uint8_t) |
| -------------- | -------------------- |
| 3              | 0 open or 1 closed   |

### Communication examples

#### Connection establishment new Connection

Sensor has no ID (Default: 0) meaning it needs one assigned

![NewConnection](/Docs/Images/NewConnection.png)

If the sensor doesn’t receive a CONNACK after 10 tries it sleeps for 1 min.

#### Connection establishment existing Connection

Sensor has ID: 15

![ExistingConnection](/Docs/Images/ExistingConnectiob.png)

If the sensor doesn’t receive a CONNACK after 10 tries it sleeps for 1 min.

#### Disconnect

Disconnect can only be done upon receiving a PING Packet or SEND Packet, because the Sensor RF Module is not constantly active. The SEND happens every 10 minutes, allowing a relatively timely disconnect.
If the base does not get an ACK after 10 tries, it will retry disconnect after the next ping from the sensor.
To disconnect a sensor it should be deleted from the sensor_data.json

![Disconnect](/Docs/Images/Disconnect.png)

#### Ping

The sensor must send a PING Packet ever 1min to the Base station to indicate it is still active. If the base station does not receive a packet after 10min, it sets the sensor status to offline/error.
If the sensor does not get an ACK/DISCONNECT after 10 tries, it must retry connecting to the station.

![Disconnect](/Docs/Images/Ping.png)

#### Sending Data

![Disconnect](/Docs/Images/SendingData.png)

Data is sent until an acknowledge is received (every 10ms up to 10 tries)
Base station sends ACK back to the sensor.
If the sensor does not get an ACK/DISCONNECT after 10 tries, it must retry connecting to the station.
Base station ignores duplicate messages.
If the base station does not receive a packet after 15min, it sets the sensor status to offline/error.

## Program flow chart

## TODO

The base station and website are projects from other students in my course, which are currently not in this repository
