#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <list>
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("zako_2.png");

	// 3Dモデルデータの生成
	model_.reset(Model::CreateFromOBJ("Player", true));
	modelPlayerBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelPlayerHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelPlayerL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelPlayerR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	// std::vector<Model*> modelPlayers_;
	std::vector<HierarchicalAnimation> animationPlayer;
	WorldTransform worldTransformPlayerBody = {
	    .translation_{0.0f, 0.0f, 0.0f}
    };
	WorldTransform worldTransformPlayerHead = {
	    .translation_{0.0f, 1.2f, 0.0f}
    };
	WorldTransform worldTransformPlayerL_arm = {
	    .translation_{-0.5f, 1.5f, 0.0f}
    };
	WorldTransform worldTransformPlayerR_arm = {
	    .translation_{0.5f, 1.5f, 0.0f}
    };

	animationPlayer.push_back({modelPlayerBody_.get(), worldTransformPlayerBody});
	animationPlayer.push_back({modelPlayerHead_.get(), worldTransformPlayerHead});
	animationPlayer.push_back({modelPlayerL_arm_.get(), worldTransformPlayerL_arm});
	animationPlayer.push_back({modelPlayerR_arm_.get(), worldTransformPlayerR_arm});
	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// player_ = new Player();
	player_ = std::make_unique<Player>();
	player_->Initialize(animationPlayer);
	player_->InitializeFloatingGimmick();
	// 天球
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_.reset(new Skydome);
	skydome_->Initialize(modelSkydome_, Vector3(0.0f, 0.0f, 0.0f));

	// 地面
	modelGround_ = Model::CreateFromOBJ("Ground", true);
	ground_.reset(new Ground);
	ground_->Initialize(modelGround_, Vector3(0.0f, 0.0f, 0.0f));

	// カメラ生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// 敵
	modelEnemyBody_.reset(Model::CreateFromOBJ("Enemy_Body", true));
	modelEnemyWheel_.reset(Model::CreateFromOBJ("Enemy_Wheel", true));
	std::vector<HierarchicalAnimation> animationEnemy;
	WorldTransform worldTransformEnemyBody = {.translation_{0.0f,0.5f,0.0f}};
	WorldTransform worldTransformEnemyWheel = {.translation_{0.0f, -0.2f, 0.0f}    };
	animationEnemy.push_back({modelEnemyBody_.get(), worldTransformEnemyBody});
	animationEnemy.push_back({modelEnemyWheel_.get(), worldTransformEnemyWheel});

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(animationEnemy);
}

void GameScene::Update() {
#ifdef _DEBUG
	// デバッグカメラを有効化
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG
	player_->Update();
	enemy_->Update();
	debugCamera_->Update();
	followCamera_->Update();

	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		// viewProjection_.UpdateMatrix();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
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
	/// </su->dmmary>
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);


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
