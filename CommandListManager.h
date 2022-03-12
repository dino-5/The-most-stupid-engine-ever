#ifndef COMMAND_LIST_MANAGER_H
#define COMMAND_LIST_MANAGER_H
#include"commin.h"
#include"CommandList.h"
#include"CommandAllocatorPool.h"
#include<vector>
template<typename T>
class CommandListManager;

using GraphicsCommandListManager = CommandListManager<ID3D12GraphicsCommandList>;

template<typename T>
class CommandListManager
{
private:
	std::vector<CommandList<T>> m_lists;
public:
	CommandListManager()=default;
	CommandListManager(UINT countListPerThread);
	CommandList<T>& GetList(ListId id);
};

#endif
