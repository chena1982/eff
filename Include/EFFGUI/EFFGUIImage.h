/******************************************************************************
created:	2017-07-14   23:35
file path:	d:\EFF\Include\EFFGUI\EFFGUIImage.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUIImage_2017_07_14__
#define __EFFGUIImage_2017_07_14__


EFFGUI_BEGIN


effVOID RenderingImage(const FloatCoord & coord, EFF3DTexture * texture);


class EFFGUIImage
{
public:
    EFFGUIImage()
    {
        textureNameId = -1;
        color = 0;
    }

    ~EFFGUIImage() {}

private:
    effINT textureNameId;
    EFF3DCOLOR color;
};


class EFFGUIImageManager : EFFComponentManager<EFFGUIImage>
{
    friend class EFFGUIRenderer;
public:
    EFFGUIImageManager();
    ~EFFGUIImageManager();

protected:

};

EFFGUI_END

#endif