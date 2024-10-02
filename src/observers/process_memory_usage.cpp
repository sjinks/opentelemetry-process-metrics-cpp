#include "process_memory_usage.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "debouncing_observer.h"
#include "types.h"

namespace {

void observe_memory_usage(opentelemetry::metrics::ObserverResult result, void* arg)
{
    const auto* observer = static_cast<const debouncing_observer*>(arg);
    if (const auto& status = observer->get_status(); status.ok) {
        ::opentelemetry::nostd::get<observer_result_int64>(result)->Observe(status.rss);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processmemoryusage
 */
void observe_process_memory_usage(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer)
{
    static auto process_memory_usage_counter{
        meter->CreateInt64ObservableUpDownCounter("process.memory.usage", "The amount of physical memory in use.", "By")
    };

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    process_memory_usage_counter->AddCallback(observe_memory_usage, const_cast<debouncing_observer*>(&observer));
}
