#ifndef ABD1D048_785F_42B5_85DB_31B2ED09E640
#define ABD1D048_785F_42B5_85DB_31B2ED09E640

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wpadded"
#endif

struct status_data {
    long long int fd_size;
    long long int peak_vm_size;
    long long int vm_size;
    long long int locked_vm_size;
    long long int pinned_vm_size;
    long long int peak_rss;
    long long int rss;
    long long int rss_anon;
    long long int rss_file;
    long long int rss_shmem;
    long long int swapped_vm_size;
    long long int huge_tlb_pages;
    long long int threads;
    bool ok;
};

#ifdef __clang__
#    pragma clang diagnostic pop
#endif

class proc_pid_status_observer {
public:
    proc_pid_status_observer();
    void observe();

    [[nodiscard]] const status_data& status() const { return this->data; }

private:
    status_data data{};
};

#endif /* ABD1D048_785F_42B5_85DB_31B2ED09E640 */
