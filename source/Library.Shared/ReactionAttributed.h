#pragma once
#include "Reaction.h"

namespace FieaGameEngine
{
	class ReactionAttributed :public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)
	public:
		ReactionAttributed();
		~ReactionAttributed();
		ReactionAttributed(const ReactionAttributed&);
		ReactionAttributed(ReactionAttributed&&);
		ReactionAttributed& operator=(const ReactionAttributed&);
		ReactionAttributed& operator=(ReactionAttributed&&);

		/**
		*@brief Called when events of the subscribed types are delived
		*@param publisher is the event and contains payload
		**/
		virtual void Notify(const EventPublisher& publisher) override;

		/**
		*@brief Getter for subtype
		*@return subtype
		**/
		const std::string& GetSubtype() const;

		/**
		*@brief Setter for subtype
		*@param subtype string
		**/
		void SetSubtype(const std::string& type);

		static Vector<Signature> Signatures()
		{
			return Reaction::Signatures().Append(
				{
					Signature("Subtype", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(ReactionAttributed, mSubtype))
				}
			);
		}
	protected:
		virtual Scope* Clone() override;
	private:
		std::string mSubtype;
	};
}