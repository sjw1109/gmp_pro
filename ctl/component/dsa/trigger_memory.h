

#ifndef _FILE_TRIGGER_MEMORY_H_
#define _FILE_TRIGGER_MEMORY_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct tag_trigger_memory_2ch
    {
        // This memory will contain all the logged information
        // This channel is used to trigger the output
        ctrl_gt *memory_ch1;
        ctrl_gt *memory_ch2;

        // monitor target
        ctrl_gt *monitor_target_ch1;
        ctrl_gt *monitor_target_ch2;

        // memory length
        uint32_t log_length;

        // cell size
        uint32_t cell_size;

        // trigger flag
        fast_gt flag_trigger;
        ctrl_gt last_target1;
        uint32_t current_index;

    } trigger_memory_log_t;

    void dsa_init_trigger_memory(
        // log object
        trigger_memory_log_t *log,
        // monitor target 2 channel for all
        ctrl_gt *target1, ctrl_gt *target2,
        // memory block
        ctrl_gt *memory_block1, ctrl_gt *memory_block2,
        // type size
        uint32_t type_size,
        // memory size
        uint32_t memory_size);

    // Set Monitor Target
    GMP_STATIC_INLINE
    void dsa_set_trigger_memory_target(
        // log object
        trigger_memory_log_t *log,
        // monitor target 2 channel for all
        ctrl_gt *target1, ctrl_gt *target2)
    {
        log->monitor_target_ch1 = target1;
        log->monitor_target_ch2 = target2;
    }

    // step monitor
    GMP_STATIC_INLINE
    void dsa_step_trigger_memory_target(
        // log object
        trigger_memory_log_t *log)
    {
        if (log->flag_trigger || (log->last_target1 < 0 && (*log->monitor_target_ch1) > 0))
        {
            log->flag_trigger = 1;

            log->memory_ch1[log->current_index] = *log->monitor_target_ch1;
            log->memory_ch2[log->current_index] = *log->monitor_target_ch2;

            log->current_index += 1;
            if (log->current_index >= log->log_length)
            {
                log->current_index = 0;
                log->flag_trigger = 0;
            }
        }
        else
        {
            log->last_target1 = *log->monitor_target_ch1;
        }
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_TRIGGER_MEMORY_H_
