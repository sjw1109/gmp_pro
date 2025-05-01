

#ifndef _FILE_TRIGGER_MEMORY_H_
#define _FILE_TRIGGER_MEMORY_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // usage demo:

    // #define MONITOR_BUFFER_SIZE 400
    // ctrl_gt monitor_buffer[MONITOR_BUFFER_SIZE];

    // basic_trigger_t trigger;

    // memset(monitor_buffer, MONITOR_BUFFER_SIZE * sizeof(ctrl_gt), 0);
    // dsa_init_basic_trigger(&trigger, MONITOR_BUFFER_SIZE);

    //// in main ISR function
    // if (dsa_step_trigger(&trigger, channel1_data))
    //{
    //     monitor_buffer[dsa_get_trigger_index(&trigger)] = channel1_data;

    //    // other channel here
    //    monitor_buffer2[dsa_get_trigger_index(&trigger)] = channel2_data;
    //}

    typedef struct _tag_basic_trigger
    {
        // target index
        addr32_gt target_index;

        // total memory size
        addr32_gt cell_size;

        // last information
        ctrl_gt last_data;

        // flag if this object has triggered.
        fast_gt flag_triggered;
    } basic_trigger_t;

    void dsa_init_basic_trigger(basic_trigger_t* trigger, addr32_gt cell_size)
    {
        trigger->target_index = 0;
        trigger->cell_size = cell_size;
        trigger->last_data = 0;
        trigger->flag_triggered = 0;
    }

    // return if current position is triggered.
    GMP_STATIC_INLINE
    fast_gt dsa_step_trigger(basic_trigger_t *trigger, ctrl_gt monitor)
    {
        // judge if has triggered
        if (trigger->flag_triggered)
        {
            trigger->target_index += 1;

            // if a series of trigger sequence is complete.
            if (trigger->target_index > trigger->cell_size)
            {
                // clear trigger flag
                trigger->flag_triggered = 0;
                trigger->target_index = 0;

                // restart last data
                trigger->last_data = monitor;
            }
        }
        // judge if should trigger
        else
        {
            // trigger condition
            if (trigger->last_data < 0 && trigger->monitor >= 0)
            {
                trigger->flag_triggered = 1;
            }

            trigger->last_data = monitor;
        }

        return trigger->flag_triggered;
    }

    GMP_STATIC_INLINE
    addr32_gt dsa_get_trigger_index(basic_trigger_t *trigger)
    {
        if (trigger->last_tick > trigger->cell_size)
            trigger->last_tick = 1;
        else if (trigger->last_tick <= 0)
            return 0;

        return trigger->last_tick - 1;
    }



    


    //typedef struct tag_trigger_memory_2ch
    //{
    //    // This memory will contain all the logged information
    //    // This channel is used to trigger the output
    //    ctrl_gt *memory_ch1;
    //    ctrl_gt *memory_ch2;

    //    // monitor target
    //    ctrl_gt *monitor_target_ch1;
    //    ctrl_gt *monitor_target_ch2;

    //    // memory length
    //    uint32_t log_length;

    //    // cell size
    //    uint32_t cell_size;

    //    // trigger flag
    //    fast_gt flag_trigger;
    //    ctrl_gt last_target1;
    //    uint32_t current_index;

    //} trigger_memory_log_t;

    //void dsa_init_trigger_memory(
    //    // log object
    //    trigger_memory_log_t *log,
    //    // monitor target 2 channel for all
    //    ctrl_gt *target1, ctrl_gt *target2,
    //    // memory block
    //    ctrl_gt *memory_block1, ctrl_gt *memory_block2,
    //    // type size
    //    uint32_t type_size,
    //    // memory size
    //    uint32_t memory_size);

    //// Set Monitor Target
    //GMP_STATIC_INLINE
    //void dsa_set_trigger_memory_target(
    //    // log object
    //    trigger_memory_log_t *log,
    //    // monitor target 2 channel for all
    //    ctrl_gt *target1, ctrl_gt *target2)
    //{
    //    log->monitor_target_ch1 = target1;
    //    log->monitor_target_ch2 = target2;
    //}

    //// step monitor
    //GMP_STATIC_INLINE
    //void dsa_step_trigger_memory_target(
    //    // log object
    //    trigger_memory_log_t *log)
    //{
    //    if (log->flag_trigger || (log->last_target1 < 0 && (*log->monitor_target_ch1) > 0))
    //    {
    //        log->flag_trigger = 1;

    //        log->memory_ch1[log->current_index] = *log->monitor_target_ch1;
    //        log->memory_ch2[log->current_index] = *log->monitor_target_ch2;

    //        log->current_index += 1;
    //        if (log->current_index >= log->log_length)
    //        {
    //            log->current_index = 0;
    //            log->flag_trigger = 0;
    //        }
    //    }
    //    else
    //    {
    //        log->last_target1 = *log->monitor_target_ch1;
    //    }
    //}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_TRIGGER_MEMORY_H_
