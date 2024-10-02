#ifndef C5A7EF6B_C5EE_4225_AFE6_67F38FC9A8D8
#define C5A7EF6B_C5EE_4225_AFE6_67F38FC9A8D8

#include <cmath>
#include <limits>

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wpadded"
#endif

struct resource_usage {
    double user_time;
    double system_time;
    double user_utilization;
    double system_utilization;
    long long int minor_page_faults;
    long long int major_page_faults;
    long long int voluntary_context_switches;
    long long int involuntary_context_switches;
    bool ok;
};

#ifdef __clang__
#    pragma clang diagnostic pop
#endif

class resource_usage_observer {
public:
    resource_usage_observer();
    void observe();
    [[nodiscard]] const resource_usage& usage() const { return this->last_usage; }

private:
    double last_measurement = std::numeric_limits<double>::quiet_NaN();
    resource_usage last_usage{};
};

#endif /* C5A7EF6B_C5EE_4225_AFE6_67F38FC9A8D8 */
