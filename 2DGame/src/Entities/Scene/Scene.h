#pragma once
#include <EntityBase.h>

class MyCamera;
class CameraComponent;
class Entity;
class Scene
{
	public:
		Scene();
		virtual void Update() final;
		~Scene();
	
	private:
		virtual void Init() final;
		virtual void Unload() final;

	private:
		std::shared_ptr<CameraComponent> camera;
		Entity* player;


	private:
		void LoadPlayer();
		void LoadCamera();
		void LoadBasicEnts(); // Load the basic entities

};

