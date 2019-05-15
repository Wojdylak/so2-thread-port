#ifndef SHIP_H
#define SHIP_H

#include "common.h"

class Ship
{
    static const std::pair<int, int> coordinates_start_ship;    
    std::pair<int, int> coordinates_wait;    
    std::pair<int, int> coordinates;
    std::pair<int, int> coordinates_previous;
    // true = vertical,  false = horizontal
    bool direction; // true = 
    bool direction_previous;
    std::chrono::milliseconds speed;
    bool end_thread;
    std::thread thread_ship;
    int capacity;
    
    void draw();
    void drawShip();
    void cleanShip();
    void setRandomCapacity();
    
    void doMove(int step, bool dir);
    void shipEnters();
    void shipEntersToPort(std::pair<int, int> coor_stop);
    void shipLeaves();
    
public:
    Ship(std::pair<int, int> coor);
    ~Ship();

    void startThread();
    void stopThread();
    void moveShip();
    
};
#endif // SHIP_H
