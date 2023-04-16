#pragma once

#include <ion/net/NetInternalTypes.h>
#include <ion/net/NetMemory.h>
#include <ion/net/NetPacketPriority.h>
#include <ion/net/NetTypes.h>

#include <ion/concurrency/MPSCQueue.h>
namespace ion
{

namespace detail
{
class NetUpdateWorker;
class NetUpdateJob;
}  // namespace detail

struct NetCommand;

using NetCommandPtr = ArenaPtr<NetCommand, NetInterfaceResource>;

// #TODO: Replace with lock-free object pool
using NetReceiveAllocator = ArenaAllocator<NetSocketReceiveData, NetInterfaceResource>;

struct NetControl
{
	NetControl(NetInterfaceResource* pool)
	  : mMemoryResource(*pool), mReceiveAllocator(pool), mPacketReturnQueue(pool), mBufferedCommands(pool)
	{
	}
	~NetControl()
	{
#if ION_ASSERTS_ENABLED
		ION_ASSERT(mUserPacketCount == 0, "Free user packets");
#endif
	}
	NetInterfaceResource& mMemoryResource;
	NetReceiveAllocator mReceiveAllocator;
#if (ION_ASSERTS_ENABLED == 1)
	std::atomic<uint64_t> mUserPacketCount = 0;
#endif
	MPSCQueueCounted<NetPacket*, NetInterfaceAllocator<NetPacket*>> mPacketReturnQueue;
	MPSCQueue<NetCommandPtr, NetInterfaceAllocator<NetCommandPtr>> mBufferedCommands;
	union Updater
	{
		Updater() : mUpdateWorker(nullptr) {}
		~Updater() {}
		NetInterfacePtr<detail::NetUpdateWorker> mUpdateWorker;
		NetInterfacePtr<detail::NetUpdateJob> mUpdateJob;
	} mUpdater;
	NetPeerUpdateMode mUpdateMode = NetPeerUpdateMode::User;
	std::atomic<bool> mIsReceiving = false;	 // True only when new data incoming from socket
	std::atomic<bool> mIsActive = false;
	TimeMS mLastUpdate;
	uint32_t mResendExtraDelay = 0; // If peer has heavy load use this to avoid resending and make load even worse
};
}  // namespace ion
