dmq trace
=========

C/C++ library for generating traces for Chrome Tracing View

Generating a Trace File
-----------------------

To use the library include **dmq_trace.h** and add **dmq_trace.c** to your compiled
files. Use `DMQ_TRACE_BEGIN` and `DMQ_TRACE_END` to start and finalize the trace.
You can only add trace events between those calls. With `DMQ_TRACE_START_EVENT` and
`DMQ_TRACE_STOP_EVENT` an event can be stored in the profile. The generated trace file
is stored in **profile.json**.

```c++
DMQ_TRACE_BEGIN();

dmq_trace_event e;
DMQ_TRACE_START_EVENT(&e, "Generate Dataset");

// Generate dataset

DMQ_TRACE_STOP_EVENT(&e);

DMQ_TRACE_END();
```

Or you can use the C++ API by including **dmq_trace.hpp**. You can create a timer object that will
start a trace event. You can stop the timer or let the destructor stop the timer automatically.

```c++
DMQ_TRACE_BEGIN();

{
dmq::Timer timer("Generate Dataset");
// Generate dataset
}

dmq::Timer timer("Finalize");
// Finalize
timer.stop();

DMQ_TRACE_END();
```



Viewing the Trace
-----------------

Open chrome and enter **chrome://tracing** in the URL bar. In the *Chrome Trace View* click
on load and select the generated trace.

![Example of Chrome Trace View](https://github.com/dmarquant/dmq_trace/blob/master/trace_view.png "Example of Chrome Trace View")

