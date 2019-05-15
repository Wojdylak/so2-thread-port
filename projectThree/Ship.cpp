#include "Ship.h"

const std::pair<int, int> Ship::coordinates_start_ship {0, 4};

Ship::Ship(std::pair<int, int> coor): coordinates_wait{coor}, end_thread{false}
{
    coordinates = {1, 1};
    direction = true;
    speed = std::chrono::milliseconds(std::rand()%200 + 200);
}

Ship::~Ship()
{
    thread_ship.join();
}

void Ship::startThread()
{
    thread_ship = std::thread(&Ship::moveShip, this);
}

void Ship::stopThread()
{
    end_thread = true;
}


void Ship::moveShip()
{
    std::shared_ptr<PortShip> curr_port;
    while (!end_thread)
    {
        curr_port = nullptr;
        std::unique_lock<std::mutex> lk(curr_port->mutex_ship, std::defer_lock);
        setRandomCapacity();
        shipEnters();
        
        // wait to empty port
        {
            std::unique_lock<std::mutex> lck(mutex_cv_ships);
            cv_ships.wait(lck, checkIfPortIsEmpty);
            // if notify , check ports
            for (auto &port : ports)
            {
                if (port->mutex_ship.try_lock())
                {
                    curr_port = port;
                    curr_port->is_ship_sail = true;
                    break;
                }
            }
        } 
        
        // go to port, notify workers and go to sleep
        shipEntersToPort(curr_port->coordinates_ship);
        curr_port->is_ship_sail = false;
        curr_port->is_ship = true;
        curr_port->capacity_to_unload = capacity;
        cv_workers.notify_all();
        {
            std::unique_lock<std::mutex> lck(curr_port->mutex_cv_unload);
            curr_port->cv_unload.wait(lck);
        }
        
        // unlock mutex, and notify ships
        curr_port->is_ship = false;
        curr_port->mutex_ship.unlock();
        cv_ships.notify_all();
        shipLeaves();
    }
}


void Ship::draw()
{
    std::lock_guard<std::mutex> lg(mutex_screen);
    cleanShip();
    drawShip();
    refresh();
}

void Ship::drawShip()
{
    if (direction){
        mvprintw((coordinates.first), coordinates.second - 1, "#" );
        mvprintw((coordinates.first), coordinates.second, "#" );
        mvprintw((coordinates.first), coordinates.second + 1, "#" );
    }
    else
    {
        mvprintw((coordinates.first - 1), coordinates.second, "#" );
        mvprintw((coordinates.first), coordinates.second, "#" );
        mvprintw((coordinates.first + 1), coordinates.second, "#" );
    }
}

void Ship::cleanShip()
{
    if (direction_previous){
        mvprintw((coordinates_previous.first), coordinates_previous.second - 1, " " );
        mvprintw((coordinates_previous.first), coordinates_previous.second, " " );
        mvprintw((coordinates_previous.first), coordinates_previous.second + 1, " " );
    }
    else
    {
        mvprintw((coordinates_previous.first - 1), coordinates_previous.second, " " );
        mvprintw((coordinates_previous.first), coordinates_previous.second, " " );
        mvprintw((coordinates_previous.first + 1), coordinates_previous.second, " " );
    }
}

void Ship::setRandomCapacity()
{
    capacity = (std::rand()%3000) + 1000;
}


void Ship::doMove(int step, bool dir)
{
    coordinates_previous = coordinates;
    direction_previous = direction;
    direction = dir;
    if (direction)
        coordinates.second += step;

    else
        coordinates.first += step;
}


void Ship::shipEnters()
{
    coordinates = coordinates_start_ship;
    
    // go to  waiting spot
    do{
        doMove(1, false);
        draw();
        std::this_thread::sleep_for(speed);
    } while(coordinates.first < coordinates_wait.first);
    
    do{
        doMove(1, true);
        draw();
        std::this_thread::sleep_for(speed);
    } while(coordinates.second < coordinates_wait.second);
}
    
void Ship::shipEntersToPort(std::pair<int, int> coor_stop)
{
    int half_way = (coordinates.second + coor_stop.second) / 2;
    
    do{
        doMove(1, true);
        draw();
        std::this_thread::sleep_for(speed);
    } while (coordinates.second < half_way);
    
    while (coordinates.first != coor_stop.first)
    {
        if (coordinates.first - coor_stop.first < 0)
            doMove(1, false);
        else
            doMove(-1, false);
        draw();
        std::this_thread::sleep_for(speed);
    }
    
    do{
        doMove(1, true);
        draw();
        std::this_thread::sleep_for(speed);
    } while (coordinates.second < coor_stop.second);
}
    
void Ship::shipLeaves()
{
    int cur_x = coordinates.second;

    do{
        doMove(-1, true);
        draw();
        std::this_thread::sleep_for(speed);
    } while (coordinates.second > cur_x - 10);
    
    do{
        doMove(-1, false);
        draw();
        std::this_thread::sleep_for(speed);
    } while (coordinates.first > -3);
}