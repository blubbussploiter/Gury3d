
#include "rbx.h"
#include "lighting.h"
#include "datamodel.h"

RTTR_REGISTRATION
{
    rttr::registration::class_ <RBX::Lighting>("Lighting")
         .constructor<>()
         .property("SpotLightV9", &RBX::Lighting::getSpotLight, &RBX::Lighting::setSpotLight)
         .property("TopAmbientV9", &RBX::Lighting::getTopAmbient, &RBX::Lighting::setTopAmbient)
         .property("BottomAmbientV9", &RBX::Lighting::getBottomAmbient, &RBX::Lighting::setBottomAmbient)
         .property("TimeOfDay", &RBX::Lighting::getTimeOfDay, &RBX::Lighting::setTimeOfDay)
         .property("GeographicLatitude",&RBX::Lighting::getGeoLatitude, &RBX::Lighting::setGeoLatitude);
}

RBX::Lighting* RBX::Lighting::singleton()
{
    return RBX::Datamodel::getDatamodel()->lighting;
}

void RBX::Lighting::begin(RenderDevice* device)
{

    LightingParameters lighting;
    Color3 ambientColor, dynamicTop, dynamicBottom;
    
    int num = 0;

    lighting = getParameters();

    device->enableLighting();

    device->setSpecularCoefficient(0);
    device->setSpecularCoefficient(Color3::white());
    device->setShininess(50.0f);

    ambientColor = (bottom_ambient + top_ambient) / 2.0f;

    dynamicTop = top_ambient - ambientColor;
    dynamicBottom = bottom_ambient - ambientColor;

    device->setColorClearValue(clear_color);
    device->setAmbientLightColor(ambientColor);

    device->setLight(0, GLight::directional(lighting.lightDirection, spotLight_color * 0.8999999761581421f, 1, 0));
    device->setLight(1, GLight::directional(toLight, dynamicBottom, 0, 1));
    device->setLight(2, GLight::directional(lightDir2, dynamicTop, 0, 1));
    
}

void RBX::Lighting::end(RenderDevice* device)
{
    device->setLight(0, 0);
    device->setLight(1, 0);
    device->setLight(2, 0);

    device->disableLighting();
}

LightingParameters RBX::Lighting::getParameters()
{
    if (!params)
    {
        params = new LightingParameters();

        params->setLatitude(latitude);
        params->setTime(timeOfDay);

    }

    return *params;
}

GameTime RBX::Time::fromString(const char* c)
{
    int hour = 0, minute = 0, second = 0;

    sscanf(c, "%d:%d:%d", &hour, &minute, &second);

    return toSeconds(hour, minute, second, PM);
}

std::string RBX::Time::toString(int seconds)
{
    int hour, minute, second;

    hour = seconds / 3600;
    minute = (seconds % 3600) / 60;
    second = seconds % 60;

    printf("hi\n");

    return RBX::Format("%d:%d:%d", hour, minute, second);
}
