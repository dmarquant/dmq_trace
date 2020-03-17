#include "dmq_trace.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <windows.h>
typedef unsigned long long u64;

LARGE_INTEGER g_performance_frequency;

void init_timer() {
    QueryPerformanceFrequency(&g_performance_frequency);
}

u64 get_current_time_us() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (now.QuadPart * 1000000) / g_performance_frequency.QuadPart;
}

CRITICAL_SECTION g_lock;

void init_lock() {
    InitializeCriticalSection(&g_lock);
}

void lock() {
    EnterCriticalSection(&g_lock);
}

void unlock() {
    LeaveCriticalSection(&g_lock);
}

#else
#error "This OS is not supported with dmq trace"
#endif

FILE* g_trace_profile = 0;
int g_first_event = 0;


void dmq_trace_begin() {
    g_trace_profile = fopen("profile.json", "wb");
    fputs("[", g_trace_profile);

    g_first_event = 1;

    init_timer();
    init_lock();
}

void dmq_trace_end() {
    fputs("]", g_trace_profile);
    fclose(g_trace_profile);
}

void dmq_trace_start_event(dmq_trace_event* event, const char* name) {
    // TODO: Maybe we will have to copy this...
    event->name = name;
    event->start_time_us = get_current_time_us();
}

void dmq_trace_stop_event(dmq_trace_event* event) {
    u64 stop_us = get_current_time_us();
    u64 start_us = event->start_time_us;

    int tid = GetCurrentThreadId();

    lock();
    if (!g_first_event) {
        fputs(",", g_trace_profile);
    } else {
        g_first_event = 0;
    }
    fprintf(g_trace_profile,
            "{\"name\": \"%s\", \"ph\": \"X\", \"ts\": %I64u, \"dur\": %I64u, \"pid\": 0, \"tid\": %d}\n",
            event->name, start_us, stop_us - start_us, tid);
    unlock();
}

#ifdef __cplusplus
}
#endif
