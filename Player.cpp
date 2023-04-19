#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model,uint32_t textureHandle) { 
	assert(model);
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	worldTransForm_.Initialize();
}

void Player::Update()
{
	worldTransForm_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) 
{ 
	model_->Draw(worldTransForm_,viewProjection,textureHandle_);
}