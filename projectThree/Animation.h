#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h"
#include "Ship.h"
#include "Worker.h"

class Animation
{
    static const std::pair<int, int> coordinates_port;
    int width_window;
    int height_window;
    std::thread thread_check_exit;
    std::vector<std::unique_ptr<Ship>> ships;
    std::vector<std::unique_ptr<Worker>> workers;
    bool exit;
    
public:
    Animation();
    ~Animation();
    
    void start();
    void catchExit();
    void drawScene();
};
#endif // ANIMATION_H
