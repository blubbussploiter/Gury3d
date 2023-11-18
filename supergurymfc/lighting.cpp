
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

void RBX::Lighting::begin(RenderDevice* device)
{

    LightingParameters lighting;
    Color3 ambientColor, dynamicTop, dynamicBottom;
    
    lighting = getParameters();

    device->enableLighting();
    device->setSpecularCoefficient(1.0f);

    ambientColor = (bottom_ambient + top_ambient) / 2.0f;
    device->setColorClearValue(clear_color);
    
    bool renderDiffuse = ((spotLight_color.r * 255) < 200);

    device->setLight(0, GLight::directional(lighting.lightDirection, spotLight_color * 0.8999999761581421f, 1, renderDiffuse)); /* places with spotlightcolors greater than 220, 220, 220 render SUPER bright (or just weirdly), and places with less than that render SUPER dark. so disable if its not so */

    if (top_ambient != ambientColor && !renderDiffuse)
    {
        Color3 ambient = top_ambient - ambientColor;
        device->setLight(1, GLight::directional(Vector3::unitY(), ambient, 0, 1));
    }

    if (bottom_ambient != ambientColor)
    {
        Color3 ambient = bottom_ambient - ambientColor;
        device->setLight(2, GLight::directional(toLight, ambient, 0, 1));
    }

    device->setAmbientLightColor(ambientColor);
}

void RBX::Lighting::end(RenderDevice* device)
{
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
    minute = seconds % (hour * 60);
    second = (seconds % (hour * 3600)) - ((seconds % (hour * 60)) * 60);

    return RBX::Format("%d:%d:%d", hour, minute, second);
}
