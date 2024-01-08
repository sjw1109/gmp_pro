#include <iostream>
#include <windows.h>

#include "workflow.hpp"

using namespace std;

void s0_edge();
void s1_edge();
void s2_edge();
void s3_edge();

//gmp_idx_t s0_hold();
gmp_wf_section_t* s1_hold();
gmp_wf_section_t* s2_hold();
//gmp_idx_t s3_hold();

gmp_wf_section_t s[] = {
    gmp_wf_section_t(0, s0_edge, nullptr, 3, 0, 0, &(s[1])),
    gmp_wf_section_t(1, s1_edge, s1_hold, 10, 2, 3, &(s[3])),
    gmp_wf_section_t(2, s2_edge, s2_hold, 0xFFFFFFFF, 2, 3),
    gmp_wf_section_t(3, s3_edge, nullptr, 3, 0, 0, GMP_WF_SEC_END)
};

bool start_barrier();

int main()
{
    gmp_workflow_t wf = gmp_workflow_t(&(s[0]), start_barrier);
    
    while (1)
    {
        wf.process();
        //cout << gmp_port_system_tick() << endl;
    }
}

void s0_edge()
{
    cout << "This is S0" << endl;
}

void s1_edge()
{
    cout << "This is S1" << endl;
}

void s2_edge()
{
    cout << "This is S2" << endl;
}

void s3_edge()
{
    cout << "This is S3" << endl;
}

gmp_wf_section_t* s1_hold()
{
    int cmd;
    cout << "s1 hold cmd: ";
    cin >> cmd;
    if (cmd == 2)
    {
        return &(s[2]);
    }
    return GMP_WF_SEC_WATING;
}

gmp_wf_section_t* s2_hold()
{
    int cmd;
    cout << "s2 hold cmd: ";
    cin >> cmd;
    if (cmd == 3)
    {
        return &(s[3]);
    }
    else if (cmd == 1)
    {
        return &(s[1]);
    }
    else if (cmd == 4)
    {
        return GMP_WF_SEC_END;
    }
    return GMP_WF_SEC_WATING;
}

bool start_barrier()
{
    int cmd;
    cout << "start cmd: ";
    cin >> cmd;
    if (cmd == 0)
    {
        return true;
    }
    return false;
}

void gmp_wf_cplt_callback(gmp_workflow_t* wf)
{
    cout << "workflow end.";
}
