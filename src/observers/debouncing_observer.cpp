#include "debouncing_observer.h"

#include <cstdint>

#include <sys/timerfd.h>
#include <unistd.h>

debouncing_observer::~debouncing_observer()
{
    if (this->fd != -1) {
        close(this->fd);
    }
}

bool debouncing_observer::initialize()
{
    constexpr auto RECHECK_INTERVAL_SECONDS = 5;

    if (this->fd != -1) {
        return false;
    }

    this->fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (this->fd != -1) {
        itimerspec timer;  // NOLINT(*-type-member-init)
        timer.it_interval.tv_sec  = RECHECK_INTERVAL_SECONDS;
        timer.it_interval.tv_nsec = 0;
        timer.it_value.tv_sec     = RECHECK_INTERVAL_SECONDS;
        timer.it_value.tv_nsec    = 0;
        if (const auto rc = timerfd_settime(this->fd, 0, &timer, nullptr); rc != -1) {
            this->rusage_observer.observe();
            this->status_observer.observe();
            return true;
        }
    }

    return false;
}

void debouncing_observer::check_timer_expiration() const
{
    if (this->fd != -1) {
        std::uint64_t expirations;  // NOLINT(*-init-variables)
        if (const auto rc = read(this->fd, &expirations, sizeof(expirations)); rc == -1) {
            this->rusage_observer.observe();
            this->status_observer.observe();
        }
    }
    else {
        this->rusage_observer.observe();
        this->status_observer.observe();
    }
}

const resource_usage& debouncing_observer::get_usage() const
{
    this->check_timer_expiration();
    return this->rusage_observer.usage();
}

const status_data& debouncing_observer::get_status() const
{
    this->check_timer_expiration();
    return this->status_observer.status();
}
