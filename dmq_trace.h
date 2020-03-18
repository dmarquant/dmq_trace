#pragma once 

typedef struct
{
    // Readable name of the event
    const char* name;
    
    // The start time of the event
    unsigned long long start_time_us; 
} dmq_trace_event;

#ifdef DMQ_DISABLE_TRACE
#define DMQ_TRACE_BEGIN()
#define DMQ_TRACE_END()
#define DMQ_TRACE_START_EVENT(...)
#define DMQ_TRACE_STOP_EVENT(...)
#else
#define DMQ_TRACE_BEGIN() dmq_trace_begin()
#define DMQ_TRACE_END() dmq_trace_end()
#define DMQ_TRACE_START_EVENT(...) dmq_trace_start_event(__VA_ARGS__)
#define DMQ_TRACE_STOP_EVENT(...) dmq_trace_stop_event(__VA_ARGS__)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void dmq_trace_begin();
void dmq_trace_end();

void dmq_trace_start_event(dmq_trace_event* event, const char* name);
void dmq_trace_stop_event(dmq_trace_event* event);

#ifdef __cplusplus
}
#endif
