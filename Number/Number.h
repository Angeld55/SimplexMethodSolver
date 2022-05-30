#ifndef NUMBER_H
#define NUMBER_H

#include <iostream>


//class wrapper for working with rationals and complexs.
//implemented during my OOP course

class Number
{

private:
	int R;  
	int Q;
	int RC;
	int QC;
	void Rationalize(int&, int&);

public:
	Number();
	Number(int R);//Integer
	Number(int R, int Q); //Rational
	Number(int R, int Q, int C);//Rational or Integer with Integer complex part;
	Number(int R, int Q, int C,int CQ);//Rational or Integer with rational or integer complex part;
	int GetR() const;
	int GetQ() const;
	int GetRC() const;
	int GetQC() const;
	bool IsComplex() const;
	bool IsRational() const;
	bool IsNegative() const;
	void Rationalize();
	Number GetConjugate() const;

	friend std::ostream& operator<<(std::ostream&, const Number&);
	Number& operator+=(const Number&);
	Number& operator-=(const Number&);
	Number& operator*=(const Number&);
	Number& operator/=(const Number&);
	Number& operator^=(const Number&);
	Number& operator++();
	//int squareRoot(int n);

	std::string ToString() const;
};
const Number operator+(const Number&, const Number&);
const Number operator-(const Number&, const Number&);
const Number operator*(const Number&, const Number&);
const Number operator/(const Number&, const Number&);
bool  operator==(const Number&, const Number&);
bool  operator< (const Number&, const Number&);
bool  operator!= (const Number&, const Number&);


#endif // !NUMBER_H