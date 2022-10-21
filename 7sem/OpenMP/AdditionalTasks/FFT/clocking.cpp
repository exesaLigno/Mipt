#pragma once
#include <ctime>

long clk()
{
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    long int time_ms = time.tv_sec * 1000000 + time.tv_nsec / 1000;
    return time_ms;
}
