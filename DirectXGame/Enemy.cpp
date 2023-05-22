#include "Enemy.h"
#include "VectorFunction.h"
#include <assert.h>
#include "MatrxFunction.h"
#include "Player.h"

/* void (Enemy::*Enemy::phaseTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave
};*/

Enemy::~Enemy()
{
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
}

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
	state_ = new EnemyStateApproach(this);
	//state_->Initialize(this);
	//Fire();
	//ApproachInitialize();
	//pFunc = &Enemy::Approach;
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
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});

	//終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			return true;
		}
		return false;
	});
	/*
	switch (phase_) {
	default:
	case Phase::Approach:
		Approach();	
		break;
	case Phase::Leave:
		Leave();
		break;
	}
	*/

	//(this->*pFunc)();
	//(this->*phaseTable[static_cast<size_t>(phase_)])();
	state_->Update();
	//worldTransForm_.translation_ += velocity_;

	// 行列を更新
	worldTransForm_.UpdateMatrix();

	//弾を生成(弾の更新より前にやらないとエラーが起きる)
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}
	//弾の更新
	for (iterator = bullets_.begin(); iterator != bullets_.end(); iterator++) {
		(*iterator)->Update();
	}

}

void Enemy::ChangeState(BaseEnemyState* state)
{

	delete state_;
	state->Initialize(this);
	state_ = state;
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity;

	Vector3 end = player_->GetWorldPosition();
	Vector3 start = GetWorldPosition();
	velocity = end - start;
	velocity = Nomalize(velocity);

	velocity = Multiply(double(kBulletSpeed),velocity);

	// 弾を生成、初期化
	std::unique_ptr<EnemyBullet> bullet_;
	bullet_.reset(new EnemyBullet());
	bullet_->Initialize(model_, worldTransForm_.translation_, velocity);

	//bullet_ = newBullet;
	bullets_.push_back(std::move(bullet_));
}

void Enemy::FireCall() 
{ 
	Fire();

	 timedCalls_.push_back(
	    new TimedCall(std::bind(&Enemy::FireCall, this), EnemyBullet::kFireInterval));
}

void Enemy::Move(const Vector3& velocity) {
	worldTransForm_.translation_ += velocity;
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransForm_, viewProjection, textureHandle_);
	for (iterator = bullets_.begin(); iterator != bullets_.end(); iterator++) {
		(*iterator)->Draw(viewProjection);
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransForm_.translation_.x;
	worldPos.y = worldTransForm_.translation_.y;
	worldPos.z = worldTransForm_.translation_.z;
	return worldPos;
}

void Enemy::OnCollision()
{

}