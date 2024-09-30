/**
 * @file workflow_scheduler.hpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// headers.hpp

#include <core/workflow/workflow.hpp>

#ifndef _FILE_WORKFLOW_SCHEDULER_HPP_
#define _FILE_WORKFLOW_SCHEDULER_HPP_

// TODO impl
// static prior


// TODO impl
// dynamic prior, set by scheduler or switch_to

// TODO impl
// 4 types of workflow scheduler
// + realtime scheduler // half of time slice (caller slice) are used here, and they are equal polling
// + fine scheduler // need a timer and the process will run in the timer.
// + common scheduler // not too precise, not too real-time
// + batch scheduler // lowest prior

// TODO impl
// saving a queue that contains which tasks is ready.
// so the time judge should be complete by scheduling

#endif // _FILE_WORKFLOW_SCHEDULER_HPP_