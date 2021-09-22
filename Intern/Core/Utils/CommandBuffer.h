#pragma once
#include <Windows.h>
#include <mutex>

// Godot


template <class T>
struct GetSimpleTypeT {
	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T&> {
	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T const> {
	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T const&> {
	typedef T type_t;
};


#define COMMA(N) _COMMA_##N
#define _COMMA_0
#define _COMMA_1 ,
#define _COMMA_2 ,
#define _COMMA_3 ,
#define _COMMA_4 ,
#define _COMMA_5 ,
#define _COMMA_6 ,
#define _COMMA_7 ,
#define _COMMA_8 ,
#define _COMMA_9 ,
#define _COMMA_10 ,
#define _COMMA_11 ,
#define _COMMA_12 ,
#define _COMMA_13 ,
#define _COMMA_14 ,
#define _COMMA_15 ,


#define COMMA_SEP_LIST(ITEM, LENGTH) _COMMA_SEP_LIST_##LENGTH(ITEM)
#define _COMMA_SEP_LIST_15(ITEM) \
	_COMMA_SEP_LIST_14(ITEM)     \
	, ITEM(15)
#define _COMMA_SEP_LIST_14(ITEM) \
	_COMMA_SEP_LIST_13(ITEM)     \
	, ITEM(14)
#define _COMMA_SEP_LIST_13(ITEM) \
	_COMMA_SEP_LIST_12(ITEM)     \
	, ITEM(13)
#define _COMMA_SEP_LIST_12(ITEM) \
	_COMMA_SEP_LIST_11(ITEM)     \
	, ITEM(12)
#define _COMMA_SEP_LIST_11(ITEM) \
	_COMMA_SEP_LIST_10(ITEM)     \
	, ITEM(11)
#define _COMMA_SEP_LIST_10(ITEM) \
	_COMMA_SEP_LIST_9(ITEM)      \
	, ITEM(10)
#define _COMMA_SEP_LIST_9(ITEM) \
	_COMMA_SEP_LIST_8(ITEM)     \
	, ITEM(9)
#define _COMMA_SEP_LIST_8(ITEM) \
	_COMMA_SEP_LIST_7(ITEM)     \
	, ITEM(8)
#define _COMMA_SEP_LIST_7(ITEM) \
	_COMMA_SEP_LIST_6(ITEM)     \
	, ITEM(7)
#define _COMMA_SEP_LIST_6(ITEM) \
	_COMMA_SEP_LIST_5(ITEM)     \
	, ITEM(6)
#define _COMMA_SEP_LIST_5(ITEM) \
	_COMMA_SEP_LIST_4(ITEM)     \
	, ITEM(5)
#define _COMMA_SEP_LIST_4(ITEM) \
	_COMMA_SEP_LIST_3(ITEM)     \
	, ITEM(4)
#define _COMMA_SEP_LIST_3(ITEM) \
	_COMMA_SEP_LIST_2(ITEM)     \
	, ITEM(3)
#define _COMMA_SEP_LIST_2(ITEM) \
	_COMMA_SEP_LIST_1(ITEM)     \
	, ITEM(2)
#define _COMMA_SEP_LIST_1(ITEM) \
	_COMMA_SEP_LIST_0(ITEM)     \
	ITEM(1)
#define _COMMA_SEP_LIST_0(ITEM)

#define SEMIC_SEP_LIST(ITEM, LENGTH) _SEMIC_SEP_LIST_##LENGTH(ITEM)
#define _SEMIC_SEP_LIST_15(ITEM) \
	_SEMIC_SEP_LIST_14(ITEM);    \
	ITEM(15)
#define _SEMIC_SEP_LIST_14(ITEM) \
	_SEMIC_SEP_LIST_13(ITEM);    \
	ITEM(14)
#define _SEMIC_SEP_LIST_13(ITEM) \
	_SEMIC_SEP_LIST_12(ITEM);    \
	ITEM(13)
#define _SEMIC_SEP_LIST_12(ITEM) \
	_SEMIC_SEP_LIST_11(ITEM);    \
	ITEM(12)
#define _SEMIC_SEP_LIST_11(ITEM) \
	_SEMIC_SEP_LIST_10(ITEM);    \
	ITEM(11)
#define _SEMIC_SEP_LIST_10(ITEM) \
	_SEMIC_SEP_LIST_9(ITEM);     \
	ITEM(10)
#define _SEMIC_SEP_LIST_9(ITEM) \
	_SEMIC_SEP_LIST_8(ITEM);    \
	ITEM(9)
#define _SEMIC_SEP_LIST_8(ITEM) \
	_SEMIC_SEP_LIST_7(ITEM);    \
	ITEM(8)
#define _SEMIC_SEP_LIST_7(ITEM) \
	_SEMIC_SEP_LIST_6(ITEM);    \
	ITEM(7)
#define _SEMIC_SEP_LIST_6(ITEM) \
	_SEMIC_SEP_LIST_5(ITEM);    \
	ITEM(6)
#define _SEMIC_SEP_LIST_5(ITEM) \
	_SEMIC_SEP_LIST_4(ITEM);    \
	ITEM(5)
#define _SEMIC_SEP_LIST_4(ITEM) \
	_SEMIC_SEP_LIST_3(ITEM);    \
	ITEM(4)
#define _SEMIC_SEP_LIST_3(ITEM) \
	_SEMIC_SEP_LIST_2(ITEM);    \
	ITEM(3)
#define _SEMIC_SEP_LIST_2(ITEM) \
	_SEMIC_SEP_LIST_1(ITEM);    \
	ITEM(2)
#define _SEMIC_SEP_LIST_1(ITEM) \
	_SEMIC_SEP_LIST_0(ITEM)     \
	ITEM(1)
#define _SEMIC_SEP_LIST_0(ITEM)

#define SPACE_SEP_LIST(ITEM, LENGTH) _SPACE_SEP_LIST_##LENGTH(ITEM)
#define _SPACE_SEP_LIST_15(ITEM) \
	_SPACE_SEP_LIST_14(ITEM)     \
	ITEM(15)
#define _SPACE_SEP_LIST_14(ITEM) \
	_SPACE_SEP_LIST_13(ITEM)     \
	ITEM(14)
#define _SPACE_SEP_LIST_13(ITEM) \
	_SPACE_SEP_LIST_12(ITEM)     \
	ITEM(13)
#define _SPACE_SEP_LIST_12(ITEM) \
	_SPACE_SEP_LIST_11(ITEM)     \
	ITEM(12)
#define _SPACE_SEP_LIST_11(ITEM) \
	_SPACE_SEP_LIST_10(ITEM)     \
	ITEM(11)
#define _SPACE_SEP_LIST_10(ITEM) \
	_SPACE_SEP_LIST_9(ITEM)      \
	ITEM(10)
#define _SPACE_SEP_LIST_9(ITEM) \
	_SPACE_SEP_LIST_8(ITEM)     \
	ITEM(9)
#define _SPACE_SEP_LIST_8(ITEM) \
	_SPACE_SEP_LIST_7(ITEM)     \
	ITEM(8)
#define _SPACE_SEP_LIST_7(ITEM) \
	_SPACE_SEP_LIST_6(ITEM)     \
	ITEM(7)
#define _SPACE_SEP_LIST_6(ITEM) \
	_SPACE_SEP_LIST_5(ITEM)     \
	ITEM(6)
#define _SPACE_SEP_LIST_5(ITEM) \
	_SPACE_SEP_LIST_4(ITEM)     \
	ITEM(5)
#define _SPACE_SEP_LIST_4(ITEM) \
	_SPACE_SEP_LIST_3(ITEM)     \
	ITEM(4)
#define _SPACE_SEP_LIST_3(ITEM) \
	_SPACE_SEP_LIST_2(ITEM)     \
	ITEM(3)
#define _SPACE_SEP_LIST_2(ITEM) \
	_SPACE_SEP_LIST_1(ITEM)     \
	ITEM(2)
#define _SPACE_SEP_LIST_1(ITEM) \
	_SPACE_SEP_LIST_0(ITEM)     \
	ITEM(1)
#define _SPACE_SEP_LIST_0(ITEM)

#define ARG(N) p##N
#define PARAM(N) P##N p##N
#define TYPE_PARAM(N) class P##N
#define PARAM_DECL(N) typename GetSimpleTypeT<P##N>::type_t p##N

#define DECL_CMD(N)                                                    \
	template <class T, class M COMMA(N) COMMA_SEP_LIST(TYPE_PARAM, N)> \
	struct Command##N : public Command {							   \
		T *instance;                                                   \
		M method;                                                      \
		SEMIC_SEP_LIST(PARAM_DECL, N);                                 \
		virtual void Call() {                                          \
			(instance->*method)(COMMA_SEP_LIST(ARG, N));               \
		}                                                              \
	};
#define DECL_CMD_RET(N)                                                         \
	template <class T, class M, COMMA_SEP_LIST(TYPE_PARAM, N) COMMA(N) class R> \
	struct CommandRet##N : public SyncCommand {                                 \
		R *ret;                                                                 \
		T *instance;                                                            \
		M method;                                                               \
		SEMIC_SEP_LIST(PARAM_DECL, N);                                          \
		virtual void Call() {                                                   \
			*ret = (instance->*method)(COMMA_SEP_LIST(ARG, N));                 \
		}                                                                       \
	};
#define DECL_CMD_SYNC(N)                                               \
	template <class T, class M COMMA(N) COMMA_SEP_LIST(TYPE_PARAM, N)> \
	struct CommandSync##N : public SyncCommand {                       \
		T *instance;                                                   \
		M method;                                                      \
		SEMIC_SEP_LIST(PARAM_DECL, N);                                 \
		virtual void Call() {                                          \
			(instance->*method)(COMMA_SEP_LIST(ARG, N));               \
		}                                                              \
	};
#define TYPE_ARG(N) P##N
#define CMD_TYPE(N) Command##N<T, M COMMA(N) COMMA_SEP_LIST(TYPE_ARG, N)>
#define CMD_ASSIGN_PARAM(N) cmd->p##N = p##N

#define DECL_PUSH(N)                                                         \
	template <class T, class M COMMA(N) COMMA_SEP_LIST(TYPE_PARAM, N)>       \
	void Push(T *p_instance, M p_method COMMA(N) COMMA_SEP_LIST(PARAM, N)) { \
		CMD_TYPE(N) *cmd = AllocateAndLock<CMD_TYPE(N)>();                 \
		cmd->instance = p_instance;                                          \
		cmd->method = p_method;                                              \
		SEMIC_SEP_LIST(CMD_ASSIGN_PARAM, N);                                 \
		mutex.unlock();                                                            \
		if (sync)                                                            \
			sync->Post();                                                    \
	}

#define CMD_RET_TYPE(N) CommandRet##N<T, M, COMMA_SEP_LIST(TYPE_ARG, N) COMMA(N) R>

#define DECL_PUSH_AND_RET(N)                                                                   \
	template <class T, class M, COMMA_SEP_LIST(TYPE_PARAM, N) COMMA(N) class R>                \
	void PushAndRet(T *p_instance, M p_method, COMMA_SEP_LIST(PARAM, N) COMMA(N) R *r_ret) { \
		SyncSemaphore *ss = _AllocSyncSem();                                                 \
		CMD_RET_TYPE(N) *cmd = AllocateAndLock<CMD_RET_TYPE(N)>();                           \
		cmd->instance = p_instance;                                                            \
		cmd->method = p_method;                                                                \
		SEMIC_SEP_LIST(CMD_ASSIGN_PARAM, N);                                                   \
		cmd->ret = r_ret;                                                                      \
		cmd->syncSem = ss;                                                                    \
		mutex.unlock();                                                                              \
		if (sync)                                                                              \
			sync->Post();                                                                      \
		ss->sem.Wait();                                                                        \
		ss->inUse = false;                                                                    \
	}

#define CMD_SYNC_TYPE(N) CommandSync##N<T, M COMMA(N) COMMA_SEP_LIST(TYPE_ARG, N)>

#define DECL_PUSH_AND_SYNC(N)                                                         \
	template <class T, class M COMMA(N) COMMA_SEP_LIST(TYPE_PARAM, N)>                \
	void PushAndSync(T *p_instance, M p_method COMMA(N) COMMA_SEP_LIST(PARAM, N)) { \
		SyncSemaphore *ss = _AllocSyncSem();                                        \
		CMD_SYNC_TYPE(N) *cmd = AllocateAndLock<CMD_SYNC_TYPE(N)>();                \
		cmd->instance = p_instance;                                                   \
		cmd->method = p_method;                                                       \
		SEMIC_SEP_LIST(CMD_ASSIGN_PARAM, N);                                          \
		cmd->syncSem = ss;                                                           \
		mutex.unlock();                                                                     \
		if (sync)                                                                     \
			sync->Post();                                                             \
		ss->sem.Wait();                                                               \
		ss->inUse = false;                                                           \
	}

#define MAX_CMD_PARAMS 15

class Semaphore
{
	mutable std::mutex mutex;
	mutable std::condition_variable condVar;
	mutable unsigned long count = 0;
public:

	
	void Post() const
	{
		std::lock_guard<std::mutex> lock(mutex);
		count++;
		condVar.notify_one();
	}

	void Wait() const
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (!count)
			condVar.wait(lock);

		--count;
	}

	bool TryWait() const
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (count)
		{
			--count;
			return true;
		}

		return false;
	}
};

class CommandBuffer
{
	struct SyncSemaphore
	{
		Semaphore sem;
		bool inUse = false;
	};

	struct Command
	{
		virtual void Call() = 0;
		virtual void Post() {}
		virtual ~Command(){}
	};

	struct SyncCommand : public Command
	{
		SyncSemaphore* syncSem;
		virtual void Post()
		{
			syncSem->sem.Post();
		}
	};

	DECL_CMD(0)
	SPACE_SEP_LIST(DECL_CMD, 15)

	DECL_CMD_RET(0)
	SPACE_SEP_LIST(DECL_CMD_RET, 15)

	DECL_CMD_SYNC(0)
	SPACE_SEP_LIST(DECL_CMD_SYNC, 15)
	

	enum {
		DEFAULT_COMMAND_MEM_SIZE_KB = 256,
		SYNC_SEMAPHORES = 8
	};

	uint8_t* commandMem = nullptr;
	uint32_t readPtrAndEpoch = 0;
	uint32_t writePtrAndEpoch = 0;
	uint32_t deallocPtr = 0;
	uint32_t commandMemSize = 0;
	SyncSemaphore syncSems[SYNC_SEMAPHORES];
	std::recursive_mutex mutex;
	Semaphore* sync = nullptr;

	template <class T>
	T* Allocate()
	{
		uint32_t allocSize = ((sizeof(T) + 8 - 1) & ~(8 - 1)) + 8;
		if (allocSize * 2 + sizeof(uint32_t) > commandMemSize)
			return nullptr;

	tryagain:
		uint32_t writePtr = writePtrAndEpoch >> 1;
		if (writePtr < deallocPtr)
		{
			if ((deallocPtr - writePtr) <= allocSize)
			{
				if (DeallocOne())
					goto tryagain;

				return nullptr;
			}
		}
		else
		{
			if ((commandMemSize - writePtr) < allocSize + sizeof(uint32_t))
			{
				if (deallocPtr == 0)
				{
					if (DeallocOne())
						goto tryagain;
					return nullptr;
				}


				if ((commandMemSize - writePtr) < 8)
					return nullptr;

				uint32_t* p = (uint32_t*)&commandMem[writePtr];
				*p = 1;
				writePtrAndEpoch = 0 | (1 & ~writePtrAndEpoch);
				if (sync)
				{
					sync->Post();
				}

				goto tryagain;
			}
		}

		uint32_t size = (sizeof(T) + 8 - 1) & ~(8 - 1);
		uint32_t* p = (uint32_t*)&commandMem[writePtr];
		*p = (size << 1) | 1;
		writePtr += 8;
		T* cmd = new(&commandMem[writePtr]) T;
		writePtr += size;
		writePtrAndEpoch = (writePtr << 1) | (writePtrAndEpoch & 1);
		return cmd;
	}

	template <class T>
	T* AllocateAndLock()
	{
		mutex.lock();
		T* ret;

		while ((ret = Allocate<T>()) == nullptr)
		{
			mutex.unlock();
			Sleep(1000 / 1000);
			mutex.lock();
		}

		return ret;
	}

	bool FlushOne(bool p_lock = true)
	{
		if (p_lock)
			mutex.lock();
	tryagain:
		if (readPtrAndEpoch == writePtrAndEpoch)
		{
			if (p_lock)
				mutex.unlock();
			return false;
		}

		uint32_t readPtr = readPtrAndEpoch >> 1;
		uint32_t sizePtr = readPtr;
		uint32_t size = *(uint32_t*)&commandMem[readPtr] >> 1;

		if (size == 0)
		{
			*(uint32_t*)&commandMem[readPtr] = 0;
			readPtrAndEpoch = 0 | (1  & ~readPtrAndEpoch);
			goto tryagain;
		}

		readPtr += 8;

		Command* cmd = reinterpret_cast<Command*>(&commandMem[readPtr]);
		readPtr += size;

		readPtrAndEpoch = (readPtr << 1) | (readPtrAndEpoch & 1);

		if (p_lock)
			mutex.unlock();

		cmd->Call();
		if (p_lock)
			mutex.lock();

		cmd->Post();
		cmd->~Command();
		*(uint32_t*)&commandMem[sizePtr] &= ~1;

		if (p_lock)
			mutex.unlock();

		return true;
	}

	SyncSemaphore* _AllocSyncSem();
	bool DeallocOne();
public:

	DECL_PUSH(0)
	SPACE_SEP_LIST(DECL_PUSH, 15)

	/* PUSH AND RET COMMANDS */
	DECL_PUSH_AND_RET(0)
	SPACE_SEP_LIST(DECL_PUSH_AND_RET, 15)

	/* PUSH AND RET SYNC COMMANDS*/
	DECL_PUSH_AND_SYNC(0)
	SPACE_SEP_LIST(DECL_PUSH_AND_SYNC, 15)

	void WaitAndFlushOne()
	{
		sync->Wait();
		FlushOne();
	}

	void FlushIfPending()
	{
		if (readPtrAndEpoch != writePtrAndEpoch)
			FlushAll();
	}

	void FlushAll()
	{
		mutex.lock();
		while(FlushOne(false))
		{}
		mutex.unlock();
	}

	CommandBuffer(bool p_sync);
	~CommandBuffer();
};

#undef ARG
#undef PARAM
#undef TYPE_PARAM
#undef PARAM_DECL
#undef DECL_CMD
#undef DECL_CMD_RET
#undef DECL_CMD_SYNC
#undef TYPE_ARG
#undef CMD_TYPE
#undef CMD_ASSIGN_PARAM
#undef DECL_PUSH
#undef CMD_RET_TYPE
#undef DECL_PUSH_AND_RET
#undef CMD_SYNC_TYPE
#undef DECL_CMD_SYNC