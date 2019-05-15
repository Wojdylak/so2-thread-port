#include "common.h"
#include "Animation.h"

// definition of variables
std::vector<std::shared_ptr<PortShip>> ports;
std::mutex mutex_screen;

std::mutex  mutex_cv_workers;
std::condition_variable cv_workers;

std::mutex  mutex_cv_ships;
std::condition_variable cv_ships;

std::mutex  mutex_cv_exit;
std::condition_variable cv_exit;

int main(int argc, char **argv)
{
	srand(time(NULL));
    
    Animation animacja;
    animacja.start();
    
	return 0;
}



