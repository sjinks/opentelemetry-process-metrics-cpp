#include "process_disk_io.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "types.h"

namespace {

struct disk_io_stats {
    long long int read_bytes;
    long long int write_bytes;
};

disk_io_stats get_disk_io_stats()
{
    try {
        std::ifstream f("/proc/self/io");
        std::string line;
        std::string key;

        disk_io_stats stats{0, 0};

        while (std::getline(f, line)) {
            std::istringstream iss(line);
            long long int value;  // NOLINT(*-init-variables)
            iss >> key >> value;
            if (key == "read_bytes:") {
                stats.read_bytes = value;
            }
            else if (key == "write_bytes:") {
                stats.write_bytes = value;
            }
        }

        return stats;
    }
    catch (const std::exception&) {
        return {-1, -1};
    }
}

void observe_disk_io(opentelemetry::metrics::ObserverResult result, void*)
{
    static const labels_t direction_read_labels{{"disk.io.direction", "read"}};
    static const labels_t direction_write_labels{{"disk.io.direction", "write"}};

    if (const auto usage = get_disk_io_stats(); usage.read_bytes != -1) {
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(usage.read_bytes, direction_read_labels);
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(usage.write_bytes, direction_write_labels);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processdiskio
 */
void observe_process_disk_io(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter)
{
    static auto process_disk_io_counter{
        meter->CreateInt64ObservableCounter("process.disk.io", "Disk bytes transferred.", "By")
    };

    process_disk_io_counter->AddCallback(observe_disk_io, nullptr);
}
