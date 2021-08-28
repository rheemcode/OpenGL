#include <glpch.h>
#include "Thread.h"


uint64_t Thread::_ThreadIDHash(const std::thread::id& p_t)
{
	static std::hash<std::thread::id> hasher;
	return hasher(p_t);
}

Thread::ID Thread::mainThreadID = _ThreadIDHash(std::this_thread::get_id());
thread_local Thread::ID Thread::callerID = 0;



void Thread::Callback(Thread* p_self, CallbackFn p_callback, void* p_user)
{
	Thread::callerID = _ThreadIDHash(p_self->thread.get_id());
	p_callback(p_user);
}

void Thread::Start(CallbackFn p_callback, void* p_user)
{
	if (id != _ThreadIDHash(std::thread::id()))
	{
		thread.detach();
		std::thread emptyThread;
		thread.swap(emptyThread);
	}

	std::thread newThread(&Thread::Callback, this, p_callback, p_user);
	thread.swap(newThread);
	id = _ThreadIDHash(thread.get_id());
}

void Thread::WaitToFinish()
{
	if (id != _ThreadIDHash(std::thread::id()))
	{
		thread.join();
		std::thread emptyThread;
		thread.swap(emptyThread);
		id = _ThreadIDHash(std::thread::id());
	}
}

Thread::Thread()
{
	if (id != _ThreadIDHash(std::thread::id()))
	{
		thread.detach();
	}
}

Thread::~Thread()
{

}