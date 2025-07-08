#pragma once
#include <raylib.h>
#include <raymath.h>
#include <ECSManager.h>
#include <Constants.h>
#include <utility>
#include <typeinfo>
#include <stdexcept>
#include <iostream>
#include <memory>
class ComponentBase;


class Entity
{
	public:
		Entity();
		const Vector3& GetPosition();
		void AddPosition(const Vector3& position);
		virtual void Cleanup();
		const int& GetUID() const;
		template<typename T>
		std::shared_ptr<T> TryAddComponent();
		template<typename T>
		std::shared_ptr<T> TryGetComponent();
		template<typename T>
		void TryRemoveComponent();

	private:
		Matrix Transform;
		Vector3 m_position = { 0,0,0 };
		Vector3 m_rotation = { 0,0,0 };
		Vector3 m_scale = { 1,1,1 };
		void InitializeTransform();
		void RegisterEntity();
		int uid = -1;
};




template<typename T>
std::shared_ptr<T> Entity::TryAddComponent()
{
	try
	{
		return ECSManager::RegisterComponent<T>(this).second;
	}
	catch (const std::runtime_error& error)
	{
		std::cout << "Caught exception while creating component of type : " << typeid(T).name() << std::endl;
		std::cout << "Reason : " << error.what() << std::endl;
		return { false , nullptr };
	}
}

template<typename T>
void Entity::TryRemoveComponent()
{
	try
	{
		ECSManager::UnregisterComponent<T>(this);
	}
	catch (const std::runtime_error& error)
	{
		std::cout << "Caught exception while removing component of type : " << typeid(T).name() << std::endl;
		std::cout << "Reason : " << error.what() << std::endl;
	}
}

template<typename T>
std::shared_ptr<T> Entity::TryGetComponent()
{
	try
	{
		return ECSManager::GetComponent<T>(this);
	}
	catch (const std::runtime_error& error)
	{
		std::cout << "Caught exception while getting component of type : " << typeid(T).name() << std::endl;
		std::cout << "Reason : " << error.what() << std::endl;
	}
}
