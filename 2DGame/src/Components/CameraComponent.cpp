#include "CameraComponent.h"
#include <EntityBase.h>
CameraComponent::CameraComponent(EntityBase* attachedEntity) : ComponentBase(attachedEntity)
{
	this->camera = std::make_shared<Camera2D>();
	this->canTick = true;
	std::cout << "Created Camera Component" << std::endl;
}

void CameraComponent::Start()
{	
	//ConfigureCamera(data);
}

void CameraComponent::Update(const float& deltaTime)
{

	
}

void CameraComponent::OnDestroy()
{
	std::cout << "Destroyed Camera Component" << std::endl;
}

std::shared_ptr<Camera2D> CameraComponent::GetCamera() const
{
	return this->camera;
}


void CameraComponent::ConfigureCamera(const CameraInitData& data)
{	
	Vector3 pos = data.targetEntity->GetPosition();
	pos = Vector3Add(this->attachedEntity->GetPosition(), pos);
	this->camera->target = { pos.x, pos.y };
	this->camera->offset = data.screenOffset;
	std::cout << data.screenOffset.x << " : " << data.screenOffset.y << std::endl;
	this->camera->rotation = data.rotation;
	this->camera->zoom = data.zoom;
}
