#ifndef WORKER_H
#define WORKER_H

#include "common.h"

class Worker
{
    std::pair<int, int> coordinates_start;
    std::pair<int, int> coordinates;
    std::pair<int, int> coordinates_previous;
    std::chrono::milliseconds speed;
    bool end_thread;
    std::thread thread_worker;
    int speed_of_unload;
    
    void setRandomSpeedOfUnload();
    void draw();
    void doMove(std::pair<int, int> step);
    void goToPort(std::pair<int, int> coorStop);
    void goToStart();
    
public:
    Worker(std::pair<int, int> coor);
    ~Worker();

    void startThread();
    void stopThread();
    void moveWorker();
};
#endif // WORKER_H
