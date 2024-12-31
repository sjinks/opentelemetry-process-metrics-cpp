#ifndef D4272022_10F5_4EF3_905A_395F07ADFB35
#define D4272022_10F5_4EF3_905A_395F07ADFB35

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

void observe_process_open_fd_count(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter);

#endif /* D4272022_10F5_4EF3_905A_395F07ADFB35 */
