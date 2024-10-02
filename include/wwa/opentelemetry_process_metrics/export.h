#ifndef FE01BB6B_5B26_4F57_9168_E5F2A413AD3E
#define FE01BB6B_5B26_4F57_9168_E5F2A413AD3E

#ifdef WWA_OPENTELEMETRY_PROCESS_METRICS_STATIC_DEFINE
#    define WWA_OPENTELEMETRY_PROCESS_METRICS_EXPORT
#    define WWA_OPENTELEMETRY_PROCESS_METRICS_NO_EXPORT
#else
#    ifdef wwa_opentelemetry_process_metrics_EXPORTS
/* We are building this library; export */
#        if defined _WIN32 || defined __CYGWIN__
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_EXPORT __declspec(dllexport)
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_NO_EXPORT
#        else
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_EXPORT    [[gnu::visibility("default")]]
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    else
/* We are using this library; import */
#        if defined _WIN32 || defined __CYGWIN__
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_EXPORT __declspec(dllimport)
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_NO_EXPORT
#        else
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_EXPORT    [[gnu::visibility("default")]]
#            define WWA_OPENTELEMETRY_PROCESS_METRICS_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    endif
#endif

#endif /* FE01BB6B_5B26_4F57_9168_E5F2A413AD3E */
