#ifndef CE38430A_C062_4A9C_942C_07876BED8A1C
#define CE38430A_C062_4A9C_942C_07876BED8A1C

#include "rusage.h"
#include "status.h"

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wpadded"
#endif

class debouncing_observer {
public:
    debouncing_observer() = default;
    ~debouncing_observer();

    debouncing_observer(const debouncing_observer&)            = delete;
    debouncing_observer& operator=(const debouncing_observer&) = delete;
    debouncing_observer(debouncing_observer&&)                 = delete;
    debouncing_observer& operator=(debouncing_observer&&)      = delete;

    bool initialize();

    [[nodiscard]] const resource_usage& get_usage() const;
    [[nodiscard]] const status_data& get_status() const;

    [[nodiscard]] bool initialized() const { return this->fd != -1; }

private:
    mutable resource_usage_observer rusage_observer;
    mutable proc_pid_status_observer status_observer;
    int fd = -1;

    void check_timer_expiration() const;
};

#ifdef __clang__
#    pragma clang diagnostic pop
#endif

#endif /* CE38430A_C062_4A9C_942C_07876BED8A1C */
