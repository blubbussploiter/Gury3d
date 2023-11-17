#ifndef ENUM_H
#define ENUM_H

#include <map>

namespace RBX
{
	template <typename E>
	class EnumDescriptor
	{
	private:
		static std::map<E, const char*> enumItems;
	public:
		static void addPair(E e, const char* name)
		{
			enumItems[e] = name;
		}
		static E fromName(const char* name)
		{
			return enumItems[name];
		}
		static const char* fromEnum(E index)
		{
			return enumItems[index];
		}
	};
}

#endif