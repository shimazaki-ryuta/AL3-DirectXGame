#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Player;

///< summary>
/// 敵の弾
///</summary>
class EnemyBullet {
public:
	const float Radius = 1.0f;
	const float kSpeed = 1.0f;
	//発射間隔
	static const int kFireInterval = 60;

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	inline bool IsDead() const { return isDead_; };


	Vector3 GetWorldPosition();

	// 衝突時に呼び出されるコールバック関数
	void OnCollision();

	inline void SetPlayer(Player* player) { player_ = player; };

private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	// 自キャラ
	Player* player_ = nullptr;
};