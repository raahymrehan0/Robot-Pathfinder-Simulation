#ifndef FUNCTIONALITY_H

#define MAX_MARKERS 9
#define MAX_OBSTACLES 15
#define MAX_QUEUE_SIZE 400 // (1000 * 1000) / (50 * 50) = 400 tiles maximum


typedef struct markers {
    int x;
    int y;
    int collected; // 0 if not collected, 1 if collected
} markers;

typedef struct coordinate {
    int x;
    int y;
} coordinate;

typedef struct myRobot {
    int x;
    int y;
    char direction;
} myRobot;

typedef struct Node {
    int x;
    int y;
    struct Node *parent;
} Node;

typedef struct Queue {
    Node *items[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

extern const int width;
extern const int height;
extern const int gridSize;
extern const int arenaStart;
extern const int minimumArenaSize;
extern const int sleepTime;

extern int arenaEnd[2];
extern int wallConnected[MAX_OBSTACLES];
extern int homeTile[2];

extern markers marker[MAX_MARKERS];
extern coordinate obstacle[MAX_OBSTACLES];
extern myRobot theRobot;

void drawRobot(myRobot *robot);
void randomArenaSize();
void initArena(myRobot *robot);
void drawHomeTile(int x, int y);
int areAdjacent(coordinate a, coordinate b);
void initGrid();
int isPositionTaken(int x, int y, markers *markersArray, int numMarkers, coordinate *obstaclesArray, int numObstacles);
void initMarker(myRobot *robot, markers *marker);
void initObstacle(coordinate *obstacle);
void randomDirection(myRobot *robot);
void initRobot(myRobot *robot);
void left(myRobot *robot);
void right(myRobot *robot);
void forward(myRobot *robot);
int canMoveForward(myRobot *robot);
int atMarker(myRobot *robot, markers *marker);
void pickUpMarker(markers *marker);
void dropMarkers(myRobot *robot);
void findMarker(myRobot *robot, markers *marker);
void findMarkers(myRobot *robot, markers *marker);
void moveToHome(myRobot *robot);
void initializeQueue(Queue* q);
int isQueueEmpty(Queue* q);
int enqueue(Queue* q, Node* node);
Node* dequeue(Queue* q);
int isValid(int x, int y);
Node** bfsPathfinding(int start_x, int start_y, int target_x, int target_y, int* pathLength);

#endif
