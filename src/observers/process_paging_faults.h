#ifndef D53AF08A_C62C_47C4_A3CA_37BA2C2DFE0B
#define D53AF08A_C62C_47C4_A3CA_37BA2C2DFE0B

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

class debouncing_observer;

void observe_process_paging_faults(
    const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter,
    const debouncing_observer& observer
);

#endif /* D53AF08A_C62C_47C4_A3CA_37BA2C2DFE0B */
