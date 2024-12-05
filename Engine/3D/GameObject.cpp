#include "GameObject.h"

using namespace AobaraEngine;

void GameObject::Initialize()
{
	Collider::Initialize();

#ifdef _DEBUG
	//for (Model* model : models_)
	//{
	//	//model->SetMaterial({ 1.0f,1.0f,1.0f,0.5f });

	//}
#endif // _DEBUG

	

}

void GameObject::Update()
{
	for (Model* model : models_)
	{
		model->Update();
	}
}

void GameObject::Draw(const Camera& camera)
{
	for (Model* model : models_)
	{
		model->Draw(camera);
	}
}

