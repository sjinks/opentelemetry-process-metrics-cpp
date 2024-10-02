#ifndef B7FB1ABF_B8D6_4489_A88B_9848C54BFC18
#define B7FB1ABF_B8D6_4489_A88B_9848C54BFC18

#include <cstdint>
#include <initializer_list>

#include <opentelemetry/common/attribute_value.h>
#include <opentelemetry/metrics/async_instruments.h>
#include <opentelemetry/metrics/observer_result.h>
#include <opentelemetry/nostd/shared_ptr.h>
#include <opentelemetry/nostd/string_view.h>

using string_view_t           = opentelemetry::nostd::string_view;
using attr_pair_t             = std::pair<const char*, opentelemetry::common::AttributeValue>;
using observable_instrument_t = opentelemetry::nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument>;
using labels_t                = std::initializer_list<attr_pair_t>;

using observer_result_double = opentelemetry::nostd::shared_ptr<opentelemetry::metrics::ObserverResultT<double>>;
using observer_result_int64  = opentelemetry::nostd::shared_ptr<opentelemetry::metrics::ObserverResultT<std::int64_t>>;

#endif /* B7FB1ABF_B8D6_4489_A88B_9848C54BFC18 */
