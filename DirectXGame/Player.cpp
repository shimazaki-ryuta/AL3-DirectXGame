#include "Player.h"
#include "ImGuiManager.h"
#include "MatrixFunction.h"
#include "VectorFunction.h"
#include <algorithm>
#include <cassert>
#include <numbers>
void Player::Initialize(std::vector<HierarchicalAnimation> model, uint32_t textureHandle) {
	//assert(model);
	//this->model_ = model;
	//modelBody_ = model[0];
	//modelHead_ = model[1];
	//modelL_arm_ = model[2];
	//modelR_arm_ = model[3];
	models_ = model;

	this->textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();

	for (HierarchicalAnimation& model_ : models_) {
		model_.worldTransform_.Initialize();
		model_.worldTransform_.UpdateMatrix();
	}
	std::vector<HierarchicalAnimation>::iterator body = models_.begin();
	body->worldTransform_.parent_ = &worldTransform_;
	for (std::vector<HierarchicalAnimation>::iterator childlen = models_.begin() + 1;
	     childlen != models_.end(); childlen++) {
		childlen->worldTransform_.parent_ = &(body->worldTransform_);
	}
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


	UpdateFloatingGimmick();
	for (HierarchicalAnimation& model : models_) {
		model.worldTransform_.UpdateMatrix();
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	//model_->Draw(worldTransform_, viewProjection);
	for (HierarchicalAnimation& model : models_)
	{
		model.model_->Draw(model.worldTransform_, viewProjection);
	}
}

void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f;
}


void Player::UpdateFloatingGimmick()
{
	static uint16_t period = 120;
	const float step = 2.0f * float(std::numbers::pi) / float(period);

	floatingParameter_ += step;

	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(std::numbers::pi));

	static float floatingAmplitude = 0.5;
	models_[0].worldTransform_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;
	models_[2].worldTransform_.rotation_.x = std::cos(floatingParameter_) * floatingAmplitude;
	models_[3].worldTransform_.rotation_.x = std::cos(floatingParameter_) * floatingAmplitude;



	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Transform", &models_[1].worldTransform_.translation_.x,-10.0f,10.0f);
	ImGui::SliderFloat3(
	    "ArmL Transform", &models_[2].worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3(
	    "ArmR Transform", &models_[3].worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderInt("Period", (reinterpret_cast<int*>(&period)), 1, 180);
	ImGui::SliderFloat("Ampritude", &floatingAmplitude,0.0f,10.0f);
	ImGui::End();
}
