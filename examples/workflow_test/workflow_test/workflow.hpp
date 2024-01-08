#include <stdint.h>
//#include <core/std/error_code.h>
//#include <csp/chip_default_type.h>
//#include <csp/port_function_prototype.h>

#ifndef _FILE_WORKFLOW_HPP_
#define _FILE_WORKFLOW_HPP_

//////////////////////////////////////////////
typedef uint32_t gmp_time_t;
typedef uint32_t gmp_stat_t;

#define GMP_STAT_OK 0

gmp_time_t gmp_port_system_tick();
//////////////////////////////////////////////


/**
 * User protocol while using GMP workflow:
 * 1. The class workflow only takes the very first section of whole workflow.
 * 2. routine() in each section should return the pointer of next section when ready to next;
 *                                     return GMP_WF_SEC_WATING when not ready.
 * 3. If routine == nullptr, next section would be GMP_WF_SEC_WATING by default.
 */

#define GMP_WF_RTSTATE_START (0x00)
#define GMP_WF_RTSTATE_TRANS_END (0x01)
#define GMP_WF_RTSTATE_T1_END (0x02)
#define GMP_WF_RTSTATE_ROUT_END (0x03)

#ifndef NULL
#define NULL (0x00000000)
#endif

#ifndef nullptr
#define nullptr (0x00000000)
#endif

// GMP workflow state class
class gmp_wf_section_t
{
public:
    // constructor
    gmp_wf_section_t(
        uint8_t id,
        void (*transfer)(void),
        gmp_wf_section_t* (*routine)(void),
        gmp_time_t t0,
        gmp_time_t t1,
        gmp_time_t t2,
        gmp_wf_section_t* default_next = (gmp_wf_section_t*)1
    )
        : id(id)
        , transfer(transfer)
        , routine(routine)
        , t0(t0)
        , t1(t1)
        , t2(t2)
        , default_next(default_next)
    {}

    gmp_wf_section_t(const gmp_wf_section_t& s)
        : id(s.id)
        , transfer(s.transfer)
        , routine(s.routine)
        , t0(s.t0)
        , t1(s.t1)
        , t2(s.t2)
        , default_next(s.default_next)
    {}

public:
    // ID
    uint8_t id;

    // function pointers
    void (*transfer)(void);
    gmp_wf_section_t* (*routine)(void);

    // time
    gmp_time_t t0; // overtime
    gmp_time_t t1; // first time
    gmp_time_t t2; // loop time

    // default next
    gmp_wf_section_t* default_next;
};

gmp_wf_section_t* const GMP_WF_SEC_START = (gmp_wf_section_t*)0;
gmp_wf_section_t* const GMP_WF_SEC_END = (gmp_wf_section_t*)1;
gmp_wf_section_t* const GMP_WF_SEC_WATING = (gmp_wf_section_t*)2;



// class workflow_t
class gmp_workflow_t
{
public:
    // constructor
    gmp_workflow_t()
        : sec_start(nullptr)
        , sec_last(GMP_WF_SEC_START)
        , sec_curr(GMP_WF_SEC_START)
        , sec_next(sec_start)
    {
        _disable();
    }

    gmp_workflow_t(gmp_wf_section_t* seciton_start)
        : sec_start(seciton_start)
        , sec_last(GMP_WF_SEC_START)
        , sec_curr(GMP_WF_SEC_START)
        , sec_next(sec_start)
    {
        _enable();
    }

    gmp_workflow_t(
        gmp_wf_section_t* seciton_start,
        bool (*start_barrier)(void),
        void (*timeout_callback)(void)
    )
        : sec_start(seciton_start)
        , sec_last(GMP_WF_SEC_START)
        , sec_curr(GMP_WF_SEC_START)
        , sec_next(sec_start)
        , start_barrier(start_barrier)
        , timeout_callback(timeout_callback)
    {
        _enable();
    }

    gmp_workflow_t(
        gmp_wf_section_t* seciton_start,
        bool (*start_barrier)(void)
    )
        : sec_start(seciton_start)
        , sec_last(GMP_WF_SEC_START)
        , sec_curr(GMP_WF_SEC_START)
        , sec_next(sec_start)
        , start_barrier(start_barrier)
    {
        _enable();
    }

    gmp_workflow_t(
        gmp_wf_section_t* seciton_start,
        void (*timeout_callback)(void)
    )
        : sec_start(seciton_start)
        , sec_last(GMP_WF_SEC_START)
        , sec_curr(GMP_WF_SEC_START)
        , sec_next(sec_start)
        , timeout_callback(timeout_callback)
    {
        _enable();
    }

    gmp_workflow_t(const gmp_workflow_t& wf)
        : sec_start(wf.sec_start)
        , sec_last(GMP_WF_SEC_START)
        , sec_curr(GMP_WF_SEC_START)
        , sec_next(sec_start)
        , start_barrier(wf.start_barrier)
        , timeout_callback(wf.timeout_callback)
    {
        _enable();
    }

private:
    // sections
    gmp_wf_section_t* sec_start;

    gmp_wf_section_t* sec_last;
    gmp_wf_section_t* sec_curr;
    gmp_wf_section_t* sec_next;

    // flag
    uint8_t flag_enable;   // enable flag
    uint8_t runtime_state; // workflow runtime flag

    // time
    gmp_time_t last_tick;
    gmp_time_t t0_start_tick;

public:
    // workflow functions
    gmp_stat_t init(gmp_wf_section_t* section_start);
    gmp_stat_t deinit();
    gmp_stat_t enable();
    gmp_stat_t disable();
    gmp_stat_t process();

private:
    // barrier functions
    bool (*start_barrier)(void); // start barrier function

    // callback functions
    void (*timeout_callback)(void); // timeout callback

public:
    // get and set
    void set_start_barrier(bool (*start_barrier)(void));
    void set_timeout_callback(void (*timeout_callback)(void));

    uint8_t get_curr_id();

private:
    // inline functions
    inline void _enable()
    {
        flag_enable = 1;
    }

    inline void _disable()
    {
        flag_enable = 0;
    }

    inline uint8_t is_enable()
    {
        return flag_enable;
    }
};


// general callback functions
void gmp_wf_timeout_callback(gmp_workflow_t* wf); // general timeout callback

void gmp_wf_cplt_callback(gmp_workflow_t* wf); // workflow end callback;


#endif // !_FILE_WORKFLOW_HPP_
