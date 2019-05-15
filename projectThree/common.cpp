#include "common.h"

bool checkIfWorkerIsNeed()
{
    for (auto port : ports)
    {
        if (port->is_ship && !port->is_worker)
        {
            return true;
        }
    }
    return false;
}

bool checkIfPortIsEmpty()
{
    for (auto port : ports)
    {
        if (!port->is_ship_sail && !port->is_ship)
        {
            return true;
        }
    }
    return false;
}