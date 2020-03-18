#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "dmq_trace.hpp"

#define NUM_TASKS 100

int main(int argc, char** argv) {
    int i;
    dmq_trace_event e;
    int t[NUM_TASKS];

    auto start = std::chrono::high_resolution_clock::now();

    DMQ_TRACE_BEGIN();

    DMQ_TRACE_START_EVENT(&e, "Initialize");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    DMQ_TRACE_STOP_EVENT(&e);


    for (i = 0; i < NUM_TASKS; i++) {
        t[i] = rand() % 300;
    }

    #pragma omp parallel for
    for (i = 0; i < NUM_TASKS; i++) {
        dmq::Timer timer("Compute");
        std::this_thread::sleep_for(std::chrono::milliseconds(t[i]));
    }

    DMQ_TRACE_START_EVENT(&e, "Store results");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    DMQ_TRACE_STOP_EVENT(&e);

    DMQ_TRACE_END();

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Duration: " << std::chrono::duration<float>(end-start).count() << "s\n";

    return 0;
}

#include "dmq_trace.c"
