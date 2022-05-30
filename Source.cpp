#include <iostream>
#include "SimplexTableSolver/SimplexTableSolver.h"

int main()
{		
	std::string fileName;
	std::cout << "Enter a configuration file path:" << std::endl;

	std::getline(std::cin, fileName);

	SimplexTableSolver solver(fileName);

	if (!solver.isValid())
	{
		std::cout << "Error while reading the config file" << std::endl;
		return -1;
	}

	solver.solve();
}