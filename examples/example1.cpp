#include <chrono>
#include <memory>
#include <thread>

#include <opentelemetry/exporters/ostream/metric_exporter.h>
#include <opentelemetry/metrics/provider.h>
#include <opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader.h>
#include <opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_options.h>
#include <opentelemetry/sdk/metrics/meter_provider.h>

#ifdef USE_SYSTEM_INCLUDES
#    include <wwa/opentelemetry_process_metrics/process_metrics.h>
#else
#    include "wwa/opentelemetry_process_metrics/process_metrics.h"
#endif

namespace {

void init_metrics()
{
    opentelemetry::sdk::metrics::PeriodicExportingMetricReaderOptions options;
    options.export_interval_millis = std::chrono::seconds(2);
    options.export_timeout_millis  = std::chrono::seconds(1);

    auto reader = std::make_unique<opentelemetry::sdk::metrics::PeriodicExportingMetricReader>(
        std::make_unique<opentelemetry::exporter::metrics::OStreamMetricExporter>(), options
    );

    auto provider = std::make_shared<opentelemetry::sdk::metrics::MeterProvider>();
    provider->AddMetricReader(std::move(reader));

    opentelemetry::metrics::Provider::SetMeterProvider(
        std::static_pointer_cast<opentelemetry::metrics::MeterProvider>(provider)
    );

    wwa::opentelemetry::init_process_metrics();
}

}  // namespace

int main()
{
    init_metrics();
    // NOLINTNEXTLINE(*-magic-numbers)
    std::this_thread::sleep_for(std::chrono::milliseconds(3500));
    return 0;
}
