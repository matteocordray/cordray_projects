# Quadruped Simulator

This project was intended to help simulate a four-legged robot in Python. The project uses pybullet, a physics engine, to simulate the robot. I simulated the walking gait that was implemented as well as the balancing. Here I will outline how the code works and what I did to fix any bugs that arised.

Here is the folder structure of the project:

```bash
├───quadruped_simulator # Main folder
    ├───code # Folder containing all the code
        ├───examples # Folder containing a few examples
        ├───lib # Folder includes representations of the quadruped, such as DAE, URDF, and STL files
        ├───matlab # Folder containing the scripts for running the simulation
        ├───src # Folder containing the source code
            ├───calc # Contains the calculations for the quadruped, such as the leg and rotation angles needed for the gait
        ├───tests # Folder containing the tests
    ├───docs # Folder containing a document on getting started with pybullet
```
