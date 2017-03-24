#include "Expression.h"
#include <ctype.h>
#include <stdexcept>
#include <memory>
#include <math.h>
#include <assert.h>

Expression *ParseAtom(std::string &str, bool unarySign);
Expression *ParseBracke(std::string &str, bool unarySign);
Expression *ParseMulDiv(std::string &str, bool unarySign);
Expression *ParseAddSub(std::string &str, bool unarySign);

void SkipSpaces(std::string &expression)
{
	size_t numSize = 0;
	while (numSize < expression.size()
		&& (expression[numSize] == ' '))
	{
		++numSize;
	}
	expression = expression.substr(numSize);
}

// Skips spaces, then reads until first non-digit character.
// If successful, removes read characters from `expression`
//  and returns true.
bool ParseDouble(std::string &expression, double &result)
{
	std::string remainingStr = expression;
	SkipSpaces(remainingStr);
	size_t numSize = 0;
	if (remainingStr.size() > 0 && isdigit(remainingStr[0]))
	{
		while (numSize < remainingStr.size()
			&& isdigit(remainingStr[numSize]))
		{
			++numSize;
		}
		result = std::stod(remainingStr.substr(0, numSize));
		expression = remainingStr.substr(numSize);

		return true;
	}

	return false;
}

// Skips spaces, then reads next operator symbol.
// If successful, removes read characters from `expression`
//  and returns true.
bool ParseOperator(std::string &expression, Operation &op)
{
	std::string remainingStr = expression;
	SkipSpaces(remainingStr);
	if (remainingStr.empty())
	{
		op = Operation::NOP;
		return false;
	}

	switch (remainingStr[0])
	{
	case '+':
		op = Operation::ADD; break;
	case '-':
		op = Operation::SUB; break;
	case '*':
		op = Operation::MUL; break;
	case '/':
		op = Operation::DIV; break;
	case '%':
		op = Operation::MOD; break;
	default:
		op = Operation::NOP; break;
	}

	const bool succeed = (op != Operation::NOP);
	if (succeed)
	{
		expression = remainingStr.substr(1);
	}
	return succeed;
}

// Parses expressions like: `a`, `a+b±...`, `a-b±...`,
//  where each sub-expression parsed by `ParseMulDiv`.
Expression *ParseAddSub(std::string &str, bool unarySign)
{
	Expression *left = ParseMulDiv(str, unarySign);
	while (true)
	{
		Operation op = Operation::NOP;

		// Don't remove operator from remaining string
		//  when this operator remains unhandled.
		std::string remainingStr = str;
		if (!ParseOperator(remainingStr, op))
		{
			return left;
		}
		switch (op)
		{
		case Operation::ADD:
		case Operation::SUB:
			break;
		default:
			return left;
		}
		str = remainingStr;

		Expression *right = nullptr;
		try
		{
			right = ParseMulDiv(str, unarySign);
		}
		catch (...)
		{
			DisposeExpression(left);
			throw;
		}

		try
		{
			Expression *expr = new Expression;
			expr->pLeft = left;
			expr->pRight = right;
			expr->op = op;
			left = expr;
		}
		catch (...)
		{
			DisposeExpression(left);
			DisposeExpression(right);
			throw;
		}
	}

	return left;
}

// Parses expressions like: `a`, `a*b...`, `a/b...`, `a%b...`
//  where each sub-expression parsed by `ParseBracke`.
Expression *ParseMulDiv(std::string &str, bool unarySign)
{
	Expression *left = ParseBracke(str, unarySign);
	while (true)
	{
		Operation op = Operation::NOP;

		// Don't remove operator from remaining string
		//  when this operator remains unhandled.
		std::string remainingStr = str;
		if (!ParseOperator(remainingStr, op))
		{
			return left;
		}
		switch (op)
		{
		case Operation::MUL:
		case Operation::DIV:
		case Operation::MOD:
			break;
		default:
			return left;
		}
		str = remainingStr;

		Expression *right = nullptr;
		try
		{
			right = ParseBracke(str, unarySign);
		}
		catch (...)
		{
			DisposeExpression(left);
			throw;
		}

		try
		{
			Expression *expr = new Expression;
			expr->pLeft = left;
			expr->pRight = right;
			expr->op = op;
			left = expr;
		}
		catch (...)
		{
			DisposeExpression(left);
			DisposeExpression(right);
			throw;
		}
	}

	return left;
}

// Parses atom expression, like a number.
Expression *ParseAtom(std::string &str, bool unarySign)
{
	Expression *expr = new Expression;
	if (!ParseDouble(str, expr->value))
	{
		DisposeExpression(expr);
		throw std::invalid_argument("Expected number at: " + str);
	}
	if (unarySign)
	{
		expr->value = -expr->value;
	}
	return expr;
}

Expression *CreateExpression(const std::string &expression)
{
	std::string remainingStr = expression;
	Expression *pExpr = ParseAddSub(remainingStr, false);

	SkipSpaces(remainingStr); // just to ensure
	if (!remainingStr.empty())
	{
		const auto message = "Unexpected symbol at: " + remainingStr;
		throw std::runtime_error(message);
	}

	return pExpr;
}

double CalculateExpression(Expression *pExpr)
{
	if (pExpr->op == Operation::NOP)
	{
		return pExpr->value;
	}
	assert(pExpr->pLeft);
	assert(pExpr->pRight);
	CalculateExpression(pExpr->pLeft);
	CalculateExpression(pExpr->pRight);

	switch (pExpr->op)
	{
	case Operation::ADD:
		pExpr->value = pExpr->pLeft->value + pExpr->pRight->value;
		break;
	case Operation::SUB:
		pExpr->value = pExpr->pLeft->value - pExpr->pRight->value;
		break;
	case Operation::MUL:
		pExpr->value = pExpr->pLeft->value * pExpr->pRight->value;
		break;
	case Operation::DIV:
		pExpr->value = pExpr->pLeft->value / pExpr->pRight->value;
		break;
	case Operation::MOD:
		pExpr->value = fmod(pExpr->pLeft->value, pExpr->pRight->value);
		break;
	case Operation::NOP:
		assert(false);
		break;
	}

	return pExpr->value;
}

void DisposeExpression(Expression *pExpression)
{
	if (pExpression)
	{
		if (pExpression->pLeft)
		{
			DisposeExpression(pExpression->pLeft);
			pExpression->pLeft = nullptr;
		}
		if (pExpression->pRight)
		{
			DisposeExpression(pExpression->pRight);
			pExpression->pRight = nullptr;
		}
		delete pExpression;
	}
}

size_t FindCloseBracket(const std::string & str)
{
	auto result = str.find(')', 0);
	if (result != std::string::npos)
	{
		auto openBracket = str.find('(', 1);
		while (openBracket != std::string::npos && 
			openBracket < result)
		{
			result = str.find(')', result + 1);
			openBracket = str.find('(', openBracket + 1);
		}
	}
	return result;
}

Expression *ParseBracke(std::string &str, bool unarySign)
{
	std::string remainingStr = str;
	SkipSpaces(remainingStr);
	if (remainingStr[0] == '+')
	{
		remainingStr = remainingStr.substr(1);
		SkipSpaces(remainingStr);
	}
	else if (remainingStr[0] == '-')
	{
		unarySign = !unarySign;
		remainingStr = remainingStr.substr(1);
		SkipSpaces(remainingStr);
	}
	str = remainingStr;
	Expression *expr = nullptr;
	try
	{
		if (remainingStr[0] == '(')
		{
			auto pos = FindCloseBracket(remainingStr);
			if (pos == std::string::npos)
			{
				throw std::invalid_argument("No closing parenthesis");
			}
			std::string brackeStr = remainingStr.substr(1, pos - 1);
			expr = ParseAddSub(brackeStr, unarySign);
			remainingStr = remainingStr.substr(pos + 1);
		}
		else
		{
			expr = ParseAtom(remainingStr, unarySign);
		}
	}	
	catch (...)
	{
		DisposeExpression(expr);
		throw;
	}
	str = remainingStr;
	return expr;
}
