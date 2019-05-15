#include "Worker.h"

Worker::Worker(std::pair<int, int> coor) : coordinates_start{coor}, coordinates{coor}, end_thread{false}
{
    speed = std::chrono::milliseconds(std::rand()%200 + 200);
    setRandomSpeedOfUnload();
}

Worker::~Worker()
{
    thread_worker.join();
}

void Worker::startThread()
{
    thread_worker = std::thread(&Worker::moveWorker, this);
}

void Worker::stopThread()
{
    end_thread = true;
}

void Worker::moveWorker()
{
    std::shared_ptr<PortShip> curr_port;
    int time_to_unload;
    draw();  
    
    while (!end_thread)
    {
        curr_port = nullptr;
        std::unique_lock<std::mutex> lk(curr_port->mutex_worker, std::defer_lock);
        // wait to ship notify
        {
            std::unique_lock<std::mutex> lck(mutex_cv_workers);
            cv_workers.wait(lck, checkIfWorkerIsNeed);
            // if notify, check ports
            for (auto &port : ports)
            {
                if (port->is_ship && port->mutex_worker.try_lock())
                {
                    curr_port = port;
                    curr_port->is_worker = true;
                    break;
                }
            }
        } 
        
        // go to port, unload ship, and notify 
        goToPort(curr_port->coordinates_worker);
        {
            std::lock_guard<std::mutex> lck(curr_port->mutex_cv_unload);
            time_to_unload = 1000  * (curr_port->capacity_to_unload / speed_of_unload);
            std::this_thread::sleep_for(std::chrono::milliseconds(time_to_unload));
        }
        curr_port->cv_unload.notify_one();
        
        // unlock mutex, and go to waiting spot
        curr_port->is_worker = false;
        curr_port->mutex_worker.unlock();
        goToStart();
    }
    
    mvprintw(coordinates.first, coordinates.second, " " );
    refresh();
}

void Worker::setRandomSpeedOfUnload()
{
    speed_of_unload = (std::rand()%300) + 50;
}
    
void Worker::draw()
{
    std::lock_guard<std::mutex> lg(mutex_screen);
    mvprintw(coordinates_previous.first, coordinates_previous.second, " " );
    mvprintw(coordinates.first, coordinates.second, "o" );
    refresh();
}
    
void Worker::doMove(std::pair<int, int> step)
{
    coordinates_previous = coordinates;
    coordinates.first += step.first;
    coordinates.second += step.second;
}

void Worker::goToPort(std::pair<int, int> coorStop)
{
    int half_way = (coordinates.second + coorStop.second) / 2;
    do{
        doMove({0, -1});
        draw();
        std::this_thread::sleep_for(speed);
    } while(coordinates.second > half_way);
    
    while (coordinates.first != coorStop.first)
    {
        if (coordinates.first - coorStop.first < 0)
            doMove({1, 0});
        else
            doMove({-1, 0});
        draw();
        std::this_thread::sleep_for(speed);
    }
    
    do{
        doMove({0, -1});
        draw();
        std::this_thread::sleep_for(speed);
    } while(coordinates.second > coorStop.second);
}

void Worker::goToStart()
{
    int half_way = (coordinates.second + coordinates_start.second) / 2 + 1;
    do{
        doMove({0, 1});
        draw();
        std::this_thread::sleep_for(speed);
    } while(coordinates.second < half_way);
    
    while (coordinates.first != coordinates_start.first)
    {
        if (coordinates.first - coordinates_start.first < 0)
            doMove({1, 0});
        else
            doMove({-1, 0});
        draw();
        std::this_thread::sleep_for(speed);
    }
    
    do{
        doMove({0, 1});
        draw();
        std::this_thread::sleep_for(speed);
    } while(coordinates.second < coordinates_start.second);
}
