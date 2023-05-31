#pragma once

#include <sstream>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary> 
	void CheckAllCollisions();

	void AddEnemyBullet(std::unique_ptr<EnemyBullet>);


	/// <summary>
	/// 敵発生
	/// </summary>
	void EnemyPop(const Vector3&);


	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();




private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	Player* player_=nullptr;
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;


	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	//std::list<std::unique_ptr<EnemyBullet>>::iterator iterator;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isWait_ = false;
	int32_t waitTime_;



	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
