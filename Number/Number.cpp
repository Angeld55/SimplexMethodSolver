#include "Number.h"
#include <string>

Number::Number() : R(0), Q(1), RC(0), QC(0) {}

Number::Number(int R) 
{
	this->R = R;
	Q = 1;
	RC = 0;
	QC = 1;
}

Number::Number(int R, int Q) 
{
	this->R = R;
	this->Q = Q;
	if (this->Q == 0)
	{
		this->Q = 1;
		this->R = 0;
	}
	this->RC = 0;
	this->QC = 1;
	Rationalize();
}

Number::Number(int R, int Q, int RC) 
{
	this->R = R;
	this->Q = Q;
	this->RC = RC;
	this->QC = 1;
	if (this->Q == 0)
		this->Q = 1;
	Rationalize();
}

Number::Number(int R, int Q, int RC, int QC)
{
	this->R = R;
	this->Q = Q;
	this->RC = RC;
	this->QC = QC;
	Rationalize();
}

int Number::GetR() const { return R; }
int Number::GetQ() const { return Q; }
int Number::GetRC() const { return RC; }
int Number::GetQC() const { return QC; }

bool Number::IsComplex() const
{
	return RC != 0;
}

bool Number::IsRational() const
{
	return Q != 1;
}
bool Number::IsNegative() const
{
	if (IsRational())
		return R*Q < 0;
	return R < 0;
	

}
void Number::Rationalize()
{
	if (IsRational())
		Rationalize(R, Q); //rationalize real part
	if (IsComplex())
	{
		if (QC != 1)
			Rationalize(RC, QC); //then the complex part
	}
}

void Number::Rationalize(int& f, int& s) //
{
	if (s == 1)
		return;
	int t = abs(f);
	int t2 = abs(s);
	int min = t < t2 ? t : t2;
	for (int i = 2; i <= min; i++)		
	{
		if (f % i == 0 && s % i == 0) 
		{
			f /= i;
			s /= i;
			t = abs(f);
			t2 = abs(s);
			min = t < t2 ? t : t2;
			i = 1;//so it would increment and become 2;
		}
	}
	if (f < 0 && s < 0)
	{
		f *= -1;
		s *= -1;
	}
}

std::string Number::ToString() const
{
	std::string temp = "";
	if (IsComplex())
		temp+= '(';

	if (R != 0)
	{
		if (!IsRational())
			temp+=std::to_string(R);
		else
		{
			temp += std::to_string(R);
			temp += '/';
			temp += std::to_string(Q);
		}
	}
	else {
		temp += std::to_string(R);
	}
	if (IsComplex())
	{
		if (R != 0)
		{
			if (QC < 0 ^ RC < 0)
				temp += '-';
			else
				temp += '+';
		}
		else if (RC < 0 ^ QC < 0)
			temp += '-';

		if (QC == 1)
		{
			temp += std::to_string(abs(RC));
			temp += 'i';
		}
		else
		{
			temp += std::to_string(abs(RC));
			temp += '/';
			temp += std::to_string(abs(QC));
			temp += 'i';
		}
		temp += ')';
		
	}
	return temp;
}

bool  operator==(const Number& l, const Number& r)
{
	return (l.GetR() == r.GetR()) && (l.GetQ() == r.GetQ()) && (l.GetQC() == r.GetQC()) && (l.GetRC() == r.GetRC());
}

std::ostream & operator<<(std::ostream& os, const Number &obj)
{
	os << obj.ToString();
	return os;
}

Number Number::GetConjugate() const
{
	return Number(R, Q, -1 * RC, QC);
}


//int Number::squareRoot(int n)
//{
//	int x = n;
//	int y = 1;
//	while (x > y)
//	{
//		x = (x + y) / 2;
//		y = n / x;
//	}
//	return x;
//}

Number& Number::operator+=(const Number& other)
{
	if (!IsRational() && !other.IsRational())
		R += other.R;
	else
	{
		int newDenominator = Q*other.Q;
		R = R*(newDenominator/Q) + other.R*(newDenominator/other.Q);
		Q = newDenominator;		
	}
	if (IsComplex() || other.IsComplex())
	{
		if (QC == 1 && other.QC == 1)
			RC += other.RC;
		else
		{
			int newDenominator = QC * other.QC;
			RC = RC * (newDenominator / QC) + other.RC * (newDenominator / other.QC);
			QC = newDenominator;
		}
	}
	Rationalize();
	return *this;
}
Number& Number::operator-=(const Number&other)
{
	Number temp(other);
	temp.R *= -1;
	temp.RC *= -1;
	*this += temp;
	return *this;
}
Number& Number:: operator*=(const Number& other)
{
	
	int tempR = R * other.R;
	int tempQ = Q * other.Q;
	int tempR2 = RC * other.RC;
	int tempQ2 = QC * other.QC;
	Number n1(tempR, tempQ);
	Number n2(tempR2, tempQ2);
	n1 -= n2;//Real part.

	int t1 = R;
	int t2 = Q;
	int t3 = other.RC;
	int t4 = other.QC;
	Number complex1(t1 * t3, t2 * t4);
	int r1 = RC;
	int r2 = QC;
	int r3 = other.R;
	int r4 = other.Q;
	Number complex2(r1 * r3, r2 * r4);
	complex1 += complex2;
	R = n1.R;
	Q = n1.Q;
	RC = complex1.R;
	QC = complex1.Q;

	Rationalize();
	return *this;
}

Number& Number:: operator/=(const Number& other)
{
	if (!other.IsComplex())
	{
		R *= other.Q;
		Q *= other.R;
		QC *= other.R;
		RC *= other.Q;
		Rationalize();
		return *this;
	}
	else
	{
		Number conjugate = other.GetConjugate();
		Number divisor = other * conjugate; //allays real;
		*this /= divisor; //recursive call the same operator (this time num is not complex).
		*this *= conjugate; //It rationalize it
	}
	return *this;
}

Number& Number::operator++()
{
	R++;
	return *this;
}

Number& Number::operator^=(const Number& num)
{
	Number copy(*this);
	for (int i = 1; i < num.R; i++)
		*this *= copy;
	return *this;
}

const Number operator+(const Number& l, const Number& r)
{
	return Number(l) += r; 
}

const Number operator-(const Number& l, const Number& r)
{
	return Number(l) -= r;
}

const Number operator*(const Number& l, const Number& r)
{
	return Number(l) *= r;
}

const Number operator/(const Number& l, const Number& r)
{
	return Number(l) /= r;
}

//bool Number::Equals(Term* other)
//{
//	if (other->GetType() != 0)
//		return false;
//	Number* converted = static_cast<Number*>(other);
//
//	return R == converted->R && Q == converted->Q && RC == converted->RC && QC == converted->QC;
//}

bool  operator<(const Number& lhs, const Number& rhs)
{
	if (lhs.IsComplex() || rhs.IsComplex())
		return false;

	return lhs.GetR() * rhs.GetQ() < lhs.GetQ() * rhs.GetR();
}
bool  operator!= (const Number& lhs, const Number& rhs)
{
	return !(lhs == rhs);
}