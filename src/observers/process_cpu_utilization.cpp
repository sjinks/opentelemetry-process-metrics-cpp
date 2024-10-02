#include "process_cpu_utilization.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "debouncing_observer.h"
#include "types.h"

namespace {

void observe_cpu_utilization(opentelemetry::metrics::ObserverResult result, void* arg)
{
    static const labels_t cpu_mode_user_labels{{"cpu.mode", "user"}};
    static const labels_t cpu_mode_system_labels{{"cpu.mode", "system"}};

    const auto* observer = static_cast<const debouncing_observer*>(arg);
    if (const auto& usage = observer->get_usage(); usage.ok) {
        opentelemetry::nostd::get<observer_result_double>(result)->Observe(usage.user_utilization, cpu_mode_user_labels);
        opentelemetry::nostd::get<observer_result_double>(result)->Observe(usage.system_utilization, cpu_mode_system_labels);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processcpuutilization
 */
void observe_process_cpu_utilization(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer)
{
    static auto process_cpu_utilization_gauge{meter->CreateDoubleObservableGauge(
        "process.cpu.utilization",
        "Difference in process.cpu.time since the last measurement, divided by the elapsed time and number of CPUs available to the process.",
        "1"
    )};

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    process_cpu_utilization_gauge->AddCallback(observe_cpu_utilization, const_cast<debouncing_observer*>(&observer));
}
