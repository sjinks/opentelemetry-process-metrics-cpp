#ifndef FC227903_5FD4_4FCB_AEC5_31244EF06D3D
#define FC227903_5FD4_4FCB_AEC5_31244EF06D3D

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

class debouncing_observer;

void observe_process_cpu_time(
    const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter,
    const debouncing_observer& observer
);

#endif /* FC227903_5FD4_4FCB_AEC5_31244EF06D3D */
