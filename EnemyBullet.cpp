#include "EnemyBullet.h"
#include <assert.h>
#include "VectorFunction.h"
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& verocity) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransForm_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransForm_.translation_ = position;

	verocity_ = verocity;
}

void EnemyBullet::Update()
{ 
	worldTransForm_.translation_ += verocity_;
	worldTransForm_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}