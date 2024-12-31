#include "process_thread_count.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/metrics/sync_instruments.h>
#include <opentelemetry/nostd/shared_ptr.h>
#include <opentelemetry/semconv/incubating/process_metrics.h>

#include "debouncing_observer.h"
#include "types.h"

namespace {

void observe_thread_count(opentelemetry::metrics::ObserverResult result, void* arg)
{
    const auto* observer = static_cast<const debouncing_observer*>(arg);
    if (const auto& status = observer->get_status(); status.ok) {
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(status.threads);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processthreadcount
 */
void observe_process_thread_count(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer)
{
    static auto process_thread_count_counter{
        opentelemetry::semconv::process::CreateAsyncInt64MetricProcessThreadCount(meter.get())
    };

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    process_thread_count_counter->AddCallback(observe_thread_count, const_cast<debouncing_observer*>(&observer));
}
