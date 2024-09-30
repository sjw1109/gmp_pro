/**
 * @file error_code_config_compiler.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// error_code_config_compiler is a compiler module for error code Excel file.
//

//////////////////////////////////////////////////////////////////////////
// System headers
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstdio>


// boost library
#include <boost/program_options.hpp>

namespace cmd_opt = boost::program_options;

int main(int argc, char* argv[])
{
	std::cout << "Hello World!\n";


	// Create a command line parser objects.
	cmd_opt::options_description desc("GMP library error code compiler.\n\nOptions:\n");

	desc.add_options()
		("File,f", cmd_opt::value<std::string>()->default_value(""), "Input the error code details file."),
		(""),
		("Help,h", "Gets help");


	// Parse the command line
	cmd_opt::variables_map param_map;

	try
	{
		cmd_opt::store(cmd_opt::parse_command_line(argc, argv, desc), param_map);
	}
	catch (...)
	{
		std::cout << "Error occurred, when command line parse.\n";
		return 1;
	}

	// ....................................................................//

	// deal with the parameters
	if (param_map.count("Help"))
	{
		std::cout << desc << std::endl;
		return 0;
	}

	// get the target files
	if (param_map.count("File"))
	{


		return 0;
	}

		std::cout << "You should provide the Error definition files, which is a .csv file.\n"
			<< std::endl;
		std::cout << "You may get more information by '--Help, or -h' parameters."
			<< std::endl;
		return 0;

}

