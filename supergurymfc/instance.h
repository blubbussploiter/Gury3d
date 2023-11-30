
#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <vector>

#include "property.h"

namespace RBX
{

	class Instance;
	class GuidItem;

	typedef std::vector<Instance*> Instances;

	class Instance 
	{
	public:

		std::string name;
		std::string className;

		Instance* parent;
		Instances* children;

		bool archivable;

	public:

		GuidItem* id;

		bool isParentLocked;

		bool isAncestorOf(RBX::Instance* i);

		template<typename T>
		inline T* findFirstChildOfClass(std::string name)
		{
			Instance* child;
			for (unsigned int i = 0; i < getChildren()->size(); i++)
			{
				child = getChildren()->at(i);
				if (child && child->getClassName() == name)
					return dynamic_cast<T*>(child);
			}
			return 0;
		}

		void setName(std::string newName) { name = newName; }
		std::string getName() { return name; }

		void setClassName2(std::string newClassName) { }
		void setClassName(std::string newClassName) { className = newClassName; }

		std::string getClassName() { return className; }

		bool getArchivable() { return archivable; }
		void setArchivable(bool narchivable) { archivable = narchivable; }

		virtual Instance* clone() const { return new Instance(*this); }
		void remove();

		virtual void onRemove() {};
		virtual void onStep() {};

		virtual void onDescendentAdded(RBX::Instance* child) 
		{
		};

		virtual void onDescendentRemoved(RBX::Instance* child) {};

		virtual void onChildAdded(RBX::Instance* child) {};

		bool contains(const RBX::Instance* child);

		void signalOnDescendentAdded(RBX::Instance* newParent, RBX::Instance* oldParent);
		void signalOnDescendentRemoved(RBX::Instance* newParent, RBX::Instance* oldParent);

		Instances* getChildren() { return children; }

		void setParent(Instance* instance);
		Instance* getParent();

		Instance* findFirstChild(std::string name);

		Instance() : name("Instance"), className("Instance"), parent(NULL), isParentLocked(false)
		{
			children = new RBX::Instances();
		}

		virtual ~Instance() { }
		RTTR_ENABLE()
	};

	template <class Type>
	bool IsA(RBX::Instance* i)
	{
		return (i && dynamic_cast<Type*>(i) != 0);
	}

}

#endif