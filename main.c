#include <stdlib.h>
#include <time.h>
#include "graphics.h"
#include "functionality.h"

int main()
{
    srand(time(0));
    setWindowSize(width, height);
    randomArenaSize();
    initRobot(&theRobot);
    initArena(&theRobot);
    findMarkers(&theRobot, marker);
    moveToHome(&theRobot);
    dropMarkers(&theRobot);

    return 0;
}
