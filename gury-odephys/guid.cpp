#include <Windows.h>
#include "guid.h"

std::string localScope;

int nextIndex = 0;
bool flag;

std::map<RBX::Guid*, RBX::Instance*>* RBX::Guid::registry = new std::map<RBX::Guid*, RBX::Instance*>();

std::string RBX::Guid::generateGUID()
{
	_GUID guid; // [esp+0h] [ebp-D4h] BYREF
	char ansiClsid[64]; // [esp+10h] [ebp-C4h] BYREF
	wchar_t wszGUID_4[64]; // [esp+54h] [ebp-80h] OVERLAPPED BYREF
	std::string result;

	result = "GRY";
	CoCreateGuid(&guid);
	memset(wszGUID_4, 0, sizeof(wszGUID_4));
	StringFromGUID2(guid, wszGUID_4, 64);
	WideCharToMultiByte(0, 0, wszGUID_4, 64, ansiClsid, 64, 0, 0);
	result += ansiClsid;
	result.erase(40, 1);
	result.erase(27, 1);
	result.erase(22, 1);
	result.erase(17, 1);
	result.erase(12, 1);
	result.erase(3, 1);
	return result;
}

std::string RBX::Guid::getLocalScope()
{
	if (!flag) /* no boost.. */
	{
		initLocalScope();
		flag = 1;
	}
	return localScope;
}

void RBX::Guid::initLocalScope()
{
	localScope = generateGUID();
}

RBX::Guid::Guid()
{
	data.scope = getLocalScope();
	data.index = nextIndex++;
}