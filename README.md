CASA0016 Project Report

River Monitoring System

Dongyi Ma 

January 2022

GitHub Repo Path: https://github.com/mandymadongyi/CASA0016-Individual-Prototype

 

#  

# Introduction 

Rivers are a critical but generally under-researched environment since they are distributed throughout the wild and not easily accessible. Rivers are essential as changes in water features also indicate climate movements and landscape changes. To better understand our environment, a sensor-based river monitoring system is designed to provide remote access to real-time information about the river without heading to the riverside to check its’ condition. While most existing watering monitoring systems can only visualise data locally without storing historical data, or merely use one or two sensors, the motivation of this project is to build a river monitoring system, lightweight and intelligent, that measures the water level, PH value and temperature, then keep tracking and analysing sensor data by visualising real-time data locally on LCD at the riverside and also remotely using raspberry pi and Grafana dashboard display. This report documents the design, build and functionality of the prototype in Design and Build and reflects on challenges, key learnings, critical feedbacks, and future work in Reflection. 

# Project Goals

This prototype explores the potential of remote monitoring environmental data and recording the changes of rivers (water level, water temperature and PH value) using multiple types of sensors, allowing us to better understand the real-time condition of the river and its tendencies. 

# Design and Build

## Design rationale 

### Collecting data

#### Measurement for Water Temperature 

[Connect temperature sensor to Arduino board](https://arduinogetstarted.com/tutorials/arduino-temperature-sensor) (Arduino - Temperature Sensor | Arduino Tutorial, 2022) to collect real-time water temperature data.

 



#### Measurement for Water Level 

[Connect ultrasonic sensor to Arduino board](https://arduinogetstarted.com/tutorials/arduino-ultrasonic-sensor)  (Arduino - Ultrasonic Sensor | Arduino Tutorial, 2022) to collect the distance between the ultrasonic sensor and the object. If the elevation of the sensor is h, the distance measured by the ultrasonic sensor is d, then

 

Water Level = h – d (meters)

 

#### Measurement for PH value 

[Connect PH sensor to Arduino board](https://github.com/mandymadongyi/CASA0016-Individual-Prototype/blob/main/References/User Manual PH Sensor.pdf) (CASA0016-Individual-Prototype/User Manual PH Sensor.pdf at main · mandymadongyi/CASA0016-Individual-Prototype, 2022) to collect real-time voltage data at Arduino’s analogue pin, then convert the analogue signal into PH value. According to the millivolts to pH formula discovered by Dr Jerry Tennant, PH number follows a simple linear relationship with the millivolts in the liquid, where the full range of pH scale goes from a low acidic pH 0 = +400 mV to a high alkaline pH 14 = -400 mV level. (pH Voltage | Acidity, Alkalinity equals Volts calculator, 2022)

 

![Table  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image001.png)

 

Figure 1. PH and mV mapping (pH Voltage | Acidity, Alkalinity equals Volts calculator, 2022)

 

 

### Data Aggregation and Visualisation

To aggregate and consume collected data for tracking and analysing, the following steps are needed.

#### Local visualisation 

[Connect LCD to Arduino board](https://arduinogetstarted.com/tutorials/arduino-ultrasonic-sensor-lcd) (Arduino - Ultrasonic Sensor - LCD | Arduino Tutorial, 2022) to display water temperature, level, PH value at the riverside.

#### Remote visualisation 

After collecting data, [Connect the device to Wi-Fi and MQTT](https://docs.arduino.cc/tutorials/uno-wifi-rev2/uno-wifi-r2-mqtt-device-to-device) (Arduino-uno-wifi-rev2-mqtt-device-to-device| Arduino Tutorial, 2022) to transmit sensor data for aggregation. Store data in influx database in a raspberry pi microcontroller, and then visualise on Grafana dashboard.  (cetools.org| CASA0014 Plant-Monitor Tutorial, 2022). 

 

## Building Process

### Part List

 

**
**

·     Board:

  [Arduino Uno Wi-Fi Rev2](https://www.arduino.cc/en/Guide/ArduinoUnoWiFiRev2)

 

·     Sensors: 

•    [DS1820B One-wire Water Temperature Sensor ](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf) 

•    [Arduino Ultrasonic Sensor ](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) 

•    [E-201-C PH Sensor](https://github.com/e-Gizmo/PH-Sensor-E-201-C/blob/master/PH Sensor E-201-C.pdf) 

 

·     Actuator:

​     [RGB 16x2 LCD Display ](https://www.arduino.cc/documents/datasheets/LCDscreen.PDF)

 

·      Power Supply:  

  [Power Squad Power Bank ](https://www.amazon.co.uk/Power-Squad-POWSQHM05-PowerSquad-Powerbank-Black/dp/B07VXQ8CFR) 

 

 

·     Data Aggregation:    

 [Raspberry Pi 4 Computer ](https://datasheets.raspberrypi.com/rpi4/raspberry-pi-4-datasheet.pdf)

-  Software:

•    [Arduino IDE](https://www.arduino.cc/en/software) 

•    [MQTT Explorer](http://mqtt-explorer.com/) 

•    [Raspberry Pi Imager](https://www.raspberrypi.com/software/) 

•    [Grafana](https://grafana.com/) 

•    [Fusion 360](https://www.autodesk.co.uk/products/fusion-360/overview)

•    [Prusaslicer](https://www.prusa3d.com/page/prusaslicer_424/)  



 

### Circuit

The prototype is based on one Arduino microcontroller, three sensors and one LCD connected in parallel. The schematic of the circuit is shown below.

![Diagram  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image002.jpg)

Figure 2. Schematic of the circuit

![img](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image003.jpg)

Figure 3. Real wiring

### Calibration

The DS1820B temperature and ultrasonic sensors are already calibrated to fetch temperature and distance data directly.

 

Much effort has been put into calibrating the PH sensor since the analogue signal generated from the sensor probe is too small, and the voltage for alkaline solutions are negative numbers, which are not able to be read by Arduino analogue pin and converted into solid PH value. To get meaningful readings, a PCB board is used to pre-process data before the probe sending signals to the analogue pin and Arduino by doing: 1. [Level shift the detected voltage ](https://daycounter.com/Circuits/OpAmp-Level-Shifter/OpAmp-Level-Shifter.phtml) (Non-Inverting Op-Amp Level Shifter - Daycounter, 2022) so that all readings are positive numbers and 2. amplify the shifted signal by three times. Then, the linear function between PH and mV will be defined by calibrating with various acid and alkaline solutions.

![A picture containing text, indoor  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image004.jpg)

Figure 4. Calibrate PH sensor with vinegar, coke, pure water, coffee, liquid soap and laundry liquid

![A picture containing person, indoor  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image005.jpg)

Figure 5. The alkaline solution (liquid soap) outcomes a small and negative value (-0.7 mV) when using a multimeter to measure the voltage at the sensor probe

 



#### Amplify the original millivoltage signal and level shift up using PCB board

The PCB board contains a voltage follower circuit and a non-inverting amplifier. 

For the voltage follower circuit, suppose the resistance of the potentiometer is Rx kΩ, according to the voltage divider rule (Monk, 2017, p20), 

The shifted voltage Vout = Vin + Rx * V/R2        (Equation 1)

Where V= 2.5V and R2 = 5.1kΩ

 

For the non-inverting amplifier, we have

Vout = Vin * (1+R2/R1)  (Monk, 2017, p322)

Where R2 = R4 = 20kΩ, R1 = R 5 = 10kΩ

Thus, Vout = 3 * Vin              (Equation 2)

Combining the equation1 and 2, the input and output voltage of PCB should follow this relationship:

Vout = 3 * ( Vin + 0.49 * Rx)      (Equation 3)

A 50kΩ potentiometer is used in this project.  

![Diagram, schematic  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image006.jpg)

Figure 6. PCB board circuit for PH sensor (CASA0016-Individual-Prototype/User Manual PH Sensor.pdf at main · mandymadongyi/CASA0016-Individual-Prototype, 2022)

 

#### Converting Voltage to PH 

#### Since voltage value is pre-processed for Arduino, Figure 1 mapping is no longer valid to convert PH value from voltage. Thus, a liner function is required to do the converting. Suppose the PH value of vinegar, water, and soap liquid for the experiment are 2.5, 7.0 and 10.0, respectively.

Based on analogue input from Arduino serial plotter, PH 2.5 liquid outcomes 610mV, while PH 7 liquid outcomes 480mV. Assume PH = a * voltage + b, then 

2.5 = 610 * a + b 

7.0 = 480 * a + b

Thus PH = -0.03461518 * voltage + 23.6153846 (Equation 4)

The lab also observed that PH 10 liquid outcomes 382mV approximately fit the prediction from Equation 4 (393.3356mV), proving the liner function valid.  



 

![Chart, line chart  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image007.png)

Figure 7. Fitting equation and a line graph of the linear relation between PH and voltage

### Basic Sketch

[Here](https://github.com/mandymadongyi/CASA0016-Individual-Prototype/blob/main/Code/final_to_upload/final_to_upload.ino) is the link to a basic standalone Arduino sketch that monitors and visualises the water features, incorporating the earlier calibration. In addition, it can be reused by adding more sensors like water turbidity or changing transmit protocols, including Bluetooth/LoRa/HTTP, as extensions. 

![Graphical user interface, text, application, email  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image008.png)

Figure 8. Calculating water level and temperature, PH value and print on LCD

![Graphical user interface, text, application  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image009.png)

Figure 9. Connecting and Sending data to MQTT



![img](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image010.png)

Figure 10. Telegraf configuration in influx DB in raspberry pi for Grafana display

 

## Wrapping up

An enclosure model is designed using Fusion 360, and 3D printed out for the system. On the bottom, front and top surfaces, there are holes for the ultrasonic sensor, LCD screen, sensor probes and power bank USB cable respectively. There are two holes on the side of the box for transport. 

![Diagram, engineering drawing  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image012.png)

Figure 11. 3D Model of the enclosure



![A picture containing indoor, wall, office  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image014.png)

 

Figure 12. 3D printed box with feet made from recycled materials (cupboard and plastic bottle tops) to protect the bottom ultrasonic sensor when placing on the floor

## Prototype Demonstration 

[Here](https://youtu.be/6DnR-BQpXQE) is a link to a video demonstrating how the system works. 

![A picture containing text, indoor, desk  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image015.jpg)

Figure 13. Final prototype

![img](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image016.png)

Figure 14. Grafana Visualisation

 

# Reflection and limitations

Overall, this device works well with remotely tracking and visualising water level, temperature, and PH value data and is somehow ready to be distributed in the wild. Lessons learnt regarding calibrating sensors, packaging and developing systems with a mix of actuators plus a range of sensors are precious.  Limitations exist since standard PH calibration liquid is absent for accurate PH value, and the PH sensor user manual is not detailed enough to provide reliable measurement, calibration has to be repeated every week. The 3D printed enclosure offers less protection than IP-rated enclosures when deploying at the riverside. Besides, the power budget is crucial to ensure the device is self-managing and survives in the wild.

 

## Challenges and Key Learnings

![Text  Description automatically generated with medium confidence](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image017.png)

Figure 15. Summary of key challenges and solutions



 

## Limitations

#### Power budget 

The power budget is an essential concession of a self-managing and long-time survival device when deploying in the wild. In the lab, the system's total power consumption is calculated using the bench power supply, which indicates 95mA and 73mA current when using MQTT or not. Take the average number 84 mA for estimation, the system runs approximately 30 hours since it uses a 5 Volt battery with capacity of 2500 mAh. As the water features won’t change as frequently as per second, the frequency of sending data through MQTT can be adjusted to lower down power budget.

Besides, a PIR motion sensor is useful for controlling the LCD screen, only turning it on when someone is around. Lora and large capacity batteries are also helpful to ensure longer endurance time.

​        ![A close-up of a control panel  Description automatically generated with low confidence](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image019.png)![A close-up of a control panel  Description automatically generated with low confidence](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image021.png)![A picture containing text, indoor, different  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image023.png)![A picture containing text, indoor, game  Description automatically generated](file:////Users/dongyima/Library/Group%20Containers/UBF8T346G9.Office/TemporaryItems/msohtmlclip/clip_image025.png) 

Figure 16. Measuring current when sending MQTT data

Figure 17. Measuring current when not sending MQTT data

Figure 18. Measuring current when sensors are not working

Figure 19. Measuring current when sensors are not working and LCD turned off

 



### Data Accuracy 

Data accuracy cannot be guaranteed in large-scale sensor devices since no battery is perfect for providing a constant 5V supply to each sensor. Also, Package dropping happens in internet transmitting. For PH sensor, current calibration might not be solid for long-term use. However, data accuracy can be improved by testing three times and choosing the average number as output.

 

## Future potentialities

#### Data Visualisation

Other visualisation tools (Echart, RAWGraphs, ChartBlocks, Tableau etc.) are recommended in the future. The Grafana dashboard is not fully utilised [[MI1\]](#_msocom_1) in this project yet, leaving a potential of exploring more features of Grafana, including tracking, analysing time-series databases and wrapping up outcomes into insightful reports and websites.  

 

#### System Design 

When sensing water level, the reflection of sunlight and passing boats may cause inaccurate readings. The ultrasonic sensor detection area is only 4 meters, so better design is needed to improve water level sensing. Also, it’s better to separate water quality and level into two parts since only PH and water temperature sensors have probes to go into the water.

 

# Reference:

\1.   Arduino Getting Started. 2022. *Arduino - Temperature Sensor | Arduino Tutorial*. [online] Available at: <https://arduinogetstarted.com/tutorials/arduino-temperature-sensor> [Accessed 3 January 2022].

\2.   Arduino Getting Started. 2022. *Arduino - Ultrasonic Sensor | Arduino Tutorial*. [online] Available at: <https://arduinogetstarted.com/tutorials/arduino-ultrasonic-sensor> [Accessed 3 January 2022].

\3.   GitHub. 2022. *CASA0016-Individual-Prototype/User Manual PH Sensor.pdf at main · mandymadongyi/CASA0016-Individual-Prototype*. [online] Available at: <https://github.com/mandymadongyi/CASA0016-Individual-Prototype/blob/main/References/User%20Manual%20PH%20Sensor.pdf> [Accessed 3 January 2022].

\4.   Traditionaloven.com. 2022. *pH Voltage | Acidity, Alkalinity equals Volts calculator*. [online] Available at: <https://www.traditionaloven.com/conversions_of_measures/ph-voltage.html> [Accessed 3 January 2022].

\5.   Arduino Getting Started. 2022. *Arduino - Ultrasonic Sensor - LCD | Arduino Tutorial*. [online] Available at: <https://arduinogetstarted.com/tutorials/arduino-ultrasonic-sensor-lcd> [Accessed 3 January 2022].

\6.   Docs.arduino.cc. 2022. [online] Available at: <https://docs.arduino.cc/tutorials/uno-wifi-rev2/uno-wifi-r2-mqtt-device-to-device> [Accessed 3 January 2022].

\7.   Workshops.cetools.org. 2022. *Plant-Monitor*. [online] Available at: <https://workshops.cetools.org/codelabs/CASA0014-2-Plant-Monitor/index.html?index=..%2F..casa0014#12> [Accessed 3 January 2022].

\8.   Daycounter.com. 2022. *Non-Inverting Op-Amp Level Shifter - Daycounter*. [online] Available at: <https://daycounter.com/Circuits/OpAmp-Level-Shifter/OpAmp-Level-Shifter.phtml> [Accessed 8 January 2022].

\9.   Monk, S., 2017. *Electronics cookbook*. 1st ed. " O'Reilly Media, Inc.".

 

 

------



 [[MI1\]](#_msoanchor_1)*utilized