#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "PlayerBullet.h"

#include <list>

class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model*,uint32_t);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection&);


private:
	WorldTransform worldTransForm_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;
};
