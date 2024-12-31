#ifndef B6F99C8D_7869_4A9A_8F65_A5A8FCA47084
#define B6F99C8D_7869_4A9A_8F65_A5A8FCA47084

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

void observe_process_disk_io(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter);

#endif /* B6F99C8D_7869_4A9A_8F65_A5A8FCA47084 */
