#include "process_thread_count.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "types.h"
#include "utils.h"

namespace {

void observe_thread_count(opentelemetry::metrics::ObserverResult result, void*)
{
    if (auto count = count_fs_entries("/proc/self/task"); count != -1) {
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(count);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processthreadcount
 */
void observe_process_thread_count(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter)
{
    static auto process_thread_count_counter{
        meter->CreateInt64ObservableUpDownCounter("process.thread.count", "Process threads count.", "{thread}")
    };

    process_thread_count_counter->AddCallback(observe_thread_count, nullptr);
}
