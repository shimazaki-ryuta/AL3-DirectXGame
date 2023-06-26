#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <list>
#include <fstream>
#include "PrimitiveDrawer.h"
#include "Curve.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete debugCamera_;
	delete player_;
	//delete enemy_;
	delete modelSkydome_;
	delete railCamera_;
	delete collisionManager_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//テクスチャの読み込み
	textureHandle_ = TextureManager::Load("zako_2.png");

	//3Dモデルデータの生成
	model_ = Model::Create();

	//レティクルのテクスチャ
	TextureManager::Load("2DReticle.png");


	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280,720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	Vector3 playerPosition(0,0,30.0f);
	player_ = new Player();
	player_->Initialize(model_,textureHandle_,playerPosition);
	player_->SetGameScene(this);

	//敵発生コマンドの読み取り
	LoadEnemyPopData();
	/*
	//敵の生成
	std::unique_ptr<Enemy> enemy;
	enemy.reset(new Enemy());
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemy->Initialize(model_, Vector3(10.0f, 2.0f, 50.0f), Vector3(0.0f, 0.0f, -0.1f));
	enemys_.push_back(std::move(enemy));
	*/

	//天球
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_.reset(new Skydome);
	skydome_->Initialize(modelSkydome_,Vector3(0.0f,0.0f,0.0f));

	controlPoints_ = {
	    {0,  0,  0},
        {0, 0, 20},
        {0, 0, 40},
        {0, 0, 60},
        {0, 0,  80},
        {0, 0,  100},
        {0, 0, 110},
        {0, 0, 120},
        {0, 0, 130},
	};

	//レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize(Vector3{0.0f, 0.0f, -10.0f}, Vector3{0.0f, 0.0f, 0.0f});
	railCamera_->SetControlPoints(controlPoints_);
	//親子関係
	player_->SetParent(&railCamera_->GetWorldTransform());

	collisionManager_ = new CollisionManager();

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
		//viewProjection_.UpdateMatrix();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});
	// デスフラグの立ったEnemyを削除
	enemys_.remove_if([](std::shared_ptr<Enemy>& enemy) {
		if (enemy->IsDead()) {
			return true;
		}
		return false;
	});

	player_->Update(viewProjection_); 
	/*
	if (enemy_)
	{
		enemy_->Update();
	}
	*/

	//敵発生コマンド
	UpdateEnemyPopCommands();

	//敵更新
	for (std::list<std::shared_ptr<Enemy>>::iterator iterator = enemys_.begin();
	     iterator != enemys_.end(); iterator++) {
		(*iterator)->Update();
	}
	//敵弾更新
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator iterator = enemyBullets_.begin();
	     iterator != enemyBullets_.end(); iterator++) {
		(*iterator)->Update();
	}

	//CheckAllCollisions();
	UseCollisionManager();

	skydome_->Update();

	debugCamera_->Update();
	railCamera_->Update();
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
	/* if (enemy_) {
		enemy_->Draw(viewProjection_);
	}*/
	for (std::list<std::shared_ptr<Enemy>>::iterator iterator = enemys_.begin();
	     iterator != enemys_.end(); iterator++) {
		(*iterator)->Draw(viewProjection_);
	}
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator iterator = enemyBullets_.begin();
	     iterator != enemyBullets_.end(); iterator++) {
		(*iterator)->Draw(viewProjection_);
	}


	skydome_->Draw(viewProjection_);
	DrawCurve(controlPoints_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::UseCollisionManager()
{
	collisionManager_->ClearList();

	collisionManager_->PushCollider(player_);
	for (std::list<std::shared_ptr<Enemy>>::iterator enemy = enemys_.begin();
	     enemy != enemys_.end(); enemy++) {
		collisionManager_->PushCollider(enemy->get());
	}
	for (PlayerBullet* playerBullet : player_->GetBullets()) {
		collisionManager_->PushCollider(playerBullet);
	}
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator enemyBullet = enemyBullets_.begin();
	     enemyBullet != enemyBullets_.end(); enemyBullet++) {
		collisionManager_->PushCollider(enemyBullet->get());
	}

	collisionManager_->CheckAllCollisions();

}

void GameScene::CheckAllCollisions() {
	//Vector3 posA, posB;
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	std::list<Collider*> colliders_;
	colliders_.push_back(player_);
	for (std::list<std::shared_ptr<Enemy>>::iterator enemy = enemys_.begin();
	     enemy != enemys_.end(); enemy++) {
		colliders_.push_back(enemy->get());
	}
	for (PlayerBullet* playerBullet : playerBullets) {
		colliders_.push_back(playerBullet);
	}
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator enemyBullet = enemyBullets_.begin();
	     enemyBullet != enemyBullets_.end(); enemyBullet++) {
		colliders_.push_back(enemyBullet->get());
	}

	
	//std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
	//std::list<std::unique_ptr<EnemyBullet>>::iterator iterator;

#pragma region 自弾と敵キャラの当たり判定

	 for (std::list<Collider*>::iterator iteA = colliders_.begin(); iteA != colliders_.end();
	          ++iteA) {
		Collider* colliderA = *iteA;
		std::list<Collider*>::iterator iteB = iteA;
		iteB++;
		for (; iteB != colliders_.end();++iteB) 
		{
			Collider* collierB = *iteB;
			CheckCollisionPair(colliderA,collierB);
		}		
	}
	
#pragma endregion
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{ 
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) || 
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
	{
		return;
	}

	Vector3 posA = colliderA->GetWorldPosition(), posB = colliderB->GetWorldPosition();
	float distance = float(
	    std::pow(posB.x - posA.x, 2) + std::pow(posB.y - posA.y, 2) + std::pow(posB.z - posA.z, 2));
	if (distance <= std::pow(colliderA->GetRadius() + colliderB->GetRadius(), 2)) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}


void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> bullet)
{
	enemyBullets_.push_back(std::move(bullet));
}


void GameScene::EnemyPop(const Vector3& position)
{
	// 敵の生成
	std::unique_ptr<Enemy> enemy;
	enemy.reset(new Enemy());
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemy->Initialize(model_, position, Vector3(0.0f, 0.0f, -0.1f));
	enemys_.push_back(std::move(enemy));
}

void GameScene::LoadEnemyPopData()
{

	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());
	
	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	if (isWait_)
	{
		waitTime_--;
		if (waitTime_ <=0)
		{
			isWait_ = false;
		}
		return;
	}


	std::string line;
	
	while (getline(enemyPopCommands,line)) 
	{
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0)
		{
			getline(line_stream, word, ',');
			float x = float(std::atof(word.c_str()));
			getline(line_stream, word, ',');
			float y = float(std::atof(word.c_str()));
			getline(line_stream, word, ',');
			float z = float(std::atof(word.c_str()));

			EnemyPop(Vector3(x,y,z));
		}

		//WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');
			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTime_ = waitTime;

			break;
		}
	}

}

std::shared_ptr<Enemy> GameScene::GetLockonEnemy(const Vector2& cursorPosition) {
	const float lockonLength = 30;
	for (std::list<std::shared_ptr<Enemy>>::iterator iterator = enemys_.begin();
	     iterator != enemys_.end(); iterator++) {
		Vector3 enemyPosition3d= (*iterator)->GetWorldPosition();
		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		Matrix4x4 matViewProjectionViewport =
		    viewProjection_.matView * viewProjection_.matProjection * matViewport;

		enemyPosition3d = Transform(enemyPosition3d, matViewProjectionViewport);
		Vector2 screenEnemyPosition(enemyPosition3d.x,enemyPosition3d.y);
		float distance2 = float(std::pow(screenEnemyPosition.x - cursorPosition.x, 2) + std::pow(screenEnemyPosition.y - cursorPosition.y, 2));

		if (distance2 <= float(std::pow(lockonLength, 2)))
		{
			return *iterator;
		}
	}
	return nullptr;
}