#include <memory>
#include <sys/timerfd.h>

#include <opentelemetry/common/attribute_value.h>
#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/metrics/provider.h>
#include <opentelemetry/nostd/shared_ptr.h>
#include <opentelemetry/nostd/string_view.h>
#include <opentelemetry/trace/semantic_conventions.h>

#include "wwa/opentelemetry_process_metrics/process_metrics.h"

#include "observers/rusage.h"
#include "observers/status.h"

#include "observers/debouncing_observer.h"
#include "observers/process_context_switches.h"
#include "observers/process_cpu_time.h"
#include "observers/process_cpu_utilization.h"
#include "observers/process_disk_io.h"
#include "observers/process_memory_usage.h"
#include "observers/process_memory_virtual.h"
#include "observers/process_network_io.h"
#include "observers/process_open_fd_count.h"
#include "observers/process_paging_faults.h"
#include "observers/process_thread_count.h"

void wwa::opentelemetry::init_process_metrics()
{
    static debouncing_observer observer;
    if (observer.initialized() || !observer.initialize()) {
        return;
    }

    auto meter = ::opentelemetry::metrics::Provider::GetMeterProvider()->GetMeter(
        "process.metrics", "1.2.0", ::opentelemetry::trace::SemanticConventions::kSchemaUrl
    );

    observe_process_cpu_time(meter, observer);
    observe_process_cpu_utilization(meter, observer);
    observe_process_memory_usage(meter, observer);
    observe_process_memory_virtual(meter, observer);
    observe_process_disk_io(meter);
    observe_process_network_io(meter);
    observe_process_thread_count(meter);
    observe_process_open_fd_count(meter);
    observe_process_context_switches(meter, observer);
    observe_process_paging_faults(meter, observer);
}
