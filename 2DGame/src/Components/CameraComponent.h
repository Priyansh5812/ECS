#pragma once
#include <ComponentBase.h>
#include <raylib.h>

typedef struct CameraInitializationData CameraInitData;


class CameraComponent : public ComponentBase
{
public:
	CameraComponent(EntityBase* attachedEntity);
	virtual void Start() override; // Initialization 
	virtual void Update(const float& deltaTime) override; // OnTick
	virtual void OnDestroy() override;// Deinitialization
	std::shared_ptr<Camera2D> GetCamera() const;
	void ConfigureCamera();
private:
	std::shared_ptr<Camera2D> camera;

};



