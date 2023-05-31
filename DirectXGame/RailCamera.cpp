#include <imgui.h>
#include "RailCamera.h"
#include "MatrixFunction.h"
#include <algorithm>
void RailCamera::Initialize(const Vector3& position, const Vector3& rotation) {
	worldTransForm_.translation_ = position;
	worldTransForm_.rotation_ = rotation;
	viewProjection_.farZ = 2000;
	viewProjection_.Initialize();
	worldTransForm_.Initialize();
}

void RailCamera::Update()
{
	//worldTransForm_.translation_.z+=0.1f;
	//worldTransForm_.rotation_.y += 0.005f;
	worldTransForm_.translation_.z = std::clamp(worldTransForm_.translation_.z, -100.0f, 100.0f);
	ImGui::Begin("Camere");
	ImGui::SliderFloat3("Translation", &worldTransForm_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotation", &worldTransForm_.rotation_.x, -2.0f, 2.0f);
	ImGui::End();

	// 行列を更新
	worldTransForm_.UpdateMatrix();
	viewProjection_.matView = Inverse(worldTransForm_.matWorld_);
}