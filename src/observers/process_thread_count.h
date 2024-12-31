#ifndef E3A21495_F639_490C_A0F9_0549669F6698
#define E3A21495_F639_490C_A0F9_0549669F6698

#include <opentelemetry/metrics/meter.h>
#include <opentelemetry/nostd/shared_ptr.h>

class debouncing_observer;

void observe_process_thread_count(const opentelemetry::nostd::shared_ptr<opentelemetry::metrics::Meter>& meter, const debouncing_observer& observer);

#endif /* E3A21495_F639_490C_A0F9_0549669F6698 */
