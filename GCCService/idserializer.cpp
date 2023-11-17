#include "idserializer.h"
#include "bitstreamHelp.h"
#include "guid.h"

void RBX::Network::IdSerializer::serializeId(BitStream& stream, RBX::Instance* instance)
{
	GuidItem* item;

	std::string scope;
	std::string className;

	if (instance)
	{
		item = instance->id;

		if (!item)
		{
			item = new GuidItem(instance);
			item->assign();
		}

		className = instance->getClassName();
		scope = item->guid->data.scope;

		stream << scope.size();
		stream << scope.c_str();
		stream << className.size();
		stream << className.c_str();
	}

}

RBX::Network::IdSerializer::NetworkId RBX::Network::IdSerializer::deserializeId(BitStream& stream)
{
	NetworkId id;
	size_t sco, sz;

	stream >> sco;
	id.guid = Help::read(stream, sco); /* read guid */
	stream >> sz;
	id.className = Help::read(stream, sz);

	return id;
}
