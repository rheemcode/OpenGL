#pragma once

#include <thread>
#include <mutex>
#include <functional>

class Thread
{
public:
	typedef uint64_t ID;
private:
	using CallbackFn = std::function<void(void*)>;
	std::thread thread;
	static uint64_t _ThreadIDHash(const std::thread::id& p_t);

	ID id = _ThreadIDHash(std::thread::id());
	static ID mainThreadID;
	static thread_local ID callerID;

	static void Callback(Thread* p_self, CallbackFn p_callback, void* p_user);
	
public:

	static Thread::ID GetCallerID() { return callerID;  }

	void Start(CallbackFn p_callback, void* p_user);
	void WaitToFinish();

	Thread();
	~Thread();
};


static std::recursive_mutex mutex;

#define THREAD_LOCK mutex.lock();
#define THREAD_UNLOCK mutex.unlock();
#define THREAD_LOCK_GUARD std::lock_guard lock(mutex);
			
