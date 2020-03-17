#pragma once 

typedef struct
{
    // Readable name of the event
    const char* name;
    
    // The start time of the event
    unsigned long long start_time_us; 
} dmq_event;

#ifdef __cplusplus
extern "C" {
#endif

void dmq_trace_begin();
void dmq_trace_end();

void dmq_trace_start_event(dmq_event* event, const char* name);
void dmq_trace_stop_event(dmq_event* event);

#ifdef __cplusplus
}
#endif
