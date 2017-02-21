#pragma once


#include "GUI/Elements/Window.h"


enum
{
    MenuEvent_MenuOptionsClose
};


class MenuOptions : public WindowRTS
{
    URHO3D_OBJECT(MenuOptions, WindowRTS);

public:
    MenuOptions(Context *context = gContext);

    static void RegisterObject(Context *context = gContext);

private:
    MenuOptions& operator=(const MenuOptions&) {};

    void HandleItemSelected(StringHash eventType, VariantMap& eventData);
    void HandleOnSlider(StringHash evenType, VariantMap& eventData);
    void HandleButtonRelease(StringHash eventType, VariantMap& eventData);

    HashMap<Slider*, uint> mapSlidersActions;

    SharedPtr<SliderWithTextAndButtons> sliderBrightness;
    SharedPtr<SliderWithTextAndButtons> sliderMaxOccluderTriangles;
    SharedPtr<SliderWithTextAndButtons> sliderVolume;

    SharedPtr<DropDownListWithTextAndButton> ddlTextureQuality;
    SharedPtr<DropDownListWithTextAndButton> ddlMaterialQuality;
    SharedPtr<DropDownListWithTextAndButton> ddlTextureAnisotropy;
    SharedPtr<DropDownListWithTextAndButton> ddlShadowsEnabled;
    SharedPtr<DropDownListWithTextAndButton> ddlSpecularLighting;
    SharedPtr<DropDownListWithTextAndButton> ddlShadowMapSize;
    SharedPtr<DropDownListWithTextAndButton> ddlShadowQuality;
    SharedPtr<DropDownListWithTextAndButton> ddlDynamicInstancing;
    PODVector<int> shadowMapSizes;
    SharedPtr<ButtonMain> buttonClose;
    IntVector2 dragBeginPosition;
};