﻿/* (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by */
#include "stdafx.h"
#include "UnitObject.h"
#include "Scene/Objects/_GameObjectEvents.h"



bool UnitObject::viewTargetView = false;



UnitObject::UnitObject(Context *context) : GameObject(context)
{
    Node* nodeCameraTarget = TheScene->CreateChild(NODE_CAMERA_TARGET);
    nodeCameraTarget->Pitch(180.0f);
    nodeCameraTarget->Yaw(180.0f);
    cameraTarget = nodeCameraTarget->CreateComponent<Camera>();
    cameraTarget->SetNearClip(0.5f);
    cameraTarget->SetFarClip(100.0f);

    renderTexture = new Texture2D(TheContext);
    renderTexture->SetSize(WIDTH_WINDOW_TARGET, HEIGHT_WINDOW_TARGET, Graphics::GetRGBFormat(), TEXTURE_RENDERTARGET);
    renderTexture->SetFilterMode(FILTER_DEFAULT);

    renderSurface = renderTexture->GetRenderSurface();
    SharedPtr<Viewport> viewport(new Viewport(TheContext, TheScene, cameraTarget));
    renderSurface->SetViewport(0, viewport);

    nodeCameraTarget->SetVar("renderSurface", Variant(renderSurface));
}


UnitObject::~UnitObject()
{

}


void UnitObject::SetViewTargetView(bool view)
{
    viewTargetView = view;
}


void UnitObject::HandlePostRenderUpdate(StringHash, VariantMap&)
{
    cameraTarget->GetNode()->SetPosition(GetNode()->GetPosition() + Vector3(0.0f, 0.2f, 0.0f));
    cameraTarget->GetNode()->SetRotation(GetNode()->GetRotation() * Quaternion(180.0f, Vector3::UP));

    VariantMap eventData = GetEventDataMap();
    eventData[GameObjectEvent::P_TEXTURE] = renderTexture;
    SendEvent(E_SETTEXTURE, eventData);
}


void UnitObject::SetSelected(bool sel)
{
    GameObject::SetSelected(sel);

    if(sel && viewTargetView)
    {
        renderSurface->SetUpdateMode(SURFACE_UPDATEALWAYS);
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(UnitObject, HandlePostRenderUpdate));
    }
    else
    {
        renderSurface->SetUpdateMode(SURFACE_MANUALUPDATE);
        UnsubscribeFromEvent(E_POSTRENDERUPDATE);
    }
}
