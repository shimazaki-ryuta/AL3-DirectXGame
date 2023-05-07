#include "PlayerBullet.h"
#include "VectorFunction.h"
#include <assert.h>


void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");
	velocity_ = velocity;
	worldTransForm_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransForm_.translation_ = position;
}

void PlayerBullet::Update()
{
	worldTransForm_.translation_ += velocity_;

	//行列を更新
	worldTransForm_.UpdateMatrix(); 

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}