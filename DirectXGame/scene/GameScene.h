#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include "Player.h"
#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;

	// 3Dモデル
	std::unique_ptr <Model> model_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	std::unique_ptr<Player> player_;
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	std::unique_ptr<Skydome> skydome_;
	Model* modelSkydome_ = nullptr;

	std::unique_ptr<Ground> ground_;
	Model* modelGround_ = nullptr;

	std::unique_ptr<FollowCamera> followCamera_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
