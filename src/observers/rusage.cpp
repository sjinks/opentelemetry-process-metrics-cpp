#include "rusage.h"

#include <ctime>

#include <sys/resource.h>
#include <unistd.h>

resource_usage_observer::resource_usage_observer()
{
    this->observe();
}

void resource_usage_observer::observe()
{
    constexpr auto NANOSECONDS_IN_SECOND  = 1E9;
    constexpr auto MICROSECONDS_IN_SECOND = 1E6;

    std::timespec time_now;  // NOLINT(*-type-member-init)
    rusage current_rusage;   // NOLINT(*-type-member-init)

    if (-1 == clock_gettime(CLOCK_MONOTONIC, &time_now) || -1 == getrusage(RUSAGE_SELF, &current_rusage)) {
        this->last_usage.ok = false;
        if (std::isnan(this->last_measurement)) {
            this->last_usage.system_time = 0;
            this->last_usage.user_time   = 0;
        }

        this->last_usage.system_utilization           = 0;
        this->last_usage.user_utilization             = 0;
        this->last_usage.major_page_faults            = 0;
        this->last_usage.minor_page_faults            = 0;
        this->last_usage.voluntary_context_switches   = 0;
        this->last_usage.involuntary_context_switches = 0;
        return;
    }

    const auto has_last_measurement = !std::isnan(this->last_measurement);

    const auto now =
        static_cast<double>(time_now.tv_sec) + static_cast<double>(time_now.tv_nsec) / NANOSECONDS_IN_SECOND;
    const double interval = has_last_measurement ? now - this->last_measurement : 0;
    const auto cpu_count  = static_cast<double>(sysconf(_SC_NPROCESSORS_ONLN));

    const double prev_system_time = has_last_measurement ? this->last_usage.system_time : 0;
    const double prev_user_time   = has_last_measurement ? this->last_usage.user_time : 0;

    this->last_usage.system_time = static_cast<double>(current_rusage.ru_stime.tv_sec) +
                                   static_cast<double>(current_rusage.ru_stime.tv_usec) / MICROSECONDS_IN_SECOND;
    this->last_usage.user_time = static_cast<double>(current_rusage.ru_utime.tv_sec) +
                                 static_cast<double>(current_rusage.ru_utime.tv_usec) / MICROSECONDS_IN_SECOND;
    this->last_usage.system_utilization =
        has_last_measurement ? (this->last_usage.system_time - prev_system_time) / interval / cpu_count : 0;
    this->last_usage.user_utilization =
        has_last_measurement ? (this->last_usage.user_time - prev_user_time) / interval / cpu_count : 0;
    this->last_usage.major_page_faults            = current_rusage.ru_majflt;
    this->last_usage.minor_page_faults            = current_rusage.ru_minflt;
    this->last_usage.voluntary_context_switches   = current_rusage.ru_nvcsw;
    this->last_usage.involuntary_context_switches = current_rusage.ru_nivcsw;

    this->last_usage.ok = true;

    this->last_measurement = now;
}
