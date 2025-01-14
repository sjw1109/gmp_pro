// RLC_solver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <fstream>
#include <iostream>

#include <cctl/objects/mosfet/half_bridge_switch_model.hpp>
#include <cctl/solver/runge_kutta_4.hpp>

#include <third_party/Eigen/Eigen.h>

using std::ofstream;

#if 0
int main()
{
    std::cout << "Hello World!\n";

    //diff_rlc_resonance<double> rlc_model;

    RungeKutta<diff_rlc_resonance<double>> rlc_model_rk(1e-7);

    rlc_model_rk.diff.C = 1e-5;
    rlc_model_rk.diff.L = 1e-3;
    rlc_model_rk.diff.R = 100;

    double Uin = 10;

    rlc_model_rk.diff.bind(&Uin);

    ofstream file("output.csv");

    for (size_t i = 0; i < 100000; ++i)
    {
        file << rlc_model_rk.time << ", " << rlc_model_rk.st.il << "," << rlc_model_rk.st.uc << std::endl;
        rlc_model_rk();
    }

    file.close();


}

#endif

#if 0
int main()
{
    std::cout << "Hello World!\n";

    // diff_rlc_resonance<double> rlc_model;

    RungeKutta<diff_rlc_resonance<double>> rlc_model_rk(1e-7);

    mdl_idel_half_bridge<double> hb;

    rlc_model_rk.diff.C = 1e-6;
    rlc_model_rk.diff.L = 1e-3;
    rlc_model_rk.diff.R = 40;

    hb.R_on = 10;
    
    
    double Ubus = 10;
    uint_fast8_t pwm;

    hb.bind(&Ubus, &rlc_model_rk.st.il, &pwm);
    rlc_model_rk.diff.bind(&hb.Ubridge);

    ofstream file("output.csv");

    for (size_t i = 0; i < 100000; ++i)
    {
        if (i % 1000 < 500)
            pwm = 0;
        else
            pwm = 1;

        hb();
        rlc_model_rk();

        file << pwm << ", " << rlc_model_rk.time << ", " << rlc_model_rk.st.il << "," << rlc_model_rk.st.uc
             << std::endl;
    }

    file.close();
}

#endif

int main()
{
    std::cout << "Hello World!\n";

    // diff_rlc_resonance<double> rlc_model;

    RungeKutta<diff_rlc_resonance<double>> rlc_model_rk(1e-7);

    mdl_idel_half_bridge<double> hb;

    rlc_model_rk.diff.C = 1e-6;
    rlc_model_rk.diff.L = 1e-3;
    rlc_model_rk.diff.R = 40;

    hb.R_on = 10;

    double Ubus = 10;
    uint_fast8_t pwm;

    hb.bind(&Ubus, &rlc_model_rk.st.il, &pwm);
    rlc_model_rk.diff.bind(&hb.Ubridge);

    ofstream file("output.csv");

    for (size_t i = 0; i < 100000; ++i)
    {
        if (i % 1000 < 500)
            pwm = 0;
        else
            pwm = 1;

        hb();
        rlc_model_rk();

        file << pwm << ", " << rlc_model_rk.time << ", " << rlc_model_rk.st.il << "," << rlc_model_rk.st.uc
             << std::endl;
    }

    file.close();
}




