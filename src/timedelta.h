#ifndef TIMEDELTA_H
#define TIMEDELTA_H

#include <chrono>

using namespace std;
using namespace std::chrono;

int64_t delta (high_resolution_clock::time_point start, high_resolution_clock::time_point end)
{

    return duration_cast<milliseconds>(end - start).count();
}


#endif