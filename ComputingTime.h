#pragma once
#include <chrono>

class ComputingTime
{
private:
	std::chrono::system_clock::time_point tStart_;
	int totalTime_;

public:
	ComputingTime() :
		totalTime_(0)
	{}

	void inline start(){
		tStart_ = std::chrono::high_resolution_clock::now();
	}

	int inline getTimeElapsed(){
		std::chrono::system_clock::time_point tNow = std::chrono::high_resolution_clock::now();
		int res = std::chrono::duration_cast<std::chrono::microseconds>(tNow - tStart_).count();
		totalTime_ += res;
		return res;
	}

	int inline getTotalTime() const{
		return totalTime_;
	}
};