#pragma once
#include "RTTI.h"
#include<fstream>
#include "Vector.h"
#include "json\json.h"

namespace FieaGameEngine
{
	class IJsonParseHelper; //forward declare

	/**
	*Parse master for Json input that gets Json data as a string or from a file, reads it into a Json::Value root object
	*and dispatches requests to parse this data into a SharedData object to a set of helpers.
	**/
	class JsonParseMaster final
	{
	public:
		/**
		*@brief Interface that provides a thin wrapper to the objects that the Json will be parsed into, with
		*some additional functionality
		**/
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)
		public:
			SharedData() = default;
			SharedData(const SharedData& other) = default;
			SharedData& operator=(const SharedData& other) = default;
			SharedData(SharedData&& other) = default;
			SharedData& operator=(SharedData&& other) = default;
			virtual ~SharedData() = default;

			/**
			*@brief Clones the SharedData and returns a pointer to the clone.
			**/
			virtual SharedData* Clone() const
			{
				return new SharedData(*this);
			}

			/**
			*@brief Initialize the shared data.
			**/
			virtual void Initialize()
			{
				mDepth = 0;
				mMaster = nullptr;
			}

			/**
			*@brief Set the parse master associated with this SharedData
			*@param master - reference to master
			**/
			void SetJsonParseMaster(JsonParseMaster& master)
			{
				mMaster = &master;
			}

			/**
			*@brief Get the parse master associated with this SharedData
			*@return reference to master
			**/
			JsonParseMaster& GetJsonParseMaster() const
			{
				return *mMaster;
			}

			/**
			*@brief Decrement current depth
			**/
			void DecrementDepth()
			{
				if (mDepth > 0)
				{
					--mDepth;
				}
			}

			/**
			*@brief Increment current depth
			**/
			void IncrementDepth()
			{
				++mDepth;
			}

			/**
			*@brief Return current depth
			**/
			uint32_t Depth() const
			{
				return mDepth;
			}
		protected:
			bool mIsClone = false;
		private:
			uint32_t mDepth = 0;
			JsonParseMaster* mMaster = nullptr;
		};

		JsonParseMaster(SharedData& data);
		JsonParseMaster(const JsonParseMaster&) = delete;
		JsonParseMaster& operator=(const JsonParseMaster&) = delete;
		JsonParseMaster(JsonParseMaster&&);
		JsonParseMaster& operator=(JsonParseMaster&&);
		~JsonParseMaster();

		/**
		*@brief Clone the master and return a pointer to the clone
		**/
		JsonParseMaster* Clone() const;

		/**
		*@brief Add a helper to this parse master's list of handlers
		*@param helper - reference to helper
		**/
		void AddHelper(IJsonParseHelper& helper);

		/**
		*@brief Remove a helper to this parse master's list of handlers
		*@param helper - reference to helper
		**/
		void  RemoveHelper(IJsonParseHelper& helper);

		/**
		*@brief Parse the given json-string into the SharedData
		*@param jsonString - string to parse
		**/
		void Parse(const std::string& jsonString);

		/**
		*@brief Parse the given input stream into the SharedData
		*@param stream - input stream to parse
		**/
		void Parse(std::istream& stream);

		/**
		*@brief Parse the given json file into the SharedData
		*@param name - name of the file to read from and parse
		**/
		void ParseFromFile(const std::string& name);

		/**
		*@brief Return the path of the file being parsed
		*@return file path
		**/
		const std::string & GetFilePath() const;

		/**
		*@brief Return a pointer to SharedData being used
		*@return pointer to SharedData
		**/
		SharedData* GetSharedData() const;

		/**
		*@brief Set internal SharedData to use
		*@param Reference to shared data
		**/
		void SetSharedData(SharedData& data);

		/**
		*@brief Reset the ParseMaster
		**/
		void Reset();

	private:
		void ParseValue(Json::Value& value);
		void TriggerStartHelper(const std::string& name, const Json::Value& value);
		void TriggerValueHelper(const std::string& name, const Json::Value& value);
		void TriggerEndHelper(const std::string& name, const Json::Value& value);
		std::string mFileName;
		SharedData *mData;
		Vector<IJsonParseHelper*> mHelpers;
		bool mIsClone = false;
	};
}