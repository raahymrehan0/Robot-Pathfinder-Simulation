# Robot Pathfinder Simulation

This C program simulates a robot navigating a randomly sized and shaped grid-based arena to collect randomly placed markers and then deposit them at the hometile where the robot spawned. The program uses the graphics.h library to visually represent the robot's movement, orientation, and actions within the arena. By randomly generating marker and obstacle positions, each simulation run provides a unique navigation challenge, showcasing principles of autonomous movement and control. The algorithm used for this is Breadth-First Search

## Features

- **Random Arena Size**: The arena dimensions vary each run.
- **Marker Collection System**: The robot finds and collects randomly placed markers.
- **Marker Deposit at Home**: After collecting markers, the robot moves to a designated home to drop them.
- **Directional Movement**: The robot moves and rotates in four primary directions.
- **Real-Time Graphics Display**: Visualizes the robot’s position, arena boundaries, grid, and markers.
- **BFS Implementation**: Utilizes Breadth-First Search to determine the shortest path for the robot to navigate the arena efficiently.


To compile and run the program:

```bash
gcc -o "filename" main.c functionality.c graphics.c

./"filename" | java -jar drawapp-4.0.jar
```



# Function Documentation

## Robot Control Functions
`drawRobot(myRobot *robot)`
- Renders the robot on the arena with its current position and direction.

`initRobot(myRobot *robot)`
- Initializes a new robot instance with random starting position and direction.

`left(myRobot *robot)`
- Rotates the robot 90 degrees counterclockwise.

`right(myRobot *robot)`
- Rotates the robot 90 degrees clockwise.

`canMoveForward(myRobot *robot)`
- Checks if the robot can move forward without hitting obstacles or arena boundaries.
- Returns: 1 if movement is possible, 0 otherwise.

`forward(myRobot *robot)`
- Moves the robot one step forward in its current direction.

## Arena Functions
`randomArenaSize()`
- Generates random dimensions for the arena within specified bounds.

`initArena(myRobot *robot)`
- Initializes the game arena with given dimensions and places the robot.

`drawHomeTile(int x, int y)`
- Draws the home tile marker at specified coordinates.

`initGrid()`
- Initializes the grid for the arena.

## Position and Movement Functions
`areAdjacent(coordinate a, coordinate b)`
- Checks if two obstacles are adjacent to each other.
- Returns: 1 if adjacent, 0 otherwise.

`isPositionTaken(int x, int y, markers *markersArray, int numMarkers, coordinate *obstaclesArray, int numObstacles)`
- Verifies if a position is occupied by any game element.
- Returns: 1 if position is taken, 0 if free.

## Game Element Initialization
`initMarker(myRobot *robot, markers *marker)`
- Initializes a marker at a random valid position in the arena.

`initObstacle(coordinate *obstacle)`
- Places an obstacle at a random valid position in the arena.

`randomDirection(myRobot *robot)`
- Sets a random direction for the robot.

## Marker Interaction Functions
`atMarker(myRobot *robot, markers *marker)`
- Checks if robot is at same position as a marker.
- Returns: 1 if at marker, 0 otherwise.

`pickUpMarker(markers *marker)`
- Removes the marker from the arena.

`dropMarkers(myRobot *robot)`
- Drops collected markers at home.

`findMarker(myRobot *robot, markers *marker)`
- Implements BFS logic for robot to locate a single marker.

`findMarkers(myRobot *robot, markers *marker)`
- Implements iteration for robot to locate multiple markers.

`moveToHome(myRobot *robot)`
- Directs the robot back to its home position.

## Queue Management for Pathfinding
`initializeQueue(Queue* q)`
- Initializes an empty queue for pathfinding algorithms.

`isQueueEmpty(Queue* q)`
- Checks if the queue is empty.
- Returns: 1 if empty, 0 otherwise.

`enqueue(Queue* q, Node* node)`
- Adds a node to the back of the queue.
- Returns: 1 if successful, 0 if queue is full.

`dequeue(Queue* q)`
- Removes and returns the front node from the queue.
- Returns: Pointer to the dequeued node, NULL if queue is empty.

## Pathfinding Functions
`isValid(int x, int y)`
- Checks if given coordinates are within arena bounds.
- Returns: 1 if valid, 0 if invalid.

`bfsPathfinding(int start_x, int start_y, int target_x, int target_y, int* pathLength)`
- Implements Breadth-First Search algorithm for finding optimal path.
- Returns: Array of nodes representing the path from start to target.

## Usage Instructions

- Launch the program to view the simulation.
- Observe the robot as it navigates the arena, collects markers, and deposits them at the corner.
