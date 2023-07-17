#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

struct HierarchicalAnimation {
	Model* model_;
	WorldTransform worldTransform_;
};

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::vector<HierarchicalAnimation>, uint32_t);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection&);
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();




	inline WorldTransform* GetWorldTransform() { return &worldTransform_; };

	inline void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	};

private:
	WorldTransform worldTransform_;
	const ViewProjection* viewProjection_ = nullptr;
	//Model* model_ = nullptr;
	//Model* modelBody_  = nullptr;
	//Model* modelHead_  = nullptr;
	//Model* modelL_arm_ = nullptr;
	//Model* modelR_arm_ = nullptr;
	//std::vector<Model*> models_;
	std::vector <HierarchicalAnimation> models_;
	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;

	Input* input_ = nullptr;

	//uint16_t period = 120;

};
