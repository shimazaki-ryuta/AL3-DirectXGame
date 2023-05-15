#include "EnemyState.h"
#include "Enemy.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) {
	enemy_ = enemy;
}

void EnemyStateApproach::Update()
{
	enemy_->Move(enemy_->GetApproachVelocity());
	if (enemy_->GetWorldPosition().z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave(enemy_));
		return;
	}

	// 発射タイマーカウントダウン
	fireTimer--;
	if (fireTimer < 0) {
		enemy_->Fire();
		fireTimer = EnemyBullet::kFireInterval;
	}
}

EnemyStateLeave::EnemyStateLeave(Enemy* enemy) { enemy_ = enemy; }

void EnemyStateLeave::Update()
{ enemy_->Move(enemy_->GetLeaveVelocity()); }