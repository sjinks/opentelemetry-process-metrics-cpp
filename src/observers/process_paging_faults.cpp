#include "process_paging_faults.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/metrics/sync_instruments.h>
#include <opentelemetry/nostd/shared_ptr.h>
#include <opentelemetry/semconv/incubating/process_attributes.h>
#include <opentelemetry/semconv/incubating/process_metrics.h>

#include "debouncing_observer.h"
#include "types.h"

namespace {

void observe_paging_faults(::opentelemetry::metrics::ObserverResult result, void* arg)
{
    static const labels_t fault_type_major_labels{{opentelemetry::semconv::process::kProcessPagingFaultType, opentelemetry::semconv::process::ProcessPagingFaultTypeValues::kMajor}};
    static const labels_t fault_type_minor_labels{{opentelemetry::semconv::process::kProcessPagingFaultType, opentelemetry::semconv::process::ProcessPagingFaultTypeValues::kMinor}};

    const auto* observer = static_cast<const debouncing_observer*>(arg);
    if (const auto& usage = observer->get_usage(); usage.ok) {
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(usage.minor_page_faults, fault_type_minor_labels);
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(usage.major_page_faults, fault_type_major_labels);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processpagingfaults
 */
void observe_process_paging_faults(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer)
{
    static auto process_paging_faults_counter{
        opentelemetry::semconv::process::CreateAsyncInt64MetricProcessPagingFaults(meter.get())
    };

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    process_paging_faults_counter->AddCallback(observe_paging_faults, const_cast<debouncing_observer*>(&observer));
}
