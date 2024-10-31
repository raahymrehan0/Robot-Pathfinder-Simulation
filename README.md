# Robot Pathfinder Simulation

This C program simulates a robot navigating a grid-based arena to collect randomly placed markers and then deposit them at a designated corner. The program uses a graphics library to visually represent the robot's movement, orientation, and actions within the arena. By randomly generating arena size and marker positions, each simulation run provides a unique navigation challenge, showcasing principles of autonomous movement and control.

## Features

- **Random Arena Size**: The arena dimensions vary each run.
- **Marker Collection System**: The robot finds and collects up to four randomly placed markers.
- **Marker Deposit at Corner**: After collecting markers, the robot moves to a designated corner to drop them.
- **Directional Movement**: The robot moves and rotates in four primary directions.
- **Real-Time Graphics Display**: Visualizes the robot’s position, arena boundaries, grid, and markers.

## Prerequisites & Setup

- **C Compiler**: e.g., GCC.
- **Graphics Library**: Requires a compatible graphics library (`graphics.h`).

To compile and run the program:

```bash
gcc -o RoboPathFinder RoboPathFinder.c graphics.c

./RoboPathFinder | java -jar drawapp-4.0.jar
```



## Function Overview

```c
// Generates random arena dimensions.
void randomArenaSize();

// Initializes arena, grid lines, and boundaries.
void initArena();

// Places up to four markers in random positions.
void initMarker(coordinate *marker);

// Sets robot’s starting position and direction.
void initRobot(myRobot *robot);

// Renders robot’s position and orientation on the grid.
void drawRobot(myRobot *robot);

// Moves the robot forward if within bounds.
void forward(myRobot *robot);

// Rotates the robot 90 degrees to the left.
void left(myRobot *robot);

// Rotates the robot 90 degrees to the right.
void right(myRobot *robot);

// Checks if robot is at a marker and returns its index.
int atMarker(myRobot *robot, coordinate *marker);

// Removes the marker after the robot has picked it up.
void pickUpMarker(coordinate *marker);

// Drops the markers at the corner.
void dropMarkers(myRobot *robot);

// Moves the robot to the corner.
void moveToCorner(myRobot *robot);

// Moves the robot to a specific marker.
void findMarker(myRobot *robot, coordinate *marker);

// Collects markers in order.
void findMarkers(myRobot *robot, coordinate *marker);
```

## Usage Instructions

- Launch the program to view the simulation.
- Observe the robot as it navigates the arena, collects markers, and deposits them at the corner.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature/YourFeature`).
3. Make your changes and commit them (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a pull request.

## Acknowledgements

- Thanks to the developers of the graphics library used in this project.
- Special thanks to everyone who contributed to the code and documentation.
