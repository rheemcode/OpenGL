#include <glpch.h>
#include "CommandBuffer.h"

CommandBuffer::SyncSemaphore* CommandBuffer::_AllocSyncSem()
{
	int idx = -1;

	while (true)
	{
		mutex.lock();
		for (int i = 0; i < SYNC_SEMAPHORES; i++)
		{
			if (!syncSems[i].inUse)
			{
				syncSems[i].inUse = true;
				idx = i;
				break;
			}
		}

		mutex.unlock();

		if (idx == -1)
		{
			Sleep(1);
		}

		else
			break;
	}
	return &syncSems[idx];
}

bool CommandBuffer::DeallocOne()
{
tryagain:
	if (deallocPtr == (writePtrAndEpoch >> 1))
	{
		return false;
	}

	uint32_t size = *(uint32_t*)&commandMem[deallocPtr];

	if (size == 0)
	{
		deallocPtr = 0;
		goto tryagain;
	}

	if (size & 1)
		return false;

	deallocPtr += (size >> 1) + 8;
	return true;
}

CommandBuffer::CommandBuffer(bool p_sync)
{
	commandMemSize = DEFAULT_COMMAND_MEM_SIZE_KB;
	commandMemSize *= 1024;
	commandMem = (uint8_t*)malloc(commandMemSize);
	if (p_sync)
		sync = new Semaphore();
}

CommandBuffer::~CommandBuffer()
{
	if (sync)
	{
		delete sync;
	}

	free(commandMem);
}