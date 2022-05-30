#include <iostream>
#include <fstream>
#include <string>
#include "../Number/Number.h"
#include <vector>

const Number M = 99999;

class SimplexTableSolver
{
private:
	struct Task
	{

		std::vector<std::string> varNames;
		std::vector<Number> task;
		bool isMin;

		struct Line
		{
			std::vector<Number> vars;
			std::string relation;
			Number constraint;
		};
		std::vector<Line> constrains;
		std::vector<bool> positive;
	};

	void printTask(const Task& t);
	void addNewVar(Task& t, int row, std::string& relation);
	void convertToPositive(Task& t, int index);
	void makeCanonical(Task& t);
	bool checkOnlyZeros(Task& t, int column, int skipRow);
	std::vector<int> makeMtask(Task& t);
	int getLineWithM(Task& t, std::vector<int>& basis);
	void gauss(std::vector<Number>& row, std::vector<Number>& temp);
	void printBasis(const std::vector<int>& basis, Task& t);
	void print(const std::vector<std::vector<Number>>& table, bool result);
	void simplex(Task& t, std::vector<int>& basis);
	Task readTaskFromFile(const std::string& fileName);
	void makeSimpleTable(Task& t);

	Task current;
	bool validState = true;
public:
	SimplexTableSolver(const std::string& fileName);
	void solve();
	bool isValid() const;
};