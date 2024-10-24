#pragma once

#include <shared_mutex>
#include <queue>

template<typename OBJ>

class CUniQueue
{
public:
	//CUniQueue();
	//~CUniQueue();

	using Container = std::queue<OBJ>;
	CUniQueue() :is_run_(true) {};
	virtual ~CUniQueue() {};

	// �����̳ʰ� ��� �ִ��� Ȯ���մϴ�.
	// ������ ������ ���� ������ shared lock��带 ����մϴ�.							
	bool Empty()
	{
		std::shared_lock<std::shared_mutex> sl(sm_);
		return con_.empty();
	}

	// OBJ�� �����̳ʿ��� �����մϴ�. 							
	// �����Ͱ� ���ٸ� condition_variable�� ���ؼ� ��� �մϴ�.							
	bool Pop(OBJ& obj) 
	{
		std::unique_lock<std::shared_mutex> ul(sm_);
		cv_.wait(ul, [this] {return (!con_.empty() || !is_run_); });

		if (!is_run_) { return false; }

		if (!con_.empty()) {
			obj = con_.front();
			con_.pop();
			return true;
		}
		return false;
	}

	// �����͸� �����̳ʿ� �ֽ��ϴ�. 							
	// condition_variable �� ���ؼ� �����մϴ�.							
	void Push(const OBJ& obj) 
	{
		std::unique_lock<std::shared_mutex> sl(sm_);
		con_.push(obj);
		cv_.notify_one();
	}

	// ��� ��� �����忡 ������ �����մϴ�.							
	void NotifyAll()
	{
		cv_.notify_all();
	}

	// �����̳��� ũ�⸦ Ȯ���մϴ�. 							
	// ������ ������ ���� ������ shared lock��带 ����մϴ�.							
	size_t Size()
	{
		std::shared_lock<std::shared_mutex> sl(sm_);
		return con_.size();
	}

	bool IsRun() 
	{ 
		return is_run_; 
	}
	void SetStop() 
	{
		std::unique_lock<std::shared_mutex> ul(sm_);
		is_run_ = false;
		NotifyAll();
	}

private:
	Container						con_;
	std::condition_variable_any		cv_;
	std::shared_mutex				sm_;
	bool							is_run_;
};

