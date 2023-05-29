#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RailCamera 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	inline const WorldTransform& GetWorldTransform() { return worldTransForm_; };
	inline const ViewProjection& GetViewProjection() { return viewProjection_; };

private:
	WorldTransform worldTransForm_;
	ViewProjection viewProjection_;
};
