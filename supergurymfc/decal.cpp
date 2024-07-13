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

                params.wrapMode = wrapMode;
                params.maxAnisotropy = 1.0f;

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

void RBX::Decal::render(RenderDevice* rd, RBX::Render::IRenderable* p)
{
    if (texture.isNull())
    {
        initContentTexture();
        return;
    }

    Render::rawDecal(rd, p, face, getGLid(), 0);
}

void RBX::Render::rawDecal(RenderDevice* d, RBX::Render::IRenderable* pv, NormalId face, int texture, bool repeat)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture); 
    
    if (repeat)
    {
        pv->renderFace(d, face);
    }
    else
    {
        pv->renderFaceFitForDecal(d, face);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}