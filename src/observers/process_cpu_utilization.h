#ifndef FE296DB4_5979_45D5_810D_32E4F704E88D
#define FE296DB4_5979_45D5_810D_32E4F704E88D

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

class debouncing_observer;

void observe_process_cpu_utilization(
    const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter,
    const debouncing_observer& observer
);

#endif /* FE296DB4_5979_45D5_810D_32E4F704E88D */
