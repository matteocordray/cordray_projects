# Quadruped Simulator Project

## Folder Structure

This project was intended to help simulate a four-legged robot in Python. The project uses pybullet, a physics engine, to simulate the robot. I simulated the walking gait that was implemented as well as the balancing. Here I will outline how the code works and what I did to fix any bugs that arised.

Here is the folder structure of the project:

```bash
├───quadruped_simulator # Main folder
    ├───code # Folder containing all the code
        ├───examples # Folder containing a few examples
        ├───lib # Folder includes representations of the quadruped, such as DAE, URDF, and STL files
        ├───matlab # Folder containing the scripts for running the simulation
        ├───src # Folder containing main Python files
            ├───calc # Contains the calculations for the quadruped, such as the leg and rotation angles needed for the gait
        ├───tests # Folder containing test files where I changed some values and played with other functions in pybullet.
    ├───docs # Folder containing a document on getting started with pybullet
```

Inside each folder, there will be a README.md file that will explain what is in the folder and how to use it.

This project was fun to explore pybullet (despite weird documentation...) and to learn about the physics engine. Plus, seeing a robot you've already built in real life move around in a simulation is pretty cool.

## Getting Started

To get started, you will need to install `pybullet`. You can do this by running the following command (if `pip` is already installed):

```bash
pip install pybullet
```

You will also need to install `numpy`, which is used for calculations. You can install it by running the following command:

```bash
pip install numpy
```

## Running the Simulation

To run the simulation, navigate to the following folder:

```bash
cd quadruped_simulator\code\src
```

Then, run the following command:

```bash
python3 main.py
```

or if you are using Windows:

```bash
python main.py
```
Other files in the `src` folder are used for testing and debugging. You can run them by changing the name of the file in the `main.py` file.

When you run the main.py folder, the robot will start automatically walking

***NOTE: SOME FILE PATHS MAY BE BROKEN WHEN REORGANIZING FOLDER STRUCTURE FOR THIS REPOSITORY***