#include "decal.h"
#include "pvinstance.h"

#include "stdout.h"

//Reflection::PropertyDescriptor<RBX::Decal, RBX::Content> RBX::Decal::prop_texture("Texture", Reflection::TYPE_Content, &RBX::Decal::getTextureContent, &RBX::Decal::setTextureContent, RBX::Decal::properties);
//Reflection::PropertyDescriptor<RBX::Decal, RBX::NormalId> RBX::Decal::prop_face("Face", Reflection::TYPE_Number, &RBX::Decal::getFace, &RBX::Decal::setFace, RBX::Decal::properties);

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::Decal>("Decal")
         .constructor<>()
         .property("Face", &RBX::Decal::getFace, &RBX::Decal::setFace)(rttr::metadata("Type", RBX::Data))
         .property("Texture", &RBX::Decal::getTextureContent, &RBX::Decal::setTextureContent)(rttr::metadata("Type", RBX::Appearance));
}

void RBX::Decal::setTextureContent(Content c)
{
    tContent = c;
}

void RBX::Decal::initContentTexture()
{
    if (!tContent.isEmpty)
    {
        if (tContent.resolve())
        {
            GImage image(tContent.content, tContent.contentLength);

            if (image.channels > 0)
            {
                Texture::Parameters params;

                params.wrapMode = Texture::WrapMode::CLAMP;
                params.interpolateMode = Texture::InterpolateMode::NEAREST_MIPMAP;

                texture = Texture::fromGImage(getName(), image, TextureFormat::RGBA8, Texture::DIM_2D, params);
            }
        }
    }
}

void RBX::Decal::fromFile(std::string file, Texture::WrapMode wrap, Texture::InterpolateMode interpolate)
{
    wrapMode = wrap;
    interpolateMode = interpolate;
    tContent = Content::fromImageFile(file);
}

void RBX::Decal::render(RenderDevice* rd, RBX::Render::Renderable* p)
{
    if (texture.isNull())
    {
        initContentTexture();
    }
    else
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Render::rawDecal(rd, p, face, getGLid(), 0);
        glBlendFunc(GL_ZERO, GL_ZERO);
        glDisable(GL_BLEND);
    }
}

void RBX::Render::rawDecal(RenderDevice* d, RBX::Render::Renderable* pv, NormalId face, int texture, bool repeat)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    pv->renderFace(d, face);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}