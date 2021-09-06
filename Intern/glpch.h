#pragma once

#define NOMINMAX


#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <istream>
#include <map>
#include <unordered_map>
#include <memory>
#include <array>
#include <cmath>
#include <cstdint>
#include <exception>
#include <dwmapi.h>
#include <thread>
#include <mutex>
#include <functional>
#include <malloc.h>
#include <cassert>
#include <chrono>
#include <immintrin.h>

#if defined(near) || defined(far)
	#undef far
	#undef near
#endif
