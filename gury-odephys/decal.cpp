#include "decal.h"
#include "pvinstance.h"

//Reflection::PropertyDescriptor<RBX::Decal, RBX::Content> RBX::Decal::prop_texture("Texture", Reflection::TYPE_Content, &RBX::Decal::getTextureContent, &RBX::Decal::setTextureContent, RBX::Decal::properties);
//Reflection::PropertyDescriptor<RBX::Decal, RBX::NormalId> RBX::Decal::prop_face("Face", Reflection::TYPE_Number, &RBX::Decal::getFace, &RBX::Decal::setFace, RBX::Decal::properties);

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::Decal>("Decal")
         .constructor<>()
         .property("Face", &RBX::Decal::getFace, &RBX::Decal::setFace)(rttr::metadata("Type", RBX::Data))
         .property("Texture", &RBX::Decal::getTextureContent, &RBX::Decal::setTextureContent)(rttr::metadata("Type", RBX::Appearance));
}

void bindDecal(int glid, int sfactor, int dfactor)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(sfactor, dfactor);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, glid);
}

void unbindDecal(int sfactor)
{
    glBlendFunc(sfactor, GL_ZERO);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void RBX::Decal::setTextureContent(Content c)
{
    tContent = c;
}

void RBX::Decal::fromFile(std::string file, Texture::WrapMode wrap, Texture::InterpolateMode interpolate)
{
    tContent = Content::fromImageFile(file);
    wrapMode = wrap;
    interpolateMode = interpolate;
}

void RBX::Decal::render(RenderDevice* rd, RBX::PVInstance* p)
{

    if (texture.isNull()
        && !tContent.isEmpty)
    {
        setTextureContent(tContent);
    }

    Render::rawDecal(rd, p, face, getGLid(), sfactor, dfactor);

}

void RBX::Render::rawDecal(RenderDevice* d, RBX::PVInstance* pv, NormalId face, int texture, int sfactor, int dfactor)
{
    bindDecal(texture, sfactor, dfactor);
    pv->renderFace(d, face);
    unbindDecal(sfactor);
}