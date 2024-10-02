#ifndef DBB0F0D3_59A1_4A22_9054_ADD490BB8B16
#define DBB0F0D3_59A1_4A22_9054_ADD490BB8B16

#include <opentelemetry/metrics/meter.h>

class debouncing_observer;

void observe_process_memory_usage(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer);

#endif /* DBB0F0D3_59A1_4A22_9054_ADD490BB8B16 */
