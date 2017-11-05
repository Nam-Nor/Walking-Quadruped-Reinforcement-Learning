# Walking-Quadruped-Reinforcement-Learning
The objective of the project will be to create a mini four-legged robot that will learn to walk in a straight line towards a target (ex. a wall) on its own. The main motivation behind this project is to design an intelligent control method to make a simple quadruped walk on
various surfaces, without having to hard-code the leg actuations.

## Control Method
I intend on using a reinforcement learning approach known as Q-learning to help the robot learn the optimal leg actuations required to move forward. This act of moving forward will be based on the robot’s current distance from the target. Artiﬁcial neural networks will be used to map robot states to the parameters that will be fed into the Q-learning algorithm, which in turn will make the robot learn to walk.

## Components
The Microcontroller used is the MSP432P401R Launchpad. Each of the four robot legs is actuated by two 1.5 kg-cm (torque) micro-servo motors. An HC-SR04 ultrasonic sensor detects the robot’s remaining distance to the target object. The micro-controller, power supply and other circuit components are supported on a wooden platform, which is further supported by the main 3-D printed frame comprised of the joints and limbs.

## Progress so far
1. 3D printed parts and assembled 
![main frame](Progress-Media/Week1Build.jpeg?raw=true "Unfinished Build")	
2. Ultrasonic sensor data tested (may require filtering).

## TODO
* Test motor driver and make sure servos work
* Finish wiring up circuit and complete build
* Write up source code
