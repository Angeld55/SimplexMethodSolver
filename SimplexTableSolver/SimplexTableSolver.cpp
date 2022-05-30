#include "SimplexTableSolver.h"
#include <algorithm>
#include "../StringUtils/StringUtils.h"

SimplexTableSolver::SimplexTableSolver(const std::string& fileName)
{
	try
	{
		current = readTaskFromFile(fileName);
	}
	catch (std::exception& ex)
	{
		std::cout << "Error while reading the file!" << std::endl;
		std::cout << ex.what() << std::endl;
		validState = false;
	}
}

bool SimplexTableSolver::isValid() const
{
	return validState;
}


void SimplexTableSolver::solve()
{
	if (!validState)
		return;

	printTask(current);
	makeCanonical(current);

	printTask(current);

	std::vector<int> basis = makeMtask(current);
	printTask(current);

	simplex(current, basis);
}


void SimplexTableSolver::printTask(const Task& t)
{
	if (t.isMin)
		std::cout << "min" << std::endl;
	else
		std::cout << "max" << std::endl;

	for (int i = 0; i < t.task.size(); i++)
	{
		if (t.task[i] == M)
			std::cout << "M";
		else
			std::cout << t.task[i];
		std::cout << "*" << t.varNames[i] << "   ";
	}
	std::cout << std::endl << std::endl;

	for (int i = 0; i < t.constrains.size(); i++)
	{
		for (int j = 0; j < t.constrains[i].vars.size(); j++)
		{
			if (t.constrains[i].vars[j] == M)
				std::cout << "M";
			else
				std::cout << t.constrains[i].vars[j];
			std::cout << "*" << t.varNames[j] << "   ";
		}
		std::cout << t.constrains[i].relation << "   " << t.constrains[i].constraint << std::endl;
	}

	for (int i = 0; i < t.positive.size(); i++)
	{
		if (t.positive[i])
			std::cout << t.varNames[i] << ">=" << 0 << "  ";
	}
	std::cout << std::endl << std::endl << std::endl << std::endl;
}

void SimplexTableSolver::addNewVar(Task& t, int row, std::string& relation)
{
	Number m = relation == "<=" ? 1 : -1;
	t.varNames.push_back("x_" + std::to_string(t.varNames.size() + 1));
	t.task.push_back(0);
	t.positive.push_back(1);

	for (int i = 0; i < t.constrains.size(); i++)
		t.constrains[i].vars.push_back(0);
	t.constrains[row].vars[t.varNames.size() - 1] = m;
	t.constrains[row].relation = "=";
}

void SimplexTableSolver::convertToPositive(Task& t, int index)
{
	auto itPos = t.varNames.begin() + index + 1;
	auto newIt = t.varNames.insert(itPos, t.varNames[index] + "(-)");

	auto itPos2 = t.positive.begin() + index + 1;
	auto newIt2 = t.positive.insert(itPos2, 1);
	t.positive[index] = true;

	t.varNames[index] += "(+)";

	for (int i = 0; i < t.constrains.size(); i++)
	{
		auto itPos = t.constrains[i].vars.begin() + index + 1;
		auto newIt = t.constrains[i].vars.insert(itPos, t.constrains[i].vars[index] * -1);
	}

	auto itPos3 = t.task.begin() + index + 1;
	auto newIt3 = t.task.insert(itPos3, t.task[index] * -1);
}

void SimplexTableSolver::makeCanonical(Task& t)
{

	for (int i = 0; i < t.constrains.size(); i++)
	{
		if (t.constrains[i].relation != "=")
			addNewVar(t, i, t.constrains[i].relation);
	}
	if (!t.isMin)
	{
		for (int i = 0; i < t.task.size(); i++)
			t.task[i] *= -1;
		t.isMin = true;
	}
	for (int i = 0; i < t.constrains.size(); i++)
	{
		if (t.constrains[i].constraint < 0)
		{
			for (int j = 0; j < t.constrains[i].vars.size(); j++)
				t.constrains[i].vars[j] *= -1;
			t.constrains[i].constraint *= -1;
		}
	}

	for (int i = 0; i < t.positive.size(); i++)
	{
		if (!t.positive[i])
			convertToPositive(t, i);
	}
}


bool SimplexTableSolver::checkOnlyZeros(Task& t, int column, int skipRow)
{
	for (int i = 0; i < t.constrains.size(); i++)
	{
		if (i == skipRow)
			continue;
		if (t.constrains[i].vars[column] != 0)
			return false;
	}

	return true;
}

std::vector<int> SimplexTableSolver::makeMtask(Task& t)
{
	std::vector<int> basis;

	int mCount = 0;

	for (int i = 0; i < t.constrains.size(); i++)
	{
		bool foundBasisVar = false;
		for (int j = 0; j < t.constrains[i].vars.size(); j++)
		{
			if (t.constrains[i].vars[j] == 1 && checkOnlyZeros(t, j, i))
			{
				basis.push_back(j);
				foundBasisVar = true;
				break;
			}
		}

		if (!foundBasisVar)
		{
			t.task.push_back(M);
			t.varNames.push_back("x_" + std::to_string(t.varNames.size()));
			for (int s = 0; s < t.constrains.size(); s++)
			{
				if (s == i)
					t.constrains[s].vars.push_back(1);
				else
					t.constrains[s].vars.push_back(0);
			}
			t.positive.push_back(1);
			basis.push_back(t.varNames.size() - 1);
		}
	}

	return basis;
}

int SimplexTableSolver::getLineWithM(Task& t, std::vector<int>& basis)
{
	for (int i = 0; i < basis.size(); i++)
	{
		if (t.task[basis[i]] == M)
			return i;
	}
	return -1;
}

void SimplexTableSolver::gauss(std::vector<Number>& row, std::vector<Number>& temp)
{
	for (int i = 0; i < row.size(); i++)
		row[i] += temp[i];
}

void SimplexTableSolver::printBasis(const std::vector<int>& basis, Task& t)
{
	std::cout << "Basis" << std::endl;
	for (int i = 0; i < basis.size(); i++)
		std::cout << t.varNames[basis[i]] << " ";
	std::cout << std::endl;
}

void SimplexTableSolver::print(const std::vector<std::vector<Number>>& table, bool result)
{
	if (!result)
		std::cout << "-------------------------------------------------" << std::endl;
	for (int i = 0; i < table.size(); i++)
	{
		for (int j = 0; j < table[i].size(); j++)
		{
			std::cout << table[i][j] << "        ";
		}
		std::cout << std::endl;
	}
	if (!result)
		std::cout << std::endl << std::endl << std::endl;
	if (result)
		std::cout << "-------------------------------------------------" << std::endl;
}

void SimplexTableSolver::simplex(Task& t, std::vector<int>& basis)
{
	std::vector<Number> simplexTask = t.task;
	std::vector<std::vector<Number>> simplexTable;

	for (int i = 0; i < t.constrains.size(); i++)
	{
		simplexTable.push_back(t.constrains[i].vars);
		simplexTable[simplexTable.size() - 1].push_back(t.constrains[i].constraint);
	}

	for (int i = 0; i < simplexTask.size(); i++)
		std::cout << simplexTask[i] << "     ";
	std::cout << std::endl << std::endl << std::endl;

	while (1)
	{
		printBasis(basis, t);

		print(simplexTable, false);

		std::vector<std::vector<Number>> result;
		result.push_back(std::vector<Number>(t.task.size() + 1));

		int lineWithM = getLineWithM(t, basis);

		for (int i = 0; i < simplexTask.size() + 1; i++)
		{
			if (std::find(basis.begin(), basis.end(), i) != basis.end())
				continue;

			Number current = i == simplexTask.size() ? 0 : simplexTask[i];

			for (int j = 0; j < simplexTable.size(); j++)
			{
				if (j == lineWithM)
					continue;
				auto t1 = simplexTable[j][i];
				auto t2 = t.task[basis[j]];
				current -= (simplexTable[j][i] * t.task[basis[j]]);
			}
			result[0][i] = current;

		}


		if (lineWithM != -1)
		{
			result.push_back(std::vector<Number>(simplexTask.size() + 1));
			for (int i = 0; i < simplexTable[simplexTable.size() - 1].size(); i++)
			{
				if (std::find(basis.begin(), basis.end(), i) != basis.end())
					continue;
				result[result.size() - 1][i] = simplexTable[lineWithM][i] * -1;
			}
		}

		print(result, true);

		//basis change

		Number basisMin = result[result.size() - 1][0];
		int basisMinIndex = 0;

		for (int i = 1; i < result[result.size() - 1].size() - 1; i++) //without the last
		{
			if (result[result.size() - 1][i] < basisMin)
			{
				basisMinIndex = i;
				basisMin = result[result.size() - 1][i];
			}
		}

		if (!(basisMin < 0))
			break;



		// key element
		const Number RATIO_DEFAULT = 999;
		const Number KEY_MIN_DEFAULT = 99999;

		Number ratio = RATIO_DEFAULT;
		int keyElementIndexToLeave = 0;
		Number keyMin = KEY_MIN_DEFAULT;
		for (int i = 0; i < simplexTable.size(); i++)
		{
			if (simplexTable[i][basisMinIndex] == 0 || simplexTable[i][basisMinIndex] < 0)
				continue;

			if ((simplexTable[i][simplexTable[i].size() - 1] / simplexTable[i][basisMinIndex]) < ratio)
			{
				ratio = (simplexTable[i][simplexTable[i].size() - 1] / simplexTable[i][basisMinIndex]);
				keyElementIndexToLeave = i;
				keyMin = simplexTable[i][basisMinIndex];
			}
		}

		if (keyMin == KEY_MIN_DEFAULT && ratio == RATIO_DEFAULT)
			break;

		std::vector<std::vector<Number>> simplexTableNew = simplexTable;

		for (int i = 0; i < simplexTableNew[keyElementIndexToLeave].size(); i++)
			simplexTableNew[keyElementIndexToLeave][i] /= keyMin;


		for (int i = 0; i < simplexTableNew.size(); i++)
		{
			if (i == keyElementIndexToLeave)
				continue;
			if (simplexTableNew[i][keyElementIndexToLeave] == 0)
				continue;

			std::vector<Number> main = simplexTable[keyElementIndexToLeave];
			for (int j = 0; j < main.size(); j++)
			{

				main[j] *= (-1 * (simplexTable[i][basisMinIndex] / keyMin));
			}
			gauss(simplexTableNew[i], main);
		}

		basis[keyElementIndexToLeave] = basisMinIndex;

		simplexTable = simplexTableNew;

	}

}

SimplexTableSolver::Task SimplexTableSolver::readTaskFromFile(const std::string& fileName)
{
	std::ifstream inputFile(fileName);

	if (!inputFile.is_open())
		throw std::exception("Unable to open the file");

	Task currentTask;

	std::string line;

	//variables
	std::getline(inputFile, line);
	std::vector<std::string> variables = StringUtils::getArgs(line);
	currentTask.varNames = std::move(variables);

	std::getline(inputFile, line);
	std::vector<std::string> mainCoefficientsStr = StringUtils::getArgs(line);

	if (mainCoefficientsStr.back() == "min")
		currentTask.isMin = true;
	else if (mainCoefficientsStr.back() == "max")
		currentTask.isMin = false;
	else
		throw std::exception("Invalid min/max value!");
	mainCoefficientsStr.pop_back();

	std::vector<Number> mainCoefficients;
	std::transform(mainCoefficientsStr.begin(), mainCoefficientsStr.end(), back_inserter(mainCoefficients), [](const std::string& str) { return Number(atoi(str.c_str())); });

	currentTask.task = mainCoefficients;


	std::vector<std::string> positiveStr;
	while (1)
	{
		std::getline(inputFile, line);
		std::vector<std::string> constrainArgs = StringUtils::getArgs(line);

		if (constrainArgs.at(constrainArgs.size() - 2).find('=') == std::string::npos)
		{
			positiveStr = std::move(constrainArgs);
			break;
		}
		Number rhs(atoi(constrainArgs.back().c_str()));
		constrainArgs.pop_back();
		std::string rel = constrainArgs.back();
		constrainArgs.pop_back();

		std::vector<Number> coefficients;
		std::transform(constrainArgs.begin(), constrainArgs.end(), back_inserter(coefficients), [](const std::string& str) { return Number(atoi(str.c_str())); });

		currentTask.constrains.push_back({ coefficients, rel, rhs });
	}

	std::vector<bool> positive;
	std::transform(positiveStr.begin(), positiveStr.end(), back_inserter(positive), [](const std::string& str) { if (str != "1" && str != "0")
		throw std::exception("Row of positive variables should contain only 0 and 1"); return str == "1"; });

	currentTask.positive = positive;

	return currentTask;
}

void SimplexTableSolver::makeSimpleTable(Task& t)
{
	printTask(t);
	makeCanonical(t);

	printTask(t);

	std::vector<int> basis = makeMtask(t);
	printTask(t);

	simplex(t, basis);
}