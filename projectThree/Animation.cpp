#include "Animation.h"

const std::pair<int, int> Animation::coordinates_port {12, 40};

Animation::Animation(): exit{false}
{
    initscr();
    curs_set(0);
    noecho();
    getmaxyx(stdscr, height_window, width_window );
    
    for (int i=0; i<3; i++) {
        ports.push_back(std::shared_ptr<PortShip>(new PortShip({coordinates_port.first + 1 + i*2, coordinates_port.second})));
    }
}

Animation::~Animation()
{
    thread_check_exit.join();
    for (auto &ship : ships) {
        ship->stopThread();
    }
    for (auto &worker : workers) {
        worker->stopThread();
    }
    ships.clear();
    workers.clear();
    endwin();
}


void Animation::start()
{
    drawScene();
    thread_check_exit = std::thread(&Animation::catchExit, this);
    
    // create 5 ships and workers 
    for (int i=0; i<5; i++)
    {
        workers.push_back(std::unique_ptr<Worker>(new Worker({20-i*2, 75})));
        workers.back()->startThread();
        
        ships.push_back(std::unique_ptr<Ship>(new Ship({20-i*2, 6})));
        ships.back()->startThread();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    
    // wait to finish
    std::unique_lock<std::mutex> lck(mutex_cv_exit);
    cv_exit.wait(lck);
}

void Animation::catchExit()
{
    while(!exit)
    {
        if (static_cast<int>(getch()) == 27)
        {
            exit = true;
            cv_exit.notify_one();
        }
    }
}

void Animation::drawScene()
{
    for (int i=0; i<3; i++){
        mvprintw( (coordinates_port.first) + i*2, coordinates_port.second - 4, "-" );
        mvprintw( (coordinates_port.first) + i*2, coordinates_port.second - 3, "-" );
        mvprintw( (coordinates_port.first) + i*2, coordinates_port.second - 2, "-" );
        mvprintw( (coordinates_port.first) + i*2, coordinates_port.second - 1, "-" );
        mvprintw( (coordinates_port.first) + i*2, coordinates_port.second, "-" );
        mvprintw( (coordinates_port.first) + i*2, coordinates_port.second + 1, "-" );
        mvprintw( (coordinates_port.first) + i*2, coordinates_port.second + 2, "-" );
        mvprintw( (coordinates_port.first) + i*2 + 1, coordinates_port.second, "|" );
    }
}