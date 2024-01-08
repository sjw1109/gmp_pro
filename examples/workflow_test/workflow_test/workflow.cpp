#include "workflow.hpp"

////////////////////////////////////
#include <windows.h>

gmp_time_t gmp_port_system_tick()
{
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    return sys.wMinute * 60 + sys.wSecond;
}
////////////////////////////////////

gmp_stat_t gmp_workflow_t::init(gmp_wf_section_t* section_start)
{
    if (sec_start == nullptr)
    {
        sec_start = section_start;
        sec_last = GMP_WF_SEC_START;
        sec_curr = GMP_WF_SEC_START; // start position
        sec_next = sec_start;        // user start position

        _enable();

        return GMP_STAT_OK;
    }
    else
    {
        // TODO
        // error("wf already init");
        // return GMP_STAT_ERROR_???;
    }
    return GMP_STAT_OK;
}

gmp_stat_t gmp_workflow_t::deinit()
{
    sec_start = nullptr;

    _disable();

    return GMP_STAT_OK;
}

gmp_stat_t gmp_workflow_t::enable()
{
    _enable();
    return GMP_STAT_OK;
}

gmp_stat_t gmp_workflow_t::disable()
{
    _disable();
    return GMP_STAT_OK;
}

gmp_stat_t gmp_workflow_t::process()
{
    // flag check
    if (sec_start == nullptr)
    {
        // TODO
        // error("wf not init");
        // return GMP_STAT_ERROR_???;
        return GMP_STAT_OK;
    }
    if (!is_enable())
    {
        // TODO
        // warning("wf not enable");
        // return GMP_STAT_WARNING_???;
        return GMP_STAT_OK;
    }

    // process
    switch (runtime_state)
    {
    case GMP_WF_RTSTATE_START:
        // check start
        if (sec_curr == GMP_WF_SEC_START)
        {
            if ((start_barrier != nullptr) && (start_barrier)() == false)
            {
                break;
            }
        }

        // check end
        else if (sec_next == GMP_WF_SEC_END)
        {
            disable();

            // end callback
            gmp_wf_cplt_callback(this);

            break;
        }

        // change index
        sec_last = sec_curr;
        sec_curr = sec_next;

        // edge duration
        if (sec_curr->transfer != nullptr)
        {
            (sec_curr->transfer)(); // run edge function
        }

        last_tick = gmp_port_system_tick(); // get system tick as last_tick

        runtime_state = GMP_WF_RTSTATE_TRANS_END;
        break;

    case GMP_WF_RTSTATE_TRANS_END:
        // t1 wait duration
        if ((gmp_port_system_tick() - last_tick) >= sec_curr->t1)
        {
            // t1 finished
            t0_start_tick = gmp_port_system_tick(); // get sys tick as t0 statr tick;

            runtime_state = GMP_WF_RTSTATE_T1_END;
        }
        break;

    case GMP_WF_RTSTATE_T1_END:
        if ((gmp_port_system_tick() - t0_start_tick) >= sec_curr->t0)
        {
            // timeout
            sec_next = sec_curr->default_next;

            runtime_state = GMP_WF_RTSTATE_START;

            // timeout callback
            if(timeout_callback != nullptr)
            {
                timeout_callback();
            }
            gmp_wf_timeout_callback(this);
        }
        else
        {
            // not timeout
            // hold function duration
            if (sec_curr->routine == nullptr)
            {
                // null hold function
                sec_next = GMP_WF_SEC_WATING;
            }
            else
            {
                // run hold function
                sec_next = (sec_curr->routine)();
                // return next position if OK
                // return GMP_WF_IDX_WATING if NOT OK
            }

            if (sec_next != GMP_WF_SEC_WATING)
            {
                runtime_state = GMP_WF_RTSTATE_START;
            }
            else
            {
                last_tick = gmp_port_system_tick(); // get sys tick as last_tick

                runtime_state = GMP_WF_RTSTATE_ROUT_END;
            }
        }
        break;

    case GMP_WF_RTSTATE_ROUT_END:
        if ((gmp_port_system_tick() - t0_start_tick) >= sec_curr->t0)
        {
            // timeout
            sec_next = sec_curr->default_next;

            runtime_state = GMP_WF_RTSTATE_START;

            // timeout callback
            if(timeout_callback != nullptr)
            {
                timeout_callback();
            }
            gmp_wf_timeout_callback(this);
        }
        else
        {
            // not timeout
            if ((gmp_port_system_tick() - last_tick) >= sec_curr->t2)
            {
                // t2 finished
                if (sec_next == GMP_WF_SEC_WATING)
                {
                    // not ready to next
                    runtime_state = GMP_WF_RTSTATE_T1_END;
                }
                else
                {
                    // ready to next
                    runtime_state = GMP_WF_RTSTATE_START;
                }
            }
        }
        break;

    default:
        break;
    }

    return GMP_STAT_OK;
}

void gmp_workflow_t::set_start_barrier(bool(*start_barrier)(void))
{
    start_barrier = start_barrier;
}

void gmp_workflow_t::set_timeout_callback(void(*timeout_callback)(void))
{
    timeout_callback = timeout_callback;
}

uint8_t gmp_workflow_t::get_curr_id()
{
    return sec_curr->id;
}


void gmp_wf_timeout_callback(gmp_workflow_t* wf)
{}

//void wf_end_callback(workflow_t* wf)
//{}
