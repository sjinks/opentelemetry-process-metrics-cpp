#ifndef D3DF7C5F_EF4F_46FE_B604_179BB73BFEA4
#define D3DF7C5F_EF4F_46FE_B604_179BB73BFEA4

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

class debouncing_observer;

void observe_process_context_switches(
    const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter,
    const debouncing_observer& observer
);

#endif /* D3DF7C5F_EF4F_46FE_B604_179BB73BFEA4 */
