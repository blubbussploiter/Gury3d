

#include "rbx.h"
#include "lighting.h"
#include "datamodel.h"
#include "view.h"

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

RBX::Lighting* RBX::Lighting::get()
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

RBX::Lighting::Lighting()
{
    RBX::View* view;
    view = RBX::View::get();

    lighting = view->lighting;
    params = &view->params;

    setGeoLatitude(41.7f);
    setTimeOfDay("14:0:0");
    setSpotLight(Color3(0.59607846f, 0.53725493f, 0.40000001f));

    clear_color = Color4(0.0f, 0.0f, 0.0f);
    setTopAmbient(Color3(0.81960785f, 0.81568629f, 0.8509804f));
    setBottomAmbient(Color3(0.47843137f, 0.52549022f, 0.47058824f));

    setName("Lighting");
    setClassName("Lighting");
}