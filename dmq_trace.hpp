#pragma once

#include "dmq_trace.h"

namespace dmq {

class Timer {
    dmq_trace_event e = {};
    bool stopped = false;

public:
    Timer(const char* name) {
        DMQ_TRACE_START_EVENT(&e, name);
    }

    ~Timer() {
        stop();
    }

    void stop() {
        if (!stopped) {
            DMQ_TRACE_STOP_EVENT(&e);
            stopped = true;
        }
    }
};

}
