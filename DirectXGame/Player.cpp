#include "Player.h"
#include <cassert>
#include <algorithm>
#include "TextureManager.h"
#include"MatrixFunction.h"
#include"VectorFunction.h"
#include "ImGuiManager.h"
#include <numbers>
Player::~Player() {
	// Bulletの解放
	// delete bullet_;
	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet;
	}
	delete sprite2DReticle_;
}
void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) { 
	assert(model);
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	worldTransForm_.translation_ = position;
	worldTransForm_.Initialize();
	input_ = Input::GetInstance();

	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("2DReticle.png");

	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2(float(WinApp::kWindowWidth)/2.0f,float(WinApp::kWindowHeight)/2.0f), Vector4(1.0f,1.0f,1.0f,1.0f),Vector2(0.5f,0.5f) );

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);
}

void Player::Update(const ViewProjection& viewProjection) {

	//ゲームパッドの状態をえる
	XINPUT_STATE joyState;

	

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});

	// 旋回処理
	Rotate();


	Vector3 move = {0,0,0};
	//移動処理
	const float kCharacterSpeed = 0.2f;
	/* if (input_->PushKey(DIK_LEFT))
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
	}*/
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += float(joyState.Gamepad.sThumbLX) / SHRT_MAX * kCharacterSpeed;
		move.z += float(joyState.Gamepad.sThumbLY) / SHRT_MAX * kCharacterSpeed;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		move.y += kCharacterSpeed;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		move.y -= kCharacterSpeed;
	}
	Vector3 xMasked{0.0f, worldTransForm_.rotation_.y, worldTransForm_.rotation_.z};
	worldTransForm_.translation_ += Transform(move, MakeRotateMatrix(xMasked));

	//const float kMoveLimitX = 32.0f;
	//const float kMoveLimitY = 18.0f;
	/*
	worldTransForm_.translation_.x =
	    std::clamp(worldTransForm_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransForm_.translation_.y =
	    std::clamp(worldTransForm_.translation_.y, -kMoveLimitY, kMoveLimitY);
	*/
	

	//行列更新
	//worldTransForm_.matWorld_ =
	//    MakeAffineMatrix(worldTransForm_.scale_, worldTransForm_.rotation_, worldTransForm_.translation_);

	//worldTransForm_.TransferMatrix();

	worldTransForm_.UpdateMatrix();

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


	//3Dレティクル
	{
		const float kDistancePlayer3DReticle = 50.0f;
		
		Vector3 offset = {0,0,1.0f};

		offset = TransformNormal(offset,worldTransForm_.matWorld_);

		offset = Nomalize(offset) * kDistancePlayer3DReticle;

		worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;
		worldTransform3DReticle_.UpdateMatrix();
	}
	//2Dレティクル
	{ 
		Vector3 positionReticle;
		positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
		positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
		positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];

		Matrix4x4 matViewport = MakeViewportMatrix(0,0,WinApp::kWindowWidth,WinApp::kWindowHeight,0,1);

		Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;

		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		//sprite2DReticle_->SetPosition(Vector2(positionReticle.x,positionReticle.y));
	}
	//マウスカーソル
	ScreenToWorld(viewProjection);
}

void Player::ScreenToWorld(const ViewProjection& viewProjection) 
{
	// ゲームパッドの状態をえる
	XINPUT_STATE joyState;

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//spritePosition.x += float(joyState.Gamepad.sThumbRX) / SHRT_MAX * 5.0f;
		//spritePosition.y -= float(joyState.Gamepad.sThumbRY) / SHRT_MAX * 5.0f;

		//sprite2DReticle_->SetPosition(spritePosition);
	}

	//POINT mousePosition;
	//GetCursorPos(&mousePosition);

	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd,&mousePosition);

	//Vector2 clientMousePos = Vector2(float(mousePosition.x), float(mousePosition.y));

	//sprite2DReticle_->SetPosition(clientMousePos);
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	//スクリーン座標系からワールド座標形へ
	posNear = Transform(posNear,matInverseVPV);
	posFar = Transform(posFar,matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Nomalize(mouseDirection);

	const float kDistanceTestObject = 100.0f;

	worldTransform3DReticle_.translation_ = (mouseDirection * kDistanceTestObject) + posNear;
	worldTransform3DReticle_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text(
	    "2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)",posNear.x,posNear.y,posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();

}

void Player::Rotate()
{
	// ゲームパッドの状態をえる
	XINPUT_STATE joyState;

	const float kRotSpeed = 0.02f;
	/* if (input_->PushKey(DIK_A))
	{
		worldTransForm_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		worldTransForm_.rotation_.y += kRotSpeed;
	}*/
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		worldTransForm_.rotation_.y += float(joyState.Gamepad.sThumbRX) / SHRT_MAX * kRotSpeed;
		worldTransForm_.rotation_.x -= float(joyState.Gamepad.sThumbRY) / SHRT_MAX * kRotSpeed;

	}
	worldTransForm_.rotation_.x = std::clamp(worldTransForm_.rotation_.x,-float(std::numbers::pi)/2.0f,float(std::numbers::pi)/2.0f);
}

void Player::Attack() 
{
	// ゲームパッドの状態をえる
	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0,joyState))
	{
		return;
	}

	if (joyState.Gamepad.bRightTrigger>= 250) {
		//弾があれば解放
		/* if (bullet_)
		{
			delete bullet_;
			bullet_ = nullptr;
		}*/
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0.0f,0.0f,kBulletSpeed);

		Vector3 worldPos3DReticle;
		worldPos3DReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
		worldPos3DReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
		worldPos3DReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];

		velocity = worldPos3DReticle - GetWorldPosition();
		velocity = Nomalize(velocity) * kBulletSpeed;


		//velocity = TransformNormal(velocity, worldTransForm_.matWorld_);

		//弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition()+velocity*3.0f, velocity);

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

	//model_->Draw(worldTransform3DReticle_,viewProjection);
}

void Player::DrawUI() 
{
	sprite2DReticle_->Draw();
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransForm_.matWorld_.m[3][0];
	worldPos.y = worldTransForm_.matWorld_.m[3][1];
	worldPos.z = worldTransForm_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision()
{

}