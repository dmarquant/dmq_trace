#include "dmq_trace.h"

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#include <windows.h>

LARGE_INTEGER g_performance_frequency;

void init_timer() {
    QueryPerformanceFrequency(&g_performance_frequency);
}

uint64_t get_current_time_us() {
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

int get_thread_id() {
    return GetCurrentThreadId();
}

#elif defined(__linux__)
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>

void init_timer() {
}

uint64_t get_current_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    
    uint64_t time_us = (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
    return time_us;
}

pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

void init_lock() {
}

void lock() {
    pthread_mutex_lock(&g_lock);
}

void unlock() {
    pthread_mutex_unlock(&g_lock);
}

int get_thread_id() {
    return syscall(__NR_gettid);
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
    uint64_t stop_us = get_current_time_us();
    uint64_t start_us = event->start_time_us;

    int tid = get_thread_id();

    lock();
    if (!g_first_event) {
        fputs(",", g_trace_profile);
    } else {
        g_first_event = 0;
    }
    fprintf(g_trace_profile,
            "{\"name\": \"%s\", \"ph\": \"X\", \"ts\": %" PRId64 ", \"dur\": %" PRId64 ", \"pid\": 0, \"tid\": %d}\n",
            event->name, start_us, stop_us - start_us, tid);
    unlock();
}

#ifdef __cplusplus
}
#endif
