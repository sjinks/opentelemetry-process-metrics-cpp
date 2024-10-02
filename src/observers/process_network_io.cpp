#include "process_network_io.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>

#include "types.h"

namespace {

struct network_io_stats {
    long long int bytes_received;
    long long int bytes_sent;
};

network_io_stats get_network_io_stats()
{
    try {
        std::ifstream file("/proc/self/net/dev");
        std::string line;
        network_io_stats usage = {0, 0};

        // Skip headers
        std::getline(file, line);
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string interface_name;
            long long int bytes_received;  // NOLINT(*-init-variables)
            long long int bytes_sent;      // NOLINT(*-init-variables)
            long long int dummy;           // NOLINT(*-init-variables)

            iss >> interface_name  // interface
                >> bytes_received  // bytes
                >> dummy           // packets
                >> dummy           // errs
                >> dummy           // drop
                >> dummy           // fifo
                >> dummy           // frame
                >> dummy           // compressed
                >> dummy           // multicast
                >> bytes_sent      // bytes
                ;

            if (interface_name != "lo:") {
                usage.bytes_received += bytes_received;
                usage.bytes_sent     += bytes_sent;
            }
        }

        return usage;
    }
    catch (const std::exception&) {
        return {-1, -1};
    }
}

void observe_network_io(opentelemetry::metrics::ObserverResult result, void*)
{
    static const labels_t direction_receive_labels{{"network.io.direction", "receive"}};
    static const labels_t direction_transmit_labels{{"network.io.direction", "transmit"}};

    if (const auto usage = get_network_io_stats(); usage.bytes_received != -1) {
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(usage.bytes_received, direction_receive_labels);
        opentelemetry::nostd::get<observer_result_int64>(result)->Observe(usage.bytes_sent, direction_transmit_labels);
    }
}

}  // namespace

/**
 * @see https://opentelemetry.io/docs/specs/semconv/system/process-metrics/#metric-processnetworkio
 */
void observe_process_network_io(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter)
{
    static auto process_network_io_counter{
        meter->CreateInt64ObservableCounter("process.network.io", "Network bytes transferred.", "By")
    };

    process_network_io_counter->AddCallback(observe_network_io, nullptr);
}
