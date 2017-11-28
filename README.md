# Walking-Quadruped-Reinforcement-Learning
The objective of the project was to create a mini four-legged robot that learns to walk in a straight line towards a target (ex. a wall). 
The main motivation behind this project was to design an intelligent control method to make a simple quadruped learn to walk on its own, without having to hard-code the leg actuations.

## Control Method
A reinforcement learning approach known as Q-learning was implemented to help the robot learn the optimal leg actuations required to move forward. This act of moving forward is based on the robot’s current distance from the target, and higher rewards are assigned to state transitions that reduce this distance. Robot state action pairs are evaluated for multiple learning episodes until convergence to an optimal policy, which in turn guides the robot's learned gait.

## Parts and Components
1. Microcontroller: **MSP432P401R Launchpad (x 1)** 
2. Leg actuators: **1.5 kg-cm (torque) micro-servo motors (x 8)** 
3. Sensor: **HC-SR04 ultrasonic sensor (x 1)**
4. Power supply: **9V DC battery**
5. Voltage regulator: **L7805CV**
6. Electronics housing platform: **Particle board (4in x 6in)**
7. Quadruped skeleton and limbs: **3D printed components**

## Progress so far
1. 3D printed parts and assembled 
2. Ultrasonic sensor data tested (may require filtering).
3. Finished wiring up circuit and completed build
![main frame 1](Progress-Media/FinalBuild22.jpeg?raw=true "Finished Build 1")	
![main frame 2](Progress-Media/FinalBuild2.JPG?raw=true "Finished Build 2")	
![main frame 3](Progress-Media/block_diagram.JPG?raw=true "Block Diagram")
4. Overall source code completed!	


## TODO
* Debug and gather data!
