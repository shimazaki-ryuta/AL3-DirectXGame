#include "Enemy.h"
#include "VectorFunction.h"
#include <assert.h>

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");
	worldTransForm_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransForm_.translation_ = position;
	velocity_ = velocity;
	approachVerocity_ = velocity;
	leaveVerocity_ = Vector3(-0.1f,0.1f,0.0f);
}

void Enemy::Approach() {
	worldTransForm_.translation_ += approachVerocity_;
	if (worldTransForm_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave() { worldTransForm_.translation_ += leaveVerocity_; }

void Enemy::Update() {
	

	switch (phase_) {
	default:
	case Phase::Approach:
		Approach();	
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	//worldTransForm_.translation_ += velocity_;

	// 行列を更新
	worldTransForm_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
}