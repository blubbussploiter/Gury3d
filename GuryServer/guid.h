#pragma once

#include <map>
#include "instance.h"

namespace RBX
{

	class Guid
	{
	public:
		static std::map<Guid*, RBX::Instance*>* registry;
	public:

		class Data
		{
		public:
			std::string scope;
			int index;
		};

		Data data;

		static std::string generateGUID();
		static std::string getLocalScope();

		static void initLocalScope();

		static inline RBX::Instance* fromIndex(int id)
		{
			std::map<Guid*, RBX::Instance*>::iterator it;

			for (it = registry->begin(); it != registry->end(); it++)
			{
				Guid* guid = it->first;
				if (guid->data.index == id)
				{
					return it->second;
				}
			}

			return 0;
		}


		Guid();
	};

	class GuidItem
	{
	public:
		Guid* guid;
		RBX::Instance* item;
	public:
		void registerGuid()
		{
			(*Guid::registry)[guid] = item;
		}
		void unregisterGuid()
		{
			(*Guid::registry)[guid] = 0;
		}
		void assign()
		{
			unregisterGuid();
			item->id = this;
			registerGuid();
		}
		GuidItem(RBX::Instance* item) : item(item)
		{
			guid = new Guid();
			guid->data.scope = Guid::generateGUID();
		}
		GuidItem(RBX::Instance* item, std::string networkId) : item(item)
		{
			guid = new Guid();
			guid->data.scope = networkId;
		}
	};
}