#include "pch.h"
#include "ActionExpression.h"
#include<stack>
#include<tuple>
#include<cstdlib>
#include<algorithm>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionExpression)

		//vector of operators indicating operator, string, precedence and whether right-to-left associative 
		//precedences taken from cppreference.en
		const Vector <std::tuple<ActionExpression::OperatorType, std::string, uint32_t, bool>> ActionExpression::operators
		=
	{
	std::make_tuple(ActionExpression::OperatorType::ADD, "+", 6, false),
	std::make_tuple(ActionExpression::OperatorType::SUB, "-", 6, false),
	std::make_tuple(ActionExpression::OperatorType::MUL, "*", 5, false),
	std::make_tuple(ActionExpression::OperatorType::DIV, "/", 5, false),
	std::make_tuple(ActionExpression::OperatorType::MOD, "%", 5, false),
	std::make_tuple(ActionExpression::OperatorType::EQU, "==", 10, false),
	std::make_tuple(ActionExpression::OperatorType::NEQU, "!=", 10, false),
	std::make_tuple(ActionExpression::OperatorType::GTE, ">=", 9, false),
	std::make_tuple(ActionExpression::OperatorType::GT, ">", 9, false),
	std::make_tuple(ActionExpression::OperatorType::LTE, "<=", 9, false),
	std::make_tuple(ActionExpression::OperatorType::LT, "<", 9, false),
	std::make_tuple(ActionExpression::OperatorType::ASSIGNMENT, "=", 16, true),
	std::make_tuple(ActionExpression::OperatorType::NOT, "!", 3, true),
	std::make_tuple(ActionExpression::OperatorType::OPEN_PARAN, "(", 100, false),
	std::make_tuple(ActionExpression::OperatorType::CLOSE_PARAN, ")", 100, false)
	};

	const Vector <std::pair<ActionExpression::FunctionType, std::string>> ActionExpression::functions
		=
	{
		std::make_pair(FunctionType::SIN, "sine"),
		std::make_pair(FunctionType::COS, "cos"),
		std::make_pair(FunctionType::SQRT, "sqrt"),
		std::make_pair(FunctionType::POW, "pow")
	};

	ActionExpression::ActionExpression()
		:Action(TypeIdInstance())
	{
	}

	void ActionExpression::ParseIntoVector(const std::string & expression)
	{
		std::string string = expression;
		string.erase(std::remove(string.begin(), string.end(), ' '),
			string.end());

		Vector<Token> tokenVector
			=
		{
			Token(string)
		};


		uint32_t i;
		for (const auto &operatorTuple : operators)
		{
			i = 0;
			while (i < tokenVector.Size())
			{
				if (tokenVector[i].Operator == OperatorType::UNKNOWN)
				{
					std::string& currentString = tokenVector[i].TokenString;
					size_t opIndex = currentString.find(std::get<1>(operatorTuple));
					if (opIndex != std::string::npos)
					{
						std::string firstString = currentString.substr(0, opIndex);
						std::string middleString = currentString.substr(opIndex, std::get<1>(operatorTuple).length());
						std::string endString = currentString.substr(opIndex + std::get<1>(operatorTuple).length());
						Token middleToken(middleString);
						middleToken.Operator = std::get<0>(operatorTuple);
						middleToken.Precedence = std::get<2>(operatorTuple);
						middleToken.RightToLeft = std::get<3>(operatorTuple);
						if (firstString.length() > 0)
						{
							currentString = firstString;
						}
						else
						{
							tokenVector.RemoveAt(i--);
						}
						tokenVector.Insert(middleToken, i + 1);
						if (endString.length() > 0)
						{
							Token endToken(endString);
							tokenVector.Insert(endToken, i + 2);
						}
						++i;
					}
				}
				++i;
			}
		}

		for (const auto &functionPair : functions)
		{
			i = 0;
			while (i < tokenVector.Size())
			{
				if (tokenVector[i].Operator == OperatorType::UNKNOWN)
				{
					if (tokenVector[i].TokenString == functionPair.second)
					{
						tokenVector[i].Function = functionPair.first;
					}
				}
				++i;
			}
		}

		std::stack<Token> opStack;
		mTokenVector.Clear();
		mTokenVector.Reserve(tokenVector.Size());

		for (auto& token : tokenVector)
		{
			if (token.Function != FunctionType::UNKNOWN)
			{
				opStack.push(token);
			}
			else if (token.Operator == OperatorType::UNKNOWN)
			{
				mTokenVector.PushBack(token);
			}
			else if (token.Operator == OperatorType::OPEN_PARAN)
			{
				opStack.push(token);
			}
			else if (token.Operator == OperatorType::CLOSE_PARAN)
			{
				while (!opStack.empty() && opStack.top().Operator != OperatorType::OPEN_PARAN)
				{
					mTokenVector.PushBack(opStack.top());
					opStack.pop();
				}
				opStack.pop();
			}
			else
			{
				while (!opStack.empty() && (((opStack.top().Precedence < token.Precedence)
					|| (opStack.top().Function != FunctionType::UNKNOWN)
					|| (opStack.top().Precedence == token.Precedence && !opStack.top().RightToLeft))
					&& opStack.top().TokenString != "("))
				{
					mTokenVector.PushBack(opStack.top());
					opStack.pop();
				}
				opStack.push(token);
			}
		}
		while (!opStack.empty())
		{
			mTokenVector.PushBack(opStack.top());
			opStack.pop();
		}
	}

	void ActionExpression::Update(WorldState & state)
	{
		state;
		ParseIntoVector(mString);
		std::stack<Token> tokenStack;
		for (auto & token : mTokenVector)
		{
			if (token.Operator != OperatorType::UNKNOWN)
			{
				if (token.Operator == OperatorType::NOT)
				{
					Token operand = tokenStack.top();
					tokenStack.pop();
					operand.TokenValue = operand.TokenValue == 0 ? 1.0f : 0.0f;
					tokenStack.push(operand);
				}
				else
				{
					Token operand2 = tokenStack.top();
					tokenStack.pop();
					Token operand1 = tokenStack.top();
					tokenStack.pop();
					Token result;
					switch (token.Operator)
					{
					case OperatorType::ADD:
						result.TokenValue = operand1.TokenValue + operand2.TokenValue;
						break;
					case OperatorType::SUB:
						result.TokenValue = operand1.TokenValue - operand2.TokenValue;
						break;
					case OperatorType::MUL:
						result.TokenValue = operand1.TokenValue * operand2.TokenValue;
						break;
					case OperatorType::DIV:
						result.TokenValue = operand1.TokenValue / operand2.TokenValue;
						break;
					case OperatorType::MOD:
						result.TokenValue = (float)((int)operand1.TokenValue % (int)operand2.TokenValue);
						break;
					case OperatorType::EQU:
						result.TokenValue = operand1.TokenValue == operand2.TokenValue ? 1.0f : 0.0f;
						break;
					case OperatorType::NEQU:
						result.TokenValue = operand1.TokenValue != operand2.TokenValue ? 1.0f : 0.0f;
						break;
					case OperatorType::GTE:
						result.TokenValue = operand1.TokenValue >= operand2.TokenValue ? 1.0f : 0.0f;
						break;
					case OperatorType::GT:
						result.TokenValue = operand1.TokenValue > operand2.TokenValue ? 1.0f : 0.0f;
						break;
					case OperatorType::LTE:
						result.TokenValue = operand1.TokenValue <= operand2.TokenValue ? 1.0f : 0.0f;
						break;
					case OperatorType::LT:
						result.TokenValue = operand1.TokenValue < operand2.TokenValue ? 1.0f : 0.0f;
						break;
					case OperatorType::ASSIGNMENT:
						assert(operand1.ResultDatum != nullptr);
						if (operand1.ResultDatum->Type() == Datum::DatumType::INTEGER)
						{
							operand1.ResultDatum->Set((int)operand2.TokenValue, operand1.Index);
						}
						else if (operand1.ResultDatum->Type() == Datum::DatumType::FLOAT)
						{
							operand1.ResultDatum->Set(operand2.TokenValue, operand1.Index);
						}
						result.TokenValue = operand2.TokenValue;
						break;
					}
					tokenStack.push(result);
				}
			}
			else if (token.Function != FunctionType::UNKNOWN)
			{
				Token operand1 = tokenStack.top();
				tokenStack.pop();
				Token result;
				switch (token.Function)
				{
				case FunctionType::SIN:
					result.TokenValue = std::sin(operand1.TokenValue);
					break;
				case  FunctionType::COS:
					result.TokenValue = std::cos(operand1.TokenValue);
					break;
				case  FunctionType::SQRT:
					result.TokenValue = std::sqrt(operand1.TokenValue);
					break;
				case  FunctionType::POW:
					result.TokenValue = std::pow(tokenStack.top().TokenValue, operand1.TokenValue);
					tokenStack.pop();
					break;
				}
				tokenStack.push(result);
			}
			else
			{
				char* p;
				float converted = strtof(token.TokenString.c_str(), &p);
				if (!*p)
				{
					token.TokenValue = converted;
				}
				else
				{
					uint32_t index = 0;
					size_t pos = token.TokenString.find("[");
					if (pos != std::string::npos) //sub-index
					{
						std::string tokenString = token.TokenString.substr(0, pos);
						size_t pos2 = token.TokenString.find("]");
						assert(pos2 != std::string::npos);
						index = atoi(token.TokenString.substr(pos + 1, pos2 - pos - 1).c_str());
						token.TokenString = tokenString;
						token.Index = index;
					}

					size_t dotPos = token.TokenString.find(".");
					Scope* current = this;
					if (dotPos != std::string::npos)
					{
						std::string scopeString = token.TokenString.substr(0, dotPos);
						token.TokenString = token.TokenString.substr(dotPos + 1);
						Datum * datum = nullptr;
						while (current != nullptr)
						{
							datum = current->Search("Name");
							if (datum != nullptr && datum->Get<std::string>() == scopeString)
							{
								break;
							}
							current = current->GetParent();
						}
					}
					assert(current != nullptr);
					Datum* result = current->Search(token.TokenString);
					if (result == nullptr)
					{
						throw std::exception("Datum not found");
					}
					if (result->Type() == Datum::DatumType::INTEGER)
					{
						token.TokenValue = (float)result->Get<int>(index);
					}
					else if (result->Type() == Datum::DatumType::FLOAT)
					{
						token.TokenValue = result->Get<float>(index);
					}
					token.ResultDatum = result;
				}
				tokenStack.push(token);
			}
		}
		Result = tokenStack.top();
	}

	Scope * ActionExpression::Clone()
	{
		return new ActionExpression(*this);
	}
}