#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model*, uint32_t);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection&);

	inline WorldTransform* GetWorldTransform() { return &worldTransform_; };

	inline void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	};

private:
	WorldTransform worldTransform_;
	const ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
};
