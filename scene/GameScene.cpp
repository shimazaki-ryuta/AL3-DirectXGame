#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <list>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete debugCamera_;
	delete player_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//テクスチャの読み込み
	textureHandle_ = TextureManager::Load("zako_2.png");

	//3Dモデルデータの生成
	model_ = Model::Create();

	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280,720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	player_ = new Player();
	player_->Initialize(model_,textureHandle_);

	//敵の生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_, Vector3(10.0f, 2.0f, 30.0f), Vector3(0.0f, 0.0f, -0.1f));
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
#ifdef _DEBUG
	//デバッグカメラを有効化
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG
	if (isDebugCameraActive_)
	{
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else
	{
		viewProjection_.UpdateMatrix();
	}


	player_->Update(); 

	if (enemy_)
	{
		enemy_->Update();
	}

	CheckAllCollisions();

	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
	std::list<std::unique_ptr<EnemyBullet>>::iterator iterator;

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();
	for (iterator = enemyBullets.begin(); iterator != enemyBullets.end(); iterator++) {
		posB = (*iterator)->GetWorldPosition();

		float distance = float(std::pow(posB.x - posA.x, 2) + std::pow(posB.y - posA.y, 2) +
		                 std::pow(posB.z - posA.z, 2));
		
		if (distance <= std::pow(player_->Radius + (*iterator)->Radius,2))
		{
			player_->OnCollision();
			(*iterator)->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		
		float distance = float(
		    std::pow(posB.x - posA.x, 2) + std::pow(posB.y - posA.y, 2) +
		    std::pow(posB.z - posA.z, 2));

		if (distance <= std::pow(enemy_->Radius + bullet->Radius, 2)) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (iterator = enemyBullets.begin(); iterator != enemyBullets.end(); iterator++) {
		posA = (*iterator)->GetWorldPosition();
		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetWorldPosition();

			float distance = float(
			    std::pow(posB.x - posA.x, 2) + std::pow(posB.y - posA.y, 2) +
			    std::pow(posB.z - posA.z, 2));

			if (distance <= std::pow(bullet->Radius + (*iterator)->Radius, 2)) {
				(*iterator)->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
}
