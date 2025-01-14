#pragma once
#include<functional>

/// <summary>
/// 時限発動
/// </summary>
class TimedCall
{
public:
	// コンストラクタ
	TimedCall(std::function<void()> method, uint32_t time);

	//更新
	void Update();

	//完了ならtureを返す
	bool isFinished();


private:
	//コールバック
	std::function<void()> method_;

	//残り時間
	uint32_t time_;

	//完了フラグ
	bool isFinish = false;


};