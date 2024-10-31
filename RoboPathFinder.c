#include <stdio.h>      // Standard I/O library
#include <stdlib.h>     // rand() and srand()
#include <math.h>       // abs() function
#include <time.h>       // time() function
#include "graphics.h"   // Graphics library

const int width = 1000;
const int height = 1000;
const int gridSize = 50;
const int arenaStart = 200;
int arenaEnd[2];
const int sleepTime = 100;
#define MAX_MARKERS 4

typedef struct coordinate
{
    int x;
    int y;
    int collected; // 0 if not collected, 1 if collected
} coordinate;

coordinate marker[MAX_MARKERS];

typedef struct myRobot
{
    int x;
    int y;
    char direction;
} myRobot;

myRobot theRobot;

// Function declarations
void drawRobot(myRobot *robot);
void randomArenaSize();
void initArena();
void initGrid();
void initMarker(coordinate *marker);
void randomDirection(myRobot *robot);
void initRobot(myRobot *robot);
void drawRobot(myRobot *robot);
void left(myRobot *robot);
void right(myRobot *robot);
void forward(myRobot *robot);
int canMoveForward(myRobot *robot);
int atMarker(myRobot *robot, coordinate *marker);
void pickUpMarker(coordinate *marker);
void dropMarkers(myRobot *robot);
void moveToCorner(myRobot *robot);
void findMarker(myRobot *robot, coordinate *marker);
void findMarkers(myRobot *robot, coordinate *marker);

// Randomly generate the size of the arena
void randomArenaSize()
{
    arenaEnd[0] = 500 + (rand() % ((width - arenaStart - 700) / gridSize + 1)) * gridSize;
    arenaEnd[1] = 500 + (rand() % ((height - arenaStart - 700) / gridSize + 1)) * gridSize;
}

// Draw the robot in its current position
void drawRobot(myRobot *robot)
{
    int xPoints[3];
    int yPoints[3];
    if (robot->direction == 'N')
    {
        xPoints[0] = robot->x - gridSize / 2;
        xPoints[1] = robot->x;
        xPoints[2] = robot->x + gridSize / 2;
        yPoints[0] = robot->y + gridSize / 2;
        yPoints[1] = robot->y - gridSize / 2;
        yPoints[2] = robot->y + gridSize / 2;
    }
    else if (robot->direction == 'E')
    {
        xPoints[0] = robot->x - gridSize / 2;
        xPoints[1] = robot->x + gridSize / 2;
        xPoints[2] = robot->x - gridSize / 2;
        yPoints[0] = robot->y - gridSize / 2;
        yPoints[1] = robot->y;
        yPoints[2] = robot->y + gridSize / 2;
    }
    else if (robot->direction == 'S')
    {
        xPoints[0] = robot->x - gridSize / 2;
        xPoints[1] = robot->x;
        xPoints[2] = robot->x + gridSize / 2;
        yPoints[0] = robot->y - gridSize / 2;
        yPoints[1] = robot->y + gridSize / 2;
        yPoints[2] = robot->y - gridSize / 2;
    }
    else if (robot->direction == 'W')
    {
        xPoints[0] = robot->x + gridSize / 2;
        xPoints[1] = robot->x - gridSize / 2;
        xPoints[2] = robot->x + gridSize / 2;
        yPoints[0] = robot->y - gridSize / 2;
        yPoints[1] = robot->y;
        yPoints[2] = robot->y + gridSize / 2;
    }
    foreground();
    setColour(blue);
    fillPolygon(3, xPoints, yPoints);
}

// Set the robot to face a random direction
void randomDirection(myRobot *robot)
{
    int direction = rand() % 4;
    if (direction == 0)
        robot->direction = 'N';
    else if (direction == 1)
        robot->direction = 'S';
    else if (direction == 2)
        robot->direction = 'E';
    else
        robot->direction = 'W';
}

// Initialize the robot to its starting position
void initRobot(myRobot *robot)
{
    // Set the robot's x and y coordinates to a random position within the arena
    do
    {
        robot->x = (rand() % ((arenaEnd[0] - arenaStart - gridSize) / gridSize)) * gridSize + arenaStart + gridSize / 2;
        robot->y = (rand() % ((arenaEnd[1] - arenaStart - gridSize) / gridSize)) * gridSize + arenaStart + gridSize / 2;
    } while (robot->x == marker[0].x && robot->y == marker[0].y); // Ensure robot doesn't start on a marker
    // Set the robot's direction to a random direction
    randomDirection(robot);
    drawRobot(robot);
}

// Turn the robot left
void left(myRobot *robot)
{
    if (robot->direction == 'N')
        robot->direction = 'W';
    else if (robot->direction == 'W')
        robot->direction = 'S';
    else if (robot->direction == 'S')
        robot->direction = 'E';
    else if (robot->direction == 'E')
        robot->direction = 'N';
}

// Turn the robot right
void right(myRobot *robot)
{
    if (robot->direction == 'N')
        robot->direction = 'E';
    else if (robot->direction == 'E')
        robot->direction = 'S';
    else if (robot->direction == 'S')
        robot->direction = 'W';
    else if (robot->direction == 'W')
        robot->direction = 'N';
}

// Move the robot forward
void forward(myRobot *robot)
{
    if (canMoveForward(robot))
    {
        foreground();
        clear(); // Clear the previous robot drawing
        if (robot->direction == 'N')
            robot->y -= gridSize;
        else if (robot->direction == 'E')
            robot->x += gridSize;
        else if (robot->direction == 'S')
            robot->y += gridSize;
        else if (robot->direction == 'W')
            robot->x -= gridSize;
        drawRobot(robot);
        sleep(sleepTime);
    }
}

// Check if the robot can move forward
int canMoveForward(myRobot *robot)
{
    if (robot->direction == 'N' && robot->y - gridSize >= arenaStart + gridSize / 2)
        return 1;
    else if (robot->direction == 'E' && robot->x + gridSize <= arenaEnd[0] - gridSize / 2)
        return 1;
    else if (robot->direction == 'S' && robot->y + gridSize <= arenaEnd[1] - gridSize / 2)
        return 1;
    else if (robot->direction == 'W' && robot->x - gridSize >= arenaStart + gridSize / 2)
        return 1;
    else
        return 0;
}

// Initialize the grid in the arena
void initGrid()
{
    setColour(gray);
    for (int i = arenaStart + gridSize; i <= arenaEnd[0]; i += gridSize)
    {
        drawLine(i, arenaStart, i, arenaEnd[1]);
    }
    for (int i = arenaStart + gridSize; i <= arenaEnd[1]; i += gridSize)
    {
        drawLine(arenaStart, i, arenaEnd[0], i);
    }
}

// Draw the markers
void initMarker(coordinate *marker)
{
    setColour(gray);
    for (int n = 0; n < MAX_MARKERS; n++)
    {
        marker[n].x = arenaStart + (rand() % ((arenaEnd[0] - arenaStart) / gridSize)) * gridSize;
        marker[n].y = arenaStart + (rand() % ((arenaEnd[1] - arenaStart) / gridSize)) * gridSize;
        marker[n].collected = 0;
        fillRect(marker[n].x, marker[n].y, gridSize, gridSize);
    }
}

// Initialize the arena
void initArena()
{
    background();
    setRGBColour(168, 50, 74);
    fillRect(arenaStart - gridSize, arenaStart - gridSize,
             arenaEnd[0] - arenaStart + 2 * gridSize,
             arenaEnd[1] - arenaStart + 2 * gridSize);
    setColour(white);
    fillRect(arenaStart, arenaStart,
             arenaEnd[0] - arenaStart,
             arenaEnd[1] - arenaStart);
    initGrid();
}

// Check if the robot is at a marker and return its index, otherwise -1
int atMarker(myRobot *robot, coordinate *marker)
{
    for (int i = 0; i < MAX_MARKERS; i++)
    {
        if (!marker[i].collected &&
            abs(robot->x - (marker[i].x + gridSize / 2)) <= gridSize / 2 &&
            abs(robot->y - (marker[i].y + gridSize / 2)) <= gridSize / 2)
        {
            pickUpMarker(&marker[i]);
            return i;
        }
    }
    return -1;
}

// Remove the marker after the robot has picked it up
void pickUpMarker(coordinate *marker)
{
    background();
    setColour(white);
    fillRect(marker->x, marker->y, gridSize, gridSize);
    initGrid();
    marker->collected = 1;
}

// Drop the markers at the corner
void dropMarkers(myRobot *robot)
{
    setColour(red);
    fillRect(robot->x - gridSize / 2, robot->y - gridSize / 2, gridSize, gridSize);
}

// Move the robot to the corner
void moveToCorner(myRobot *robot)
{
    // Move horizontally to the left boundary
    while (robot->x > arenaStart + gridSize / 2)
    {
        if (robot->direction != 'W')
        {
            left(robot);
        }
        else
        {
            forward(robot);
        }
    }
    // Move vertically to the top boundary
    while (robot->y > arenaStart + gridSize / 2)
    {
        if (robot->direction != 'N')
        {
            left(robot);
        }
        else
        {
            forward(robot);
        }
    }
}

// Function to collect a specific marker
void findMarker(myRobot *robot, coordinate *marker)
{
    // Move horizontally towards the marker
    while (robot->x != marker->x + gridSize / 2)
    {
        if (robot->x < marker->x + gridSize / 2)
        {
            if (robot->direction != 'E')
            {
                right(robot);
            }
            else
            {
                forward(robot);
            }
        }
        else if (robot->x > marker->x + gridSize / 2)
        {
            if (robot->direction != 'W')
            {
                left(robot);
            }
            else
            {
                forward(robot);
            }
        }
    }

    // Move vertically towards the marker
    while (robot->y != marker->y + gridSize / 2)
    {
        if (robot->y < marker->y + gridSize / 2)
        {
            if (robot->direction != 'S')
            {
                right(robot);
            }
            else
            {
                forward(robot);
            }
        }
        else if (robot->y > marker->y + gridSize / 2)
        {
            if (robot->direction != 'N')
            {
                left(robot);
            }
            else
            {
                forward(robot);
            }
        }
    }
}

// Function to collect markers in order
void findMarkers(myRobot *robot, coordinate *marker)
{
    for (int i = 0; i < MAX_MARKERS; i++)
    {
        if (!marker[i].collected)
        {
            findMarker(robot, &marker[i]);
            pickUpMarker(&marker[i]);
        }
    }
}

int main()
{
    srand(time(0));
    setWindowSize(width, height);
    randomArenaSize();
    initArena();
    initMarker(marker);
    initRobot(&theRobot);

    // Collect all markers
    findMarkers(&theRobot, marker);
    pickUpMarker(&marker[0]);

    // Move to corner and drop all markers
    moveToCorner(&theRobot);
    dropMarkers(&theRobot);

    return 0;
}