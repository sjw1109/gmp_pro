// RLC_solver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>

#include <cctl/solver/runge_kutta_4.hpp>

using std::ofstream;

int main()
{
    std::cout << "Hello World!\n";

    //diff_rlc_resonance<double> rlc_model;

    RungeKutta<diff_rlc_resonance<double>> rlc_model_rk(1e-7);

    rlc_model_rk.diff.C = 1e-5;
    rlc_model_rk.diff.L = 1e-3;
    rlc_model_rk.diff.R = 100;

    rlc_model_rk.diff.U_in = 10;

    ofstream file("output.csv");

    for (size_t i = 0; i < 100000; ++i)
    {
        file << rlc_model_rk.time << ", " << rlc_model_rk.st.il << "," << rlc_model_rk.st.uc << std::endl;
        rlc_model_rk();
    }

    file.close();


}

