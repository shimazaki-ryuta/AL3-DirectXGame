#pragma once
#include "Model.h"
#include "WorldTransform.h"

///< summary>
/// 敵の弾
///</summary>
class EnemyBullet {
public:
	//発射間隔
	static const int kFireInterval = 60;

	void Initialize(Model* model, const Vector3& position, const Vector3& verocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 verocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};