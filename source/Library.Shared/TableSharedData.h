#pragma once
#include "JsonParseMaster.h"
#include "Scope.h"
#include<stack>

namespace FieaGameEngine
{
	/**
	*SharedData class for parsing tables from Json data
	**/
	class TableSharedData final : public JsonParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TableSharedData, JsonParseMaster::SharedData)
	public:
		TableSharedData(Scope &scope);
		TableSharedData(const TableSharedData&) = delete;
		TableSharedData& operator=(const TableSharedData&) = delete;
		TableSharedData(TableSharedData&&);
		TableSharedData& operator=(TableSharedData&&);
		~TableSharedData();

		/**
		*@brief Returns the address of current scope the internal pointer holds
		*@return address of current scope the internal pointer holds
		**/
		Scope* GetScope() const;

		/**
		*@brief Sets the current scope to the reference's address
		*@param scope - reference of scope to point internal scope to
		**/
		void SetScope(Scope& scope);

		/**
		*@brief Clones the SharedData and returns a pointer to the clone.
		**/
		SharedData* Clone() const override;

		/**
		*@brief Initialize the shared data.
		**/
		void Initialize() override;
	private:
		Scope * mScope = nullptr;
	};
}