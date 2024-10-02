#ifndef E9BC5939_C97D_4E3F_A7A7_0969AB98F79B
#define E9BC5939_C97D_4E3F_A7A7_0969AB98F79B

#include <opentelemetry/metrics/meter.h>

class debouncing_observer;

void observe_process_memory_virtual(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer);

#endif /* E9BC5939_C97D_4E3F_A7A7_0969AB98F79B */
