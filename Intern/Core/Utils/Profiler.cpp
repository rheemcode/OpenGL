#include <glpch.h>
#include "Profiler.h"
#include "Console.h"

Profiler::Profiler(const std::string& p_functionName)
	:functionName(p_functionName)
{
	start = std::chrono::high_resolution_clock().now();
}

Profiler::Profiler(const Profiler& p_profiler)
	: functionName(p_profiler.functionName)
{
	start = p_profiler.start;
}

void Profiler::Start()
{
	start = std::chrono::high_resolution_clock().now();
	stopped = false;
}

float Profiler::End()
{
	stopped = true;
	end = std::chrono::high_resolution_clock().now();
	duration = end - start;
	return duration.count();
}

Profiler::~Profiler()
{
	if (stopped)
		return;
	end = std::chrono::high_resolution_clock().now();
	duration = end - start;
	std::stringstream ss;
	ss << "Function [" << functionName << "]" << "took: " << duration.count() << "ms\n";
	Console::Log(LogMode::DEBUG, ss.str());
}