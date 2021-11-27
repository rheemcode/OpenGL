#pragma once
#define FUNC_STR __FUNCTION__
#define PROFILE_FUNCTION Profiler profiler(FUNC_STR);

struct Profiler
{
	std::string functionName;
	bool stopped = false;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	std::chrono::duration<double, std::milli> duration;
	float End();
	void Start();
	
	Profiler() {}
	Profiler(const std::string& functionName);
	Profiler(const Profiler& p_profiler);
	~Profiler();
};

