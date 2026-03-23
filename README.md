# Teleoperated Micro-Assembly System with Haptic Feedback

![Research](https://img.shields.io/badge/Research-Robotics-red)
![Field](https://img.shields.io/badge/Field-Cobotics-blue)
![Topic](https://img.shields.io/badge/Topic-Teleoperation-green)
![Application](https://img.shields.io/badge/Application-Micro_Assembly-orange)

This project presents the design and implementation of a **human–machine interface (HMI) for teleoperated micro-assembly with haptic feedback**.

The system enables an operator to manipulate **sub-millimeter components remotely** using a portable **tele-tweezer master device**, which controls a robotic **micro-gripper slave system**.

This work was conducted from March 2020 to November 2020 at the **Institute for Intelligent Systems and Robotics (ISIR), Sorbonne University**, within the framework of the **COLAMIR project**.

Overview of the hardware platform: 

![Robot](media/overview_hardware_platform.png)

---

# Project Highlights

• Development of a **master–slave teleoperation system for micro-assembly**  
• Implementation of **high-speed UDP communication (220 Hz control loop)**  
• **Hysteresis modeling of piezoelectric micro-grippers** for force compensation  
• **Bilateral haptic coupling** enabling force feedback to the operator  

---

# Overview

Micro-assembly tasks involve manipulation of objects at **sub-millimeter scale**, such as components used in watchmaking, electronics, and biomedical devices.

These tasks are traditionally performed manually using **precision tweezers under microscopes**, requiring high dexterity and expertise.

This project proposes a **cobotic teleoperation system** combining:

- human expertise and dexterity
- robotic precision
- real-time haptic feedback

The goal is to **assist operators in precision manipulation while preserving human decision-making in the loop**.

---

# System Architecture

The teleoperation platform consists of four main subsystems:

- **Tele-tweezer** (master device)
- **ChronoLab micro-manipulation station** (slave device including micro-gripper, sensors and cameras)
- **Control computer**
- **OptiTrack motion tracking system**

The operator manipulates the tele-tweezer, which controls the robotic micro-gripper in real time.

Illustration of system architecture:
![System Architecture](media/control_loop_1.png)

---

# Hardware Platform

## Master Device – Tele-Tweezer

The master interface is a **handheld haptic tele-tweezer**, originally developed at ISIR and presented in [1].

The portable device designed to replicate the shape and functionality of traditional precision tweezers.

Features:

- strain gauges for deformation measurement
- force sensing
- DC motor for haptic feedback
- accelerometer
- embedded PIC32 microcontroller

The tele-tweezer allows intuitive control of the remote micro-gripper.

![Master Device](media/teletweezer3.png)

---

## Slave Device – Robotic Micro-Gripper

The slave interface is a robotic micro-assembly station used to manipulate micro-components.

This station is developed by Percipio Robotics and described in [1].

Features:

- piezoelectric micro-gripper
- strain gauges for force sensing
- precision positioning platform
- optical observation system

![Slave Device](media/microtweezer2.png)

---

# Research Contributions

## High-Speed Communication Architecture

The original system used **TCP communication**, limiting control frequency to about **40 Hz**.

To improve teleoperation responsiveness, a **UDP communication server** was implemented directly on the embedded controller.

Results:

- communication frequency increased to **≈220 Hz**
- command latency reduced to **7–9 ms**
- embedded control loop **≈2.6 ms**

---

## Master–Slave Motion Coupling

A real-time mapping was implemented between:

```
tele-tweezer opening/closing → actuation of micro-gripper (opening/closing) 
```

This enables intuitive remote manipulation of micro-scale objects through the following steps:

```
Operator
   ↓
Tele-tweezer (Master)
   ↓
UDP Communication
   ↓
Control Computer
   ↓
Robotic Micro-Gripper (Slave)
```

---

## Hysteresis Modeling of Piezoelectric Actuators

The micro-gripper uses **piezoelectric actuators**, which exhibit nonlinear hysteresis.

Experiments were conducted to:

- measure actuator response
- identify hysteresis behavior
- derive a polynomial hysteresis model

The model enables **force correction and improved gripping accuracy**.

---

## Bilateral Teleoperation with Haptic Feedback

A **bilateral coupling scheme** was implemented to enable real-time interaction between the master (tele-tweezer) and the slave (micro-gripper)..

When the micro-gripper contacts an object:

1. strain gauges measure interaction forces  
2. force information is transmitted to the master device  
3. a motor generates **haptic feedback on the tele-tweezer**

The operator can therefore **feel contact events remotely**, preventing excessive gripping forces.

A control loop was implemented and operates as shown in the following figure:

![Control Loop](media/control_scheme_6.png)

## Human–Machine Interface (HMI) Implementation

To enable real-time interaction between all subsystems, a custom graphical user interface (GUI) was developed using Qt (C++).

![Human–Machine Interface](media/Human–Machine_Interface.png)

The GUI is organized into several functional modules:

**ChronoLab Control Panel (Top-left)**: Connection to the ChronoLab micro-manipulation station; Real-time display of positioners state and sensor values (force, position, etc.).

**OptiTrack Tracking Panel (Top-center)**: Connection to the OptiTrack motion tracking system; Tracking of the 6-DoF motion of the master device in real time.

**Tele-Tweezer Control Panel (Top-right)**: USB communication via COM port; Displays embedded sensor data from the tele-tweezer...

**Debug & Manual Control Panel (Bottom-left)**: Provides tools for debugging communication issues, manual control of position and speed, etc...

**Coupling & Control Panel (Bottom-center)**: Core module for bilateral teleoperation; coupling activation between tele-tweezer and micro-gripper; control of the **haptic feedback loop** (asservissement in French);

This module directly implements the bilateral coupling strategy described in the previous section, enabling both real-time master–slave synchronization and force feedback rendering to the operator.

---

# Experimental Demonstration

Micro-assembly tasks were performed to validate the proposed **bilateral coupling scheme**.

### Hysteresis identification

![hysteresis](media/reference_curve_1.png)

### Comparison: Without vs With Haptic Feedback

**Dynamic demonstration (gif):**

![with feedback](media/VID_20201026_195506.gif)

(a) Without haptic feedback

![with feedback](media/VID_20201026_195316.gif)

(b) With haptic feedback

**Annotated comparison (figure):**

![with feedback](media/haptic_feedback_1.png)

**Explanation:**

(a) Without haptic feedback, the operator does not receive any information about the contact between the micro-gripper and the object. As a result, the tele-tweezer can continue to close even after the micro-gripper touches the object, which may lead to excessive gripping forces and potential damage to the micro-gripper.

(b) With haptic feedback, the operator receives a force feedback when the micro-gripper contacts the object. This feedback prevents further closing of the tele-tweezer and allows the operator to perceive the contact.

These results demonstrate that haptic feedback effectively prevents excessive gripping forces, and significantly improves manipulation safety and control in teleoperated micro-assembly tasks.

**Full demonstration video:**

https://youtu.be/DjOsX6Ir4xc

---

# Technologies Used

## Programming

- C
- C++

## Communication

- UDP socket communication
- TCP/IP networking

## Embedded Systems

- PIC32 microcontrollers
- MPLAB X IDE
- Microchip ICD3 debugger

## Software

- Qt Creator
- MATLAB (system identification and data analysis)

## Sensors and Hardware

- strain gauges
- force sensors
- piezoelectric actuators
- optical vision system
- OptiTrack motion tracking

---

# Applications

Potential applications include:

- watchmaking micro-assembly
- micro-electronics assembly
- biomedical micro-manipulation
- teleoperated precision robotics

---

# Project Context

This research was conducted within the project:

**COLAMIR – Agile Micro-Collaborative Robots for Ultra-Precision Assembly**

Funded by:

- French National Research Agency (ANR)
- Percipio Robotics

---

# References

[1] S. Sakr, T. Daunizeau, D. Reversat, S. Régnier, and S. Haliyo,
"A Handheld Master Device for 3D Remote Micro-manipulation",
IEEE Conference, 2019.

[2] Z. Zhang, Human–Machine Interface for Teleoperated Micro-Assembly, Master thesis, Sorbonne University, 2020.

# Author

Zibo Zhang  
PhD in Robotics  
IMT Atlantique / Université Grenoble Alpes
