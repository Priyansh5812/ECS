#pragma once
#include <ComponentBase.h>
class Sprite : public ComponentBase
{	
	public:
		Sprite(EntityBase* attachedEntity);
		virtual void Start() override; // Initialization 
		virtual void Update(const float& deltaTime) override; // OnTick
		virtual void OnDestroy() override;// Deinitialization
	private:
		void Draw();
};

