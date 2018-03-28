#pragma once
#include "HashMap.h"
#include<string>
#include<gsl/gsl>

namespace FieaGameEngine
{
	/**
	*@brief Factory class that acts as both Factory Manager and concrete factory. The manager is the static interface of the class,
	*and provides an add, remove, a list of factories, and a Find and Create. The concrete factories have to implement their own Create
	*and ClassName methods and return them as an abstract product type.
	**/
	template<typename T>
	class Factory
	{
	public:
		using FactoryMap = HashMap<std::string, const Factory<T>*>;

		Factory() = default;
		virtual ~Factory() = default;
		Factory(const Factory&) = default;
		Factory& operator=(const Factory&) = default;
		Factory(Factory&&) = default;
		Factory& operator=(Factory&&) = default;

		/**
		*@brief Finds if a factory is registered to the list and returns a bool, and also a pointer by reference if it exists
		*@param name - name of the factory
		*@param outFactory - reference to a pointer to a factory
		*@return true if factory exists
		**/
		static bool Find(const std::string& name, const Factory<T>*& outFactory);

		/**
		*@brief Creates an object of the specified type and returns it (nullptr if it doesn't exist)
		*@param name - name of the factory
		*@return pointer to the newly created object
		**/
		static gsl::owner<T*> Create(const std::string& name);

		/**
		*@brief Adds a new concrete factory to the list of factories
		*@param concreteFactory - reference to the factory to add
		**/
		static void Add(const Factory<T>& concreteFactory);

		/**
		*@brief Removes a new concrete factory to the list of factories
		*@param concreteFactory - reference to the factory to remove
		**/
		static void Remove(const Factory<T>& concreteFactory);

		/**
		*@brief Returns an iterator to the first factory in the list
		*@return FactoryMap::Iterator
		**/
		static typename FactoryMap::Iterator begin();

		/**
		*@brief Returns an iterator to the last factory in the list
		*@return FactoryMap::Iterator
		**/
		static typename FactoryMap::Iterator end();

		/**
		*@brief Removes the size of the list of factories registered
		*@return size
		**/
		static uint32_t Size()
		{
			return sFactoryMap.Size();
		}

		/**
		*@brief Creates a concrete product in a concrete factory and returns it as abstract product
		*@return pointer to an abstract product
		**/
		virtual T* Create() const = 0;

		/**
		*@brief Returns the name of the concrete product
		*@return name of the product as a const string reference
		**/
		virtual const std::string& ClassName() const = 0;
	private:
		static FactoryMap sFactoryMap;
	};

#define ConcreteFactory(ConcreteProductType, AbstractProductType)									\
	class ConcreteProductType##Factory : public Factory<AbstractProductType>						\
	{																								\
	public:																							\
		ConcreteProductType##Factory()																\
		{																							\
			Add(*this);																				\
		}																							\
																									\
		~ConcreteProductType##Factory()																\
		{																							\
			Remove(*this);																			\
		}																							\
																									\
		gsl::owner<AbstractProductType*> Create() const override									\
		{																							\
			return new ConcreteProductType();														\
		}																							\
																									\
		const std::string& ClassName() const override												\
		{																							\
			return mName;																			\
		}																							\
	private:																						\
		std::string mName = std::string(#ConcreteProductType);										\
	};																																																				
}

#include "Factory.inl"