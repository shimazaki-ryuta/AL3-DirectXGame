#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
enum class Phase {
	Approach,
	Leave,
};

///< summary>
/// 敵
///</summary>
class Enemy {
public:
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
	//void Leave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();
	

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;

	Vector3 approachVerocity_;
	Vector3 leaveVerocity_;

	Phase phase_ = Phase::Approach;


	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	std::list<std::unique_ptr<EnemyBullet>>::iterator iterator;

	int32_t fireTimer = 0;
};