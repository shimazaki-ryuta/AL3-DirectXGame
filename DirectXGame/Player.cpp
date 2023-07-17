#include "Player.h"
#include "ImGuiManager.h"
#include "MatrixFunction.h"
#include "VectorFunction.h"
#include <algorithm>
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() {
	// ゲームパッドの状態をえる
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float kCharacterSpeed = 0.3f;

		Vector3 move = {float(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0,
		    float(joyState.Gamepad.sThumbLY) / SHRT_MAX};
	
		move = Nomalize(move) * kCharacterSpeed;
		Vector3 cameraDirectionYcorection = {0.0f,viewProjection_->rotation_.y,0.0f};
		move = Transform(move,MakeRotateMatrix(cameraDirectionYcorection));
		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0)
		{
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}
		worldTransform_.translation_ += move;

	}

	/*
	const float kMoveLimitX = 32.0f;
	const float kMoveLimitY = 18.0f;

	worldTransform_.translation_.x =
	    std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y =
	    std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
		*/
	// 行列更新
	worldTransform_.UpdateMatrix();

	// キャラクタの座標を表示する
	float* slider3[3] = {
	    &worldTransform_.translation_.x, &worldTransform_.translation_.y,
	    &worldTransform_.translation_.z};
	ImGui::Begin("Player");
	ImGui::SliderFloat3("", *slider3, -100.0f, 100.0f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
