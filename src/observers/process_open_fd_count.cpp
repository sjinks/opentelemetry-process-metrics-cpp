#include "process_open_fd_count.h"

#include "process_thread_count.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "types.h"
#include "utils.h"

namespace {

void observe_open_fd_count(opentelemetry::metrics::ObserverResult result, void*)
{
    if (auto count = count_fs_entries("/proc/self/fd"); count != -1) {
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(count);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processopen_file_descriptorcount
 */
void observe_process_open_fd_count(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter)
{
    static auto process_open_fd_count_counter{meter->CreateInt64ObservableUpDownCounter(
        "process.open_file_descriptor.count", "Number of file descriptors in use by the process.", "{count}"
    )};

    process_open_fd_count_counter->AddCallback(observe_open_fd_count, nullptr);
}
