#include "decal.h"
#include "pvinstance.h"

//Reflection::PropertyDescriptor<RBX::Decal, RBX::Content> RBX::Decal::prop_texture("Texture", Reflection::TYPE_Content, &RBX::Decal::getTextureContent, &RBX::Decal::setTextureContent, RBX::Decal::properties);
//Reflection::PropertyDescriptor<RBX::Decal, RBX::NormalId> RBX::Decal::prop_face("Face", Reflection::TYPE_Number, &RBX::Decal::getFace, &RBX::Decal::setFace, RBX::Decal::properties);

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
    std::string contentPath;

    RBX::ContentProvider::singleton()->downloadContent(c, contentPath);
    if (contentPath.empty()) return;

    texture = Texture::fromFile(contentPath);

    RBX::ContentProvider::singleton()->cleanupContent(c);
}

void RBX::Decal::fromFile(std::string file, Texture::WrapMode wrap, Texture::InterpolateMode interpolate)
{
    filePath = file;
    wrapMode = wrap;
    interpolateMode = interpolate;
}

void RBX::Decal::render(RenderDevice* rd, RBX::PVInstance* p)
{

    if (texture.isNull())
    {
        texture = Texture::fromFile(filePath, TextureFormat::AUTO, wrapMode, interpolateMode);
    }

    Render::rawDecal(rd, p, face, getGLid(), sfactor, dfactor);

}

void RBX::Render::rawDecal(RenderDevice* d, RBX::PVInstance* pv, NormalId face, int texture, int sfactor, int dfactor)
{

    bindDecal(texture, sfactor, dfactor);
    pv->renderFace(d, face);
    unbindDecal(sfactor);
}