#pragma once

namespace FieaGameEngine
{
	template<typename T>
	bool Factory<T>::Find(const std::string& name, const Factory<T>*& outFactory)
	{
		FactoryMap::Iterator iterator = sFactoryMap.Find(name);
		if (iterator == sFactoryMap.end())
		{
			return false;
		}
		outFactory = iterator->second;
		return true;
	}

	template<typename T>
	T* Factory<T>::Create(const std::string& name)
	{
		const Factory<T>* factory = nullptr;
		if (!Find(name, factory))
		{
			return nullptr;
		}
		return factory->Create();
	}

	template<typename T>
	void Factory<T>::Add(const Factory<T>& concreteFactory)
	{
		sFactoryMap[concreteFactory.ClassName()] = &concreteFactory;
	}

	template<typename T>
	void Factory<T>::Remove(const Factory<T>& concreteFactory)
	{
		sFactoryMap.Remove(concreteFactory.ClassName());
	}

	template<typename T>
	typename Factory<T>::FactoryMap::Iterator Factory<T>::begin()
	{
		return sFactoryMap.begin();
	}

	template<typename T>
	typename Factory<T>::FactoryMap::Iterator Factory<T>::end()
	{
		return sFactoryMap.end();
	}

	template<typename T>
	typename Factory<T>::FactoryMap Factory<T>::sFactoryMap;
}