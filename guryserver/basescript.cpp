#include "basescript.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<RBX::BaseScript>("Script")
		.constructor<>()
		.property("Source", &RBX::BaseScript::getSource, &RBX::BaseScript::setSource)(rttr::metadata("Type", RBX::Data));
}