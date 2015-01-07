#pragma once
#include <chrono>

class Timer
{
private:
	std::chrono::system_clock::time_point tStart_;
	int secondsAllowed_;

public:
	Timer(int secondsAllowed) :
		secondsAllowed_(secondsAllowed)
	{}

	void launch(){
		tStart_ = std::chrono::high_resolution_clock::now();
	}

	int inline getSecondsElapsed() const{
		std::chrono::system_clock::time_point tNow = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::seconds>(tNow - tStart_).count();
	}

	bool inline isNotFinished() const{
		if (getSecondsElapsed() < secondsAllowed_){
			return true;
		}
		else {
			return false;
		}
	}
};

