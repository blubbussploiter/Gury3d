#include "strings.h"

#include "instance.h"
#include "sounds.h"

#include "rbx.h"

static RBX::Sound* pageTurn = RBX::Sound::fromFile(GetFileInPath("/content/sounds/pageturn.wav"));

RTTR_REGISTRATION
{
	rttr::registration::class_<RBX::Instance>("Instance")
		 .constructor<>()
		 .property("Name", &RBX::Instance::getName,
			&RBX::Instance::setName)(rttr::metadata("Type", RBX::Data))
		 .property_readonly("className", &RBX::Instance::getClassName)(rttr::metadata("Type", RBX::Data))
		 .property("archivable", &RBX::Instance::getArchivable, &RBX::Instance::setArchivable)(rttr::metadata("Type", RBX::Behavior))
		 .property("Parent", &RBX::Instance::getParent,
			 &RBX::Instance::setParent)
		.method("children", &RBX::Instance::getChildren)
		.method("findFirstChild", &RBX::Instance::findFirstChild);
}

bool RBX::Instance::isAncestorOf(RBX::Instance* descendant)
{
	const RBX::Instance* v2; // eax

	v2 = descendant;
	if (!descendant)
		return 0;
	while (1)
	{
		v2 = v2->parent;
		if (v2 == this)
			break;
		if (!v2)
			return 0;
	}
	return 1;
}

bool RBX::Instance::contains(const RBX::Instance* child)
{
	const RBX::Instance* v2; // eax

	v2 = child;
	if (!child)
		return 0;
	while (v2 != this)
	{
		v2 = v2->parent;
		if (!v2)
			return 0;
	}
	return 1;
}

void RBX::Instance::signalOnDescendentAdded(RBX::Instance* beginParent, RBX::Instance* child)
{
	RBX::Instance* i;
	RBX::Instances* c;

	c = child->getChildren();

	for (i = beginParent; i; i = i->parent)
	{
		i->onDescendentAdded(child);
	}

	for (unsigned int i = 0; i < c->size(); i++)
	{
		RBX::Instance* in = c->at(i);
		child->signalOnDescendentAdded(child, in);
	}
}

void RBX::Instance::signalOnDescendentRemoved(RBX::Instance* newParent, RBX::Instance* child)
{
	RBX::Instance* i;
	RBX::Instances* c;

	c = child->getChildren();

	for (i = child; i; i = i->parent)
	{
		i->onDescendentRemoved(child);
	}

	for (unsigned int i = 0; i < c->size(); i++)
	{
		RBX::Instance* in = c->at(i);
		child->signalOnDescendentRemoved(child, in);
	}

}

void RBX::Instance::setParent(Instance* instance)
{
	RBX::Instance* oldParent;
	oldParent = parent;

	if (isParentLocked)
		return;

	if (instance == this)
		throw std::runtime_error(RBX::Format("Attempt to set %s as its own parent", getName().c_str()));

	if (oldParent)
	{
		if (std::find(parent->getChildren()->begin(), parent->getChildren()->end(), this) != parent->getChildren()->end())
		{
			auto l = std::remove(parent->getChildren()->begin(), parent->getChildren()->end(), this);
			parent->signalOnDescendentRemoved(parent, this);
			parent->getChildren()->erase(l);

		}
	}
	parent = instance;

	if (parent)
	{
		parent->children->push_back(this);
		parent->onChildAdded(this);

		if (!parent->contains(oldParent))
			parent->signalOnDescendentAdded(parent, this);
	}
}

void RBX::Instance::remove()
{
	if (isParentLocked) return;

	for (RBX::Instance* i : *children)
	{
		i->remove();
	}

	setParent(0);
	pageTurn->play();

	onRemove();
}

RBX::Instance* RBX::Instance::getParent()
{
	return parent;
}

RBX::Instance* RBX::Instance::findFirstChild(std::string name)
{
	Instance* child;
	RBX::Instances* children = getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		child = children->at(i);
		if (child && child->getName() == name)
			return child;
	}
	return 0;
}