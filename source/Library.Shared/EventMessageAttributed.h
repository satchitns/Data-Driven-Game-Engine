#pragma once
#include "Attributed.h"

namespace FieaGameEngine
{
	class World;

	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)
	public:
		EventMessageAttributed();
		~EventMessageAttributed() = default;
		EventMessageAttributed(const EventMessageAttributed&) = default;
		EventMessageAttributed(EventMessageAttributed&&) = default;
		EventMessageAttributed& operator=(const EventMessageAttributed&) = default;
		EventMessageAttributed& operator=(EventMessageAttributed&&) = default;

		/**
		*@brief Getter for current world
		*@return world
		**/
		World* GetWorld() const;

		/**
		*@brief Getter for subtype
		*@return subtype
		**/
		const std::string& GetSubtype() const;

		/**
		*@brief Setter for world
		*@param world
		**/
		void SetWorld(World& world);

		/**
		*@brief Setter for subtype
		*@param subtype string
		**/
		void SetSubtype(const std::string& type);

		static Vector<Signature> Signatures()
		{
			return {
				Signature("Subtype", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(EventMessageAttributed, mSubtype))
				};
		}
	protected:
		virtual Scope* Clone() override;
	private:
		std::string mSubtype;
		World* mWorld;
	};
}