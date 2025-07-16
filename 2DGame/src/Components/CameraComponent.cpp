#include "CameraComponent.h"
#include <EntityBase.h>
CameraComponent::CameraComponent(EntityBase* attachedEntity) : ComponentBase(attachedEntity)
{
	this->camera = std::make_shared<Camera2D>();
	this->canTick = false;
	std::cout << "Created Camera Component" << std::endl;
}

void CameraComponent::Start()
{	
	ConfigureCamera();
}

void CameraComponent::Update(const float& deltaTime)
{
	//noop
}

void CameraComponent::OnDestroy()
{
	std::cout << "Destroyed Camera Component" << std::endl;
}

std::shared_ptr<Camera2D> CameraComponent::GetCamera() const
{
	return this->camera;
}


void CameraComponent::ConfigureCamera()
{	
	this->camera->target = {0.0f,0.0f};
	this->camera->offset = { SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 };
	this->camera->rotation = 0.0f;
	this->camera->zoom = 0.0f;
}
