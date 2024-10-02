#include "process_context_switches.h"

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "debouncing_observer.h"
#include "types.h"

namespace {

void observe_context_switches(::opentelemetry::metrics::ObserverResult result, void* arg)
{
    static const labels_t context_switch_voluntary_labels{{"process.context_switch_type", "voluntary"}};
    static const labels_t context_switch_involuntary_labels{{"process.context_switch_type", "involuntary"}};

    const auto* observer = static_cast<const debouncing_observer*>(arg);
    if (const auto& usage = observer->get_usage(); usage.ok) {
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(
            usage.involuntary_context_switches, context_switch_involuntary_labels
        );

        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(
            usage.voluntary_context_switches, context_switch_voluntary_labels
        );
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processcontext_switches
 */
void observe_process_context_switches(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer)
{
    static auto process_context_switches_counter{meter->CreateInt64ObservableCounter(
        "process.context_switches", "Number of times the process has been context switched.", "{count}"
    )};

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    process_context_switches_counter->AddCallback(observe_context_switches, const_cast<debouncing_observer*>(&observer));
}
