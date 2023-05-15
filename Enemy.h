#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "EnemyBullet.h"
#include "EnemyState.h"
#include <list>
enum class Phase {
	Approach,
	Leave,
};

class Player;

///< summary>
/// 敵
///</summary>
class Enemy {
public:
	const float Radius = 5.0f;


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>]
	/// <param name="positon">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	///フェーズ
	void Approach();
	void Leave();
	//フェーズ初期化
	void ApproachInitialize();

	void ChangeState(BaseEnemyState*);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();
	
	/// <summary>
	/// 移動
	/// </summary>
	void Move(const Vector3& velocity);
	


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	inline void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();
	//inline Vector3 GetPosition() { return worldTransForm_.translation_; };
	inline Vector3 GetApproachVelocity() { return approachVerocity_; };
	inline Vector3 GetLeaveVelocity() { return leaveVerocity_; };


	//衝突時に呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	inline std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }


private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;

	Vector3 approachVerocity_;
	Vector3 leaveVerocity_;

	Phase phase_ = Phase::Approach;

	//void (Enemy::*pFunc)();
	//static void(Enemy::*phaseTable[])();

	BaseEnemyState* state_ = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	std::list<std::unique_ptr<EnemyBullet>>::iterator iterator;

	int32_t fireTimer = 0;

	//自キャラ
	Player* player_ = nullptr;
};