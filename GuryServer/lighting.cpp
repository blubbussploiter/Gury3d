

#include "rbx.h"
#include "lighting.h"
#include "datamodel.h"

RTTR_REGISTRATION
{
    rttr::registration::class_ <RBX::Lighting>("Lighting")
         .constructor<>()
         .property("TimeOfDay", &RBX::Lighting::getTimeOfDay, &RBX::Lighting::setTimeOfDay)(rttr::metadata("Type", RBX::Data))
         .property("BottomAmbientV9", &RBX::Lighting::getBottomAmbient, &RBX::Lighting::setBottomAmbient)(rttr::metadata("Type", RBX::Appearance))
         .property("ClearColor", &RBX::Lighting::getClearColor, &RBX::Lighting::setClearColor)(rttr::metadata("Type", RBX::Appearance))
         .property("SpotLightV9", &RBX::Lighting::getSpotLight, &RBX::Lighting::setSpotLight)(rttr::metadata("Type", RBX::Appearance))
         .property("TopAmbientV9", &RBX::Lighting::getTopAmbient, &RBX::Lighting::setTopAmbient)(rttr::metadata("Type", RBX::Appearance))
         .property("GeographicLatitude",&RBX::Lighting::getGeoLatitude, &RBX::Lighting::setGeoLatitude)(rttr::metadata("Type", RBX::Appearance));
}

RBX::Lighting* RBX::Lighting::singleton()
{
    return RBX::Datamodel::getDatamodel()->lighting;
}

GameTime RBX::Time::fromString(const char* c)
{
    int hour = 0, minute = 0, second = 0;
    AMPM ampm;

    sscanf(c, "%d:%d:%d", &hour, &minute, &second);
    if (hour < 12)
        ampm = AM;
    else
        ampm = PM;

    return toSeconds(hour, minute, second, ampm);
}

std::string RBX::Time::toString(int seconds)
{
    int hour, minute, second;

    hour = seconds / 3600;
    minute = seconds % (hour * 60);
    second = (seconds % (hour * 3600)) - ((seconds % (hour * 60)) * 60);

    return RBX::Format("%d:%d:%d", hour, minute, second);
}
