#pragma once
#include "Action.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class ActionExpression final : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action)
	public:
		enum class OperatorType
		{
			UNKNOWN,
			ADD,
			SUB,
			MUL,
			DIV,
			MOD,
			ASSIGNMENT,
			EQU,
			NEQU,
			GTE,
			GT,
			LTE,
			LT,
			NOT,
			OPEN_PARAN,
			CLOSE_PARAN,
			MAX
		};

		enum class FunctionType
		{
			UNKNOWN,
			SIN,
			COS,
			SQRT,
			POW,
			MAX
		};

		class Token
		{
		public:
			Token() = default;
			Token(const std::string& string)
			{
				TokenString = string;
			}
			OperatorType Operator = OperatorType::UNKNOWN;
			FunctionType Function = FunctionType::UNKNOWN;
			uint32_t Precedence = 100;
			bool RightToLeft = false;
			std::string TokenString;
			float TokenValue;
			Datum* ResultDatum = nullptr;
			uint32_t Index = 0;
		};

		ActionExpression();
		~ActionExpression() = default;
		ActionExpression(const ActionExpression&) = delete;
		ActionExpression(ActionExpression&&) = default;
		ActionExpression& operator=(const ActionExpression&) = delete;
		ActionExpression& operator=(ActionExpression&&) = default;

		/**
		*@brief Method to parse an infix expression into a vector in RPN notation
		*@param expression
		**/
		void ParseIntoVector(const std::string& expression);

		/**
		*@brief Method to evaluate an RPN expression
		*@param world state for context
		**/
		void Update(WorldState& state) override;

		/**
		*@brief Returns the string for access
		*@return string for accessing
		**/
		std::string& String()
		{
			return mString;
		}

		Token Result;

		static Vector<Signature> Signatures()
		{
			return Action::Signatures().Append({
				Signature("String", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(ActionExpression, mString))
			});
		}
	private:
		Vector<Token> mTokenVector;
		std::string mString;
		static const Vector < std::tuple<OperatorType, std::string, uint32_t, bool>> operators;
		static const Vector < std::pair<FunctionType, std::string>> functions;
	};
}