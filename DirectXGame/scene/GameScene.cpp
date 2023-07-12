#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

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
	model_.reset(Model::CreateFromOBJ("Player",true));

	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//player_ = new Player();
	player_ = std::make_unique<Player>();
	player_->Initialize(model_.get(), textureHandle_);

	// 天球
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_.reset(new Skydome);
	skydome_->Initialize(modelSkydome_, Vector3(0.0f, 0.0f, 0.0f));

	// 地面
	modelGround_ = Model::CreateFromOBJ("Ground", true);
	ground_.reset(new Ground);
	ground_->Initialize(modelGround_, Vector3(0.0f, 0.0f, 0.0f));
}

void GameScene::Update() {
#ifdef _DEBUG
	// デバッグカメラを有効化
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	player_->Update();
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
	/// </su->dmmary>
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	player_->Draw(viewProjection_);



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
