#ifndef C010375D_DCBF_42E3_A782_1D457320FA18
#define C010375D_DCBF_42E3_A782_1D457320FA18

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

void observe_process_network_io(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter);

#endif /* C010375D_DCBF_42E3_A782_1D457320FA18 */
