#include "PlayerBullet.h"
#include <assert.h>


void PlayerBullet::Initialize(Model* model, const Vector3& position) 
{
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransForm_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransForm_.translation_ = position;
}

void PlayerBullet::Update()
{
	worldTransForm_.UpdateMatrix(); 

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}