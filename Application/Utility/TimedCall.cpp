#include "TimedCall.h"

// コンストラクタ
TimedCall::TimedCall(std::function<void()> method, uint32_t time) : method_(method), time_(time)
{

}

// 更新
void TimedCall::Update()
{
	if (this->isFinish == true)
	{
		return;
	}

	this->time_--;

	if (this->time_ <= 0)
	{
		//完了にする
		this->isFinish = true;
		//コールバック関数呼び出し
		this->method_();

	}

}

// 完了ならtureを返す
bool TimedCall::isFinished()
{
	return this->isFinish;
}