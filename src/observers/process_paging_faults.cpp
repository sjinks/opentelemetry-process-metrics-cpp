#include "process_paging_faults.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "debouncing_observer.h"
#include "types.h"

namespace {

void observe_paging_faults(::opentelemetry::metrics::ObserverResult result, void* arg)
{
    static const labels_t fault_type_major_labels{{"process.paging.fault_type", "major"}};
    static const labels_t fault_type_minor_labels{{"process.paging.fault_type", "minor"}};

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
    static auto process_paging_faults_counter{meter->CreateInt64ObservableCounter(
        "process.paging.faults", "Number of page faults the process has made.", "{fault}"
    )};

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    process_paging_faults_counter->AddCallback(observe_paging_faults, const_cast<debouncing_observer*>(&observer));
}
