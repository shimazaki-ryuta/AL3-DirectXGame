#include "EnemyBullet.h"
#include <assert.h>
#include "VectorFunction.h"
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransForm_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransForm_.translation_ = position;
	worldTransForm_.scale_.x = 0.5f;
	worldTransForm_.scale_.y = 0.5f;
	worldTransForm_.scale_.z = 3.0f;

	velocity_ = velocity;
	//解法2
	//y軸周り角度(θy)
	worldTransForm_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Vector3 velocityXY{velocity_.x,0.0f,velocity_.y};
	float besage = Length(velocityXY);
	worldTransForm_.rotation_.x = std::atan2(-velocity_.y,besage);
}

void EnemyBullet::Update()
{ 
	worldTransForm_.translation_ += velocity_;
	worldTransForm_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransForm_.matWorld_.m[3][0];
	worldPos.y = worldTransForm_.matWorld_.m[3][1];
	worldPos.z = worldTransForm_.matWorld_.m[3][2];
	return worldPos;
}


void EnemyBullet::OnCollision() { isDead_ = true; }