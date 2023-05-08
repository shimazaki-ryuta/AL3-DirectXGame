#include "Player.h"
#include <cassert>
#include <algorithm>
#include"MatrxFunction.h"
#include"VectorFunction.h"
#include "ImGuiManager.h"

Player::~Player() {
	// Bulletの解放
	// delete bullet_;
	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet;
	}
}
void Player::Initialize(Model* model,uint32_t textureHandle) { 
	assert(model);
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	worldTransForm_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0,0,0};
	//移動処理
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	worldTransForm_.translation_ +=move;

	const float kMoveLimitX = 32.0f;
	const float kMoveLimitY = 18.0f;

	worldTransForm_.translation_.x =
	    std::clamp(worldTransForm_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransForm_.translation_.y =
	    std::clamp(worldTransForm_.translation_.y, -kMoveLimitY, kMoveLimitY);
	//旋回処理
	Rotate();


	//行列更新
	worldTransForm_.matWorld_ =
	    MakeAffineMatrix(worldTransForm_.scale_, worldTransForm_.rotation_, worldTransForm_.translation_);

	worldTransForm_.TransferMatrix();

	//攻撃処理
	Attack();
	//弾更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	//キャラクタの座標を表示する
	float* slider3[3] = {
	    &worldTransForm_.translation_.x, &worldTransForm_.translation_.y,
	    &worldTransForm_.translation_.z};
	ImGui::Begin("Player");
	ImGui::SliderFloat3("", *slider3, -100.0f,100.0f);
	ImGui::End();
}

void Player::Rotate()
{
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A))
	{
		worldTransForm_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		worldTransForm_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() 
{
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾があれば解放
		/* if (bullet_)
		{
			delete bullet_;
			bullet_ = nullptr;
		}*/
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0.0f,0.0f,kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransForm_.matWorld_);

		//弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_,worldTransForm_.translation_,velocity);

		//bullet_ = newBullet;
		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection& viewProjection) 
{ 
	model_->Draw(worldTransForm_,viewProjection,textureHandle_);

	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransForm_.matWorld_.m[3][0];
	worldPos.y = worldTransForm_.matWorld_.m[3][1];
	worldPos.z = worldTransForm_.matWorld_.m[3][2];
	return worldPos;
}