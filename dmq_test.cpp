#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "dmq_trace.h"
#include <windows.h>

#define NUM_TASKS 10000

#if DMQ_TRACE_ENABLED
#define DMQ_BEGIN() dmq_trace_begin()
#define DMQ_END() dmq_trace_end()
#define DMQ_START_EVENT(...) dmq_trace_start_event(__VA_ARGS__)
#define DMQ_STOP_EVENT(...) dmq_trace_stop_event(__VA_ARGS__)
#else
#define DMQ_BEGIN() 
#define DMQ_END() 
#define DMQ_START_EVENT(...) 
#define DMQ_STOP_EVENT(...) 
#endif

int main(int argc, char** argv) {
    int i;
    dmq_event e;
    int t[NUM_TASKS];

    auto start = std::chrono::high_resolution_clock::now();

    DMQ_BEGIN();

    DMQ_START_EVENT(&e, "Initialize");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    DMQ_STOP_EVENT(&e);


    for (i = 0; i < NUM_TASKS; i++) {
        t[i] = rand() % 300;
    }

    #pragma omp parallel for
    for (i = 0; i < NUM_TASKS; i++) {
        dmq_event e;
        DMQ_START_EVENT(&e, "Compute");
        std::this_thread::sleep_for(std::chrono::milliseconds(t[i]));
        DMQ_STOP_EVENT(&e);
    }

    DMQ_START_EVENT(&e, "Store results");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    DMQ_STOP_EVENT(&e);

    DMQ_END();

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Duration: " << std::chrono::duration<float>(end-start).count() << "s\n";

    return 0;
}

#include "dmq_trace.c"
