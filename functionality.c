#include <stdlib.h>     // rand() and srand()
#include <math.h>       // abs() function
#include "graphics.h"  
#include "functionality.h" 

const int width = 1000;
const int height = 1000;
const int gridSize = 50;
const int arenaStart = 150;
const int minimumArenaSize = 500;
const int sleepTime = 200;

int arenaEnd[2];
int wallConnected[MAX_OBSTACLES] = {0};
int homeTile[2];

markers marker[MAX_MARKERS];
coordinate obstacle[MAX_OBSTACLES];
myRobot theRobot;

void randomArenaSize()
{
    arenaEnd[0] = minimumArenaSize + (rand() % ((width - arenaStart - 500) / gridSize + 1)) * gridSize;
    arenaEnd[1] = minimumArenaSize + (rand() % ((height - arenaStart - 600) / gridSize + 1)) * gridSize;
}


void drawRobot(myRobot *robot)
{
    foreground();
    setColour(blue);
    // check the direction and display the appropriate image
    switch (robot->direction)
    {
        case 'N':

            displayImage("robot_north.jpg", robot->x - gridSize / 2 + 1, robot->y - gridSize / 2 + 1);
            break;
    
        case 'E':
            displayImage("robot_east.jpg", robot->x - gridSize / 2 + 1, robot->y - gridSize / 2 + 1);
            break;
    
        case 'S':
            displayImage("robot_south.jpg", robot->x - gridSize / 2 + 1, robot->y - gridSize / 2 + 1);
            break;

        case 'W':
            displayImage("robot_west.jpg", robot->x - gridSize / 2 + 1, robot->y - gridSize / 2 + 1);
            break;
    }
   
}

void randomDirection(myRobot *robot)
{
    int direction = rand() % 4;

    switch (direction)
    {
        case 0:

            robot->direction = 'N';
            break;

        case 1:

            robot->direction = 'S';
            break;

        case 2:

            robot->direction = 'E';
            break;

        case 3:

            robot->direction = 'W';
            break;
    }
}

void initRobot(myRobot *robot)
{
    int valid = 0;
    while (!valid) {
        robot->x = arenaStart + (rand() % ((arenaEnd[0] - arenaStart - gridSize) / gridSize)) * gridSize + gridSize / 2;
        robot->y = arenaStart + (rand() % ((arenaEnd[1] - arenaStart - gridSize) / gridSize)) * gridSize + gridSize / 2;

        // Ensure robot doesn't start on a marker or obstacle
        if (!isPositionTaken(robot->x, robot->y, marker, MAX_MARKERS, obstacle, MAX_OBSTACLES)) {
            valid = 1; // Position is free
        }
    }
    randomDirection(robot);
    drawRobot(robot);
    homeTile[0] = robot->x;
    homeTile[1] = robot->y;
}

void drawHomeTile(int x, int y)
{
    setColour(pink);
    fillRect(x - gridSize / 2, y - gridSize / 2, gridSize, gridSize);
}

void left(myRobot *robot)
{
    switch (robot->direction)
    {
        case 'N':

            robot->direction = 'W';
            break;

        case 'W':

            robot->direction = 'S';
            break;

        case 'S':

            robot->direction = 'E';
            break;

        case 'E':

            robot->direction = 'N';
            break;
    }
}

void right(myRobot *robot)
{
    switch (robot->direction)
    {
        case 'N':
            robot->direction = 'E';
            break;
        case 'E':
            robot->direction = 'S';
            break;
        case 'S':
            robot->direction = 'W';
            break;
        case 'W':
            robot->direction = 'N';
            break;
    }
}

void forward(myRobot *robot)
{
    if (canMoveForward(robot))
    {
        foreground();
        clear(); // Clear the previous robot drawing
        switch (robot->direction) {
            case 'N':

                robot->y -= gridSize;
                break;

            case 'E':

                robot->x += gridSize;
                break;

            case 'S':

                robot->y += gridSize;
                break;

            case 'W':

                robot->x -= gridSize;
                break;
        }
        drawRobot(robot);
        sleep(sleepTime);

        // Check if arrived at a marker
        atMarker(robot, marker);
    }
}

int canMoveForward(myRobot *robot)
{
    int new_x = robot->x;
    int new_y = robot->y;

    // Determine the new position based on the current direction
    if (robot->direction == 'N') 
    {
        new_y -= gridSize;
        if (new_y < arenaStart + gridSize / 2)
            return 0; // Out of arena bounds
    }
    else if (robot->direction == 'E') 
    {
        new_x += gridSize;
        if (new_x > arenaEnd[0] - gridSize / 2)
            return 0;
    }
    else if (robot->direction == 'S') 
    {
        new_y += gridSize;
        if (new_y > arenaEnd[1] - gridSize / 2)
            return 0; 
    }
    else if (robot->direction == 'W') 
    {
        new_x -= gridSize;
        if (new_x < arenaStart + gridSize / 2)
            return 0;
    }

    // Check for obstacles at the new position
    for (int i = 0; i < MAX_OBSTACLES; i++) 
    {
        if (obstacle[i].x == new_x && obstacle[i].y == new_y) {
            return 0; // Obstacle detected
        }
    }

    return 1; // can move forward
}

void initGrid()
{
    setColour(gray);
    
    // Draw vertical grid lines
    for (int i = arenaStart + gridSize; i < arenaEnd[0]; i += gridSize)
    {
        for (int j = arenaStart; j <= arenaEnd[1] - gridSize; j += gridSize)
        {
            int obstacleLeft = -1, obstacleRight = -1;
            for (int n = 0; n < MAX_OBSTACLES; n++) 
            {
                if (obstacle[n].x == i - gridSize / 2 && obstacle[n].y == j + gridSize / 2 && wallConnected[n] == 1) 
                {
                    obstacleLeft = n;
                }
                if (obstacle[n].x == i + gridSize / 2 && obstacle[n].y == j + gridSize / 2 && wallConnected[n] == 1) 
                {
                    obstacleRight = n;
                }
            }
            if (!(obstacleLeft != -1 && obstacleRight != -1 && areAdjacent(obstacle[obstacleLeft], obstacle[obstacleRight]))) 
            {
                drawLine(i, j, i, j + gridSize);
            }
        }
    }

    // Draw horizontal grid lines
    for (int j = arenaStart + gridSize; j < arenaEnd[1]; j += gridSize)
    {
        for (int i = arenaStart; i <= arenaEnd[0] - gridSize; i += gridSize)
        {
            int obstacleTop = -1, obstacleBottom = -1;
            for (int n = 0; n < MAX_OBSTACLES; n++) 
            {
                if (obstacle[n].y == j - gridSize / 2 && obstacle[n].x == i + gridSize / 2 && wallConnected[n] == 1) 
                {
                    obstacleTop = n;
                }
                if (obstacle[n].y == j + gridSize / 2 && obstacle[n].x == i + gridSize / 2 && wallConnected[n] == 1) 
                {
                    obstacleBottom = n;
                }
            }
            if (!(obstacleTop != -1 && obstacleBottom != -1 && areAdjacent(obstacle[obstacleTop], obstacle[obstacleBottom]))) 
            {
                drawLine(i, j, i + gridSize, j);
            }
        }
    }
}

int isPositionTaken(int x, int y, markers *markersArray, int numMarkers, coordinate *obstaclesArray, int numObstacles) 
{
    // Check markers
    for (int i = 0; i < numMarkers; i++) 
    {
        if (markersArray[i].x == x && markersArray[i].y == y && markersArray[i].collected == 0) {
            return 1; // Position is taken by a marker
        }
    }
    // Check obstacles
    for (int i = 0; i < numObstacles; i++)
    {
        if (obstaclesArray[i].x == x && obstaclesArray[i].y == y) {
            return 1; // Position is taken by an obstacle
        }
    }
    return 0; // Position is free
}

void initMarker(myRobot *robot, markers *marker)
{
    setColour(green);
    for (int n = 0; n < MAX_MARKERS; n++) 
    {
        int valid = 0;
        while (!valid) 
        {
            marker[n].x = arenaStart + (rand() % ((arenaEnd[0] - arenaStart) / gridSize)) * gridSize + gridSize / 2;
            marker[n].y = arenaStart + (rand() % ((arenaEnd[1] - arenaStart) / gridSize)) * gridSize + gridSize / 2;
            marker[n].collected = 0;

            // Check for overlap with existing markers and obstacles
            if (!isPositionTaken(marker[n].x, marker[n].y, marker, n, obstacle, MAX_OBSTACLES)) 
            {
                valid = 1;
                fillRect(marker[n].x - gridSize / 2, marker[n].y - gridSize / 2, gridSize, gridSize);
            }
        }
    }
}

// check if two obstacles are adjacent
int areAdjacent(coordinate a, coordinate b) 
{
    return ((abs(a.x - b.x) == gridSize && a.y == b.y) || (abs(a.y - b.y) == gridSize && a.x == b.x));
}

void initObstacle(coordinate *obstacles)
{
    int obstacleQueue[MAX_OBSTACLES];
    int front = 0, rear = 0;
    for (int n = 0; n < MAX_OBSTACLES; n++) 
    {
        int valid = 0;
        while (!valid) 
        {
            obstacles[n].x = arenaStart + (rand() % ((arenaEnd[0] - arenaStart) / gridSize)) * gridSize + gridSize / 2;
            obstacles[n].y = arenaStart + (rand() % ((arenaEnd[1] - arenaStart) / gridSize)) * gridSize + gridSize / 2;

            // Check for overlap with existing markers and obstacles
            if (!isPositionTaken(obstacles[n].x, obstacles[n].y, marker, MAX_MARKERS, obstacles, n)) 
            {
                valid = 1; // Position is free
            }
        }
    }
    
    // Identify obstacles adjacent to walls and enqueue them
    for (int n = 0; n < MAX_OBSTACLES; n++) 
    {
        int adjacentToLeftWall = obstacles[n].x - gridSize / 2 <= arenaStart;
        int adjacentToRightWall = obstacles[n].x + gridSize / 2 >= arenaEnd[0];
        int adjacentToTopWall = obstacles[n].y - gridSize / 2 <= arenaStart;
        int adjacentToBottomWall = obstacles[n].y + gridSize / 2 >= arenaEnd[1];
    
        if (adjacentToLeftWall || adjacentToRightWall || adjacentToTopWall || adjacentToBottomWall) 
        {
            wallConnected[n] = 1;
            obstacleQueue[rear++] = n;
        }
    }
    
    // Find all connected obstacles using BFS
    while (front < rear) 
    {
        int current = obstacleQueue[front++];
    
        for (int n = 0; n < MAX_OBSTACLES; n++) 
        {
            if (wallConnected[n] == 0 && areAdjacent(obstacles[current], obstacles[n])) 
            {
                wallConnected[n] = 1;
                obstacleQueue[rear++] = n;
            }
        }
    }
    for (int n = 0; n < MAX_OBSTACLES; n++) 
    {
        if (wallConnected[n] == 1) 
        {
            // Set color to match the boundaries
            setRGBColour(168, 50, 74);
        }
        else 
        {
            setColour(black);
        }
    
        fillRect(obstacles[n].x - gridSize / 2, obstacles[n].y - gridSize / 2, gridSize, gridSize);
    }
}

void initArena(myRobot *robot)
{
    background();
    setRGBColour(168, 50, 74);
    fillRect(arenaStart - gridSize, arenaStart - gridSize, arenaEnd[0] - arenaStart + 2 * gridSize, arenaEnd[1] - arenaStart + 2 * gridSize);
    setColour(white);
    fillRect(arenaStart, arenaStart, arenaEnd[0] - arenaStart, arenaEnd[1] - arenaStart);
    initObstacle(obstacle);
    initMarker(robot, marker);
    drawHomeTile(homeTile[0], homeTile[1]);
    initGrid();
}

// Check if the robot is at a marker and return its index, otherwise -1
int atMarker(myRobot *robot, markers *marker)
{
    for (int i = 0; i < MAX_MARKERS; i++)
    {
        if (!marker[i].collected &&
            abs(robot->x - marker[i].x) <= gridSize / 2 &&
            abs(robot->y - marker[i].y) <= gridSize / 2)
        {
            pickUpMarker(&marker[i]);
            return i;
        }
    }
    return -1;
}

// Remove the marker after the robot has picked it up
void pickUpMarker(markers *marker)
{
    background();
    setColour(white);
    fillRect(marker->x - gridSize / 2, marker->y - gridSize / 2, gridSize, gridSize);
    initGrid();
    marker->collected = 1;
}


void dropMarkers(myRobot *robot)
{
    setColour(red);
    fillRect(robot->x - gridSize / 2, robot->y - gridSize / 2, gridSize, gridSize);
}

int atHomeTile(myRobot *robot)
{
    if (homeTile[0] == robot->x && homeTile[1] == robot->y)
    {
        return 1;
    }
    return 0;
}

void initializeQueue(Queue* q) 
{
    q->front = 0;
    q->rear = 0;
}

int isQueueEmpty(Queue* q) 
{
    return q->front == q->rear;
}

int enqueue(Queue* q, Node* node) 
{
    if (q->rear == MAX_QUEUE_SIZE) 
    {
        return -1; // Queue is full
    }
    q->items[q->rear++] = node;
    return 0;
}

Node* dequeue(Queue* q) 
{
    if (isQueueEmpty(q)) 
    {
        return NULL; 
    }
    return q->items[q->front++];
}

// check if a position is valid and not blocked
int isValid(int x, int y) 
{
    // Check arena bounds
    if (x < arenaStart || x > arenaEnd[0] || y < arenaStart || y > arenaEnd[1])
        return 0;
    
    // Check if the position is occupied by an obstacle
    for (int i = 0; i < MAX_OBSTACLES; i++) 
    {
        if (obstacle[i].x == x && obstacle[i].y == y)
            return 0;
    }
    
    return 1;
}

// Function to perform BFS and find the shortest path
Node** bfsPathfinding(int start_x, int start_y, int target_x, int target_y, int* pathLength) 
{
    Queue q;
    initializeQueue(&q);
    
    // Calculate the number of cells in the arena
    int numCellsX = (arenaEnd[0] - arenaStart) / gridSize + 1;
    int numCellsY = (arenaEnd[1] - arenaStart) / gridSize + 1;
    
    // allocate memory for the visited array
    int **visited = malloc(numCellsX * sizeof(int *));
    if (visited == NULL) 
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numCellsX; i++) 
    {
        visited[i] = calloc(numCellsY, sizeof(int));
        if (visited[i] == NULL) 
        {
            exit(EXIT_FAILURE);
        }
    }
    
    // Create the starting node
    Node* startNode = (Node*)malloc(sizeof(Node));
    startNode->x = start_x;
    startNode->y = start_y;
    startNode->parent = NULL;
    
    enqueue(&q, startNode);
    visited[(start_x - arenaStart) / gridSize][(start_y - arenaStart) / gridSize] = 1;
    
    Node* current = NULL;
    
    // Directions: North, East, South, West
    int dirX[4] = {0, gridSize, 0, -gridSize};
    int dirY[4] = {-gridSize, 0, gridSize, 0};
    
    while (!isQueueEmpty(&q)) 
    {
        current = dequeue(&q);
        
        if (current->x == target_x && current->y == target_y) 
        {
            break;
        }
        
        // Explore all four directions
        for (int i = 0; i < 4; i++) 
        {
            int new_x = current->x + dirX[i];
            int new_y = current->y + dirY[i];
            
            if (isValid(new_x, new_y)) 
            {
                int grid_x = (new_x - arenaStart) / gridSize;
                int grid_y = (new_y - arenaStart) / gridSize;
                
                if (!visited[grid_x][grid_y]) 
                {
                    visited[grid_x][grid_y] = 1;
                    
                    // Create new node
                    Node* neighbor = (Node*)malloc(sizeof(Node));
                    neighbor->x = new_x;
                    neighbor->y = new_y;
                    neighbor->parent = current;
                    
                    enqueue(&q, neighbor);
                }
            }
        }
    }

    // Calculate the path length
    *pathLength = 0;
    Node* temp = current;
    while (temp != NULL) 
    {
        (*pathLength)++;
        temp = temp->parent;
    }
    
    // Allocate memory for the path
    Node** path = (Node**)malloc((*pathLength) * sizeof(Node*));
    temp = current;
    for (int i = *pathLength - 1; i >= 0; i--) 
    {
        path[i] = temp;
        temp = temp->parent;
    }
    
    // Free visited array
    for (int i = 0; i < numCellsX; i++) 
    {
        free(visited[i]);
    }
    free(visited);
    
    return path;
}

void findMarker(myRobot *robot, markers *marker)
{
    int target_x = marker->x;
    int target_y = marker->y;
    int pathLength = 0;
    Node** path = bfsPathfinding(robot->x, robot->y, target_x, target_y, &pathLength);
    
    if (path == NULL || pathLength == 0) 
    {
        return;
    }
    
    for (int i = 1; i < pathLength; i++) 
    { // Start from 1 to skip the current position
        int next_x = path[i]->x;
        int next_y = path[i]->y;

        if (next_x > robot->x) 
        {
            while (robot->direction != 'E') 
            {
                right(robot);
            }
        }
        else if (next_x < robot->x) 
        {
            while (robot->direction != 'W') 
            {
                right(robot);
            }
        }
        else if (next_y > robot->y) 
        {
            while (robot->direction != 'S') 
            {
                right(robot);
            }
        }
        else if (next_y < robot->y) 
        {
            while (robot->direction != 'N') 
            {
                right(robot);
            }
        }
        forward(robot);
    }
    
    atMarker(robot, marker);
    
    // Free the allocated path nodes
    for (int i = 0; i < pathLength; i++) 
    {
        free(path[i]);
    }
    free(path);
}

// iterate through all markers
void findMarkers(myRobot *robot, markers *marker)
{
    for (int i = 0; i < MAX_MARKERS; i++)
    {
        if (!marker[i].collected)
        {
            findMarker(robot, &marker[i]);
        }
    }
}

void moveToHome(myRobot *robot)

{
    int target_x = homeTile[0];
    int target_y = homeTile[1];
    
    int pathLength = 0;
    Node** path = bfsPathfinding(robot->x, robot->y, target_x, target_y, &pathLength);
    
    if (path == NULL || pathLength == 0) 
    {
        return;
    }
    
    for (int i = 1; i < pathLength; i++) 
    { // Start from 1 to skip current position
        int next_x = path[i]->x;
        int next_y = path[i]->y;
        
        if (next_x > robot->x) 
        {
            while (robot->direction != 'E') 
            {
                right(robot);
            }
        }
        else if (next_x < robot->x) 
        {
            while (robot->direction != 'W') 
            {
                right(robot);
            }
        }
        else if (next_y > robot->y) 
        {
            while (robot->direction != 'S') 
            {
                right(robot);
            }
        }
        else if (next_y < robot->y) 
        {
            while (robot->direction != 'N') 
            {
                right(robot);
            }
        }
        
        forward(robot);
    }
    
    // Free the allocated path
    for (int i = 0; i < pathLength; i++) 
    {
        free(path[i]);
    }
    free(path);
}
