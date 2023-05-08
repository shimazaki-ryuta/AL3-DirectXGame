#include "Enemy.h"
#include "VectorFunction.h"
#include <assert.h>
#include "MatrxFunction.h"

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

	//Fire();
	ApproachInitialize();
}
void Enemy::ApproachInitialize()
{ 
	fireTimer = EnemyBullet::kFireInterval; 
}
void Enemy::Approach() {
	worldTransForm_.translation_ += approachVerocity_;
	if (worldTransForm_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	//発射タイマーカウントダウン
	fireTimer--;
	if (fireTimer<0)
	{
		Fire();
		fireTimer = EnemyBullet::kFireInterval;
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
	for (iterator = bullets_.begin(); iterator != bullets_.end(); iterator++) {
		(*iterator)->Update();
	}
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = -2.0f;
	Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

	//velocity = TransformNormal(velocity, worldTransForm_.matWorld_);

	// 弾を生成、初期化
	std::unique_ptr<EnemyBullet> bullet_;
	bullet_.reset(new EnemyBullet());
	bullet_->Initialize(model_, worldTransForm_.translation_, velocity);

	//bullet_ = newBullet;
	bullets_.push_back(std::move(bullet_));
}


void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
	for (iterator = bullets_.begin(); iterator != bullets_.end(); iterator++) {
		(*iterator)->Draw(viewProjection);
	}
}