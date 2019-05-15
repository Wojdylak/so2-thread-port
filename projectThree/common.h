#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <ncurses.h>
#include <vector>
#include <utility>
#include <unistd.h>
#include <memory>
#include <chrono>
#include <time.h>
#include <thread>
#include <mutex>
#include <condition_variable>

struct PortShip {
    std::mutex mutex_ship;
    std::mutex mutex_worker;
    std::mutex mutex_cv_unload;
    std::condition_variable cv_unload;
    std::pair<int, int> coordinates_ship;
    std::pair<int, int> coordinates_worker;
    int capacity_to_unload;
    bool is_ship;
    bool is_worker;
    bool is_ship_sail;
    
    PortShip(std::pair<int, int> coor):
        coordinates_ship{coor}, coordinates_worker{coor}, is_ship{false}, is_worker{false}, is_ship_sail{false}
        {
            coordinates_ship.second -= 2;
            coordinates_worker.second += 1;
        }
};


extern std::vector<std::shared_ptr<PortShip>> ports;
extern std::mutex mutex_screen;

extern std::mutex  mutex_cv_workers;
extern std::condition_variable cv_workers;

extern std::mutex  mutex_cv_ships;
extern std::condition_variable cv_ships;

extern std::mutex  mutex_cv_exit;
extern std::condition_variable cv_exit;

bool checkIfWorkerIsNeed();
bool checkIfPortIsEmpty();

#endif // COMMON_H