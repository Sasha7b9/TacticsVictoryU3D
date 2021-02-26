// 2021/02/26 15:54:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Scene/Objects/Units/Tank_.h"
#include "Scene/Objects/Units/UnitObjectC.h"
#include "Scene/Objects/Units/UnitsEvents_.h"


void UnitObjectSpecificPart::Create(Node *node)
{
    node->CreateComponent<UnitObjectSpecificPartC>();
}


void UnitObjectSpecificPartC::HandleMouseClick(StringHash, VariantMap &eventData)
{
    using namespace UnitMouseClick;

    if (eventData[P_NODE].GetPtr() == node_)
    {
        if (!eventData[P_CTRL_PRESSED].GetBool())
        {
            for (Tank *t : Tank::storage)
            {
                if (t->GetNode() != node_)
                {
                    t->GetNode()->GetComponent<UnitObjectSpecificPartC>()->tile->Disable();
                }
            }
        }

        tile->IsEnabled() ? tile->Disable() : tile->Enable();
    }
}


void UnitObjectSpecificPartC::OnNodeSet(Node *node)
{
    if (node)
    {
        UnitObjectSpecificPart::OnNodeSet(node);

        tile = node->CreateComponent<TileSelected>();

        SubscribeToEvent(EU_MOUSE_CLICK, URHO3D_HANDLER(UnitObjectSpecificPartC, HandleMouseClick));
    }
}
