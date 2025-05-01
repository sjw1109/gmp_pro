

#include <gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// trigger memory module
#include <ctl/component/dsa/trigger_memory.h>

void dsa_init_basic_trigger(basic_trigger_t *trigger, addr32_gt cell_size)
{
    trigger->target_index = 0;
    trigger->cell_size = cell_size;
    trigger->last_data = 0;
    trigger->flag_triggered = 0;
}

//
// void dsa_init_trigger_memory(
//    // log object
//    trigger_memory_log_t *log,
//    // monitor target 2 channel for all
//    ctrl_gt *target1, ctrl_gt *target2,
//    // memory block
//    ctrl_gt *memory_block1, ctrl_gt *memory_block2,
//    // type size
//    uint32_t type_size,
//    // memory size
//    uint32_t memory_size)
//{
//    log->memory_ch1 = memory_block1;
//    log->memory_ch2 = memory_block2;
//
//    log->monitor_target_ch1 = target1;
//    log->monitor_target_ch2 = target2;
//
//    log->log_length = memory_size;
//    log->cell_size = type_size;
//
//    log->flag_trigger = 0;
//    log->last_target1 = 0;
//    log->current_index = 0;
//}
