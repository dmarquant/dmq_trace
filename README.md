dmq trace
=========

C/C++ library for generating traces for Chrome Tracing View

Generating a Trace File
-----------------------

To use the library include "dmq_trace.h" and add "dmq_trace.c" to your compiled
files. Use `dmq_trace_begin` and `dmq_trace_end` to start and finalize the trace.
You can only add trace events between those calls. With `dmq_trace_start_event` and
`dmq_trace_stop_event` an event can be stored in the profile. The generated trace file
is stored in **profile.json**.

```c++
dmq_trace_begin();

dmq_trace_event e;
dmq_trace_start_event(&e, "Generate Dataset");

// Generate dataset

dmq_trace_stop_event(&e);

dmq_trace_end();
```

Viewing the Trace
-----------------

Open chrome and enter **chrome://tracing** in the URL bar. In the *Chrome Trace View* click
on load and select the generated trace.

