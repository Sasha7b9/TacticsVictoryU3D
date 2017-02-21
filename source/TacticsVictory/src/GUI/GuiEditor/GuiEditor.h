#pragma once


class WindowRTS;
class ButtonMain;
class PanelBottom;
class PanelMap;
class PanelMain;
class SliderWithTextAndButtons;


class GuiEditor : public Object
{
    URHO3D_OBJECT(GuiEditor, Object);

public:
    GuiEditor(Context* context = gContext);

    bool CheckOnDeadZoneForCursorBottomScreen(int x);
    bool IsInside(IntVector2 &position);
    bool IsVisible();
    void SetVisible(bool visible);

    enum ModeSelect
    {
        ModeSelect_Plane,
        ModeSelect_Edge
    } modeSelect = ModeSelect_Plane;

private:
    GuiEditor& operator=(const GuiEditor&) {};

    SharedPtr<ButtonMain> buttonInterface;
    SharedPtr<ButtonMain> buttonMenu;

    SharedPtr<ButtonMain> btnNewMap;

    SharedPtr<PanelBottom> panelBottom;
    SharedPtr<PanelMap>    panelMap;
    SharedPtr<PanelMain>   panelMain;

    SharedPtr<WindowRTS>      windowNewMap;
    SharedPtr<WindowRTS>      windowMenu;
    SharedPtr<WindowRTS>      windowConfirmExit;
    SharedPtr<SliderWithTextAndButtons> sliderSizeNewMapX;
    SharedPtr<SliderWithTextAndButtons> sliderSizeNewMapY;

    int width = 100;
    int height = 19;
    int dY = 27;
    int y = 10;
    int x = 10;

    bool IntersectionX(ButtonMain *button, int x);
    void CreateWindows();
    void ToggleInterfacePanels();

    void CreatePanels();
    void CreateTabs();
    void CreateTabFile();
    void CreateTabEdit();
    void CreateTabTerrain();
    void CreateTabObjects();

    // Tab "File"
    void HandleFileLoad(StringHash, VariantMap&);
    void HandleFileSave(StringHash, VariantMap&);
    void HandleFileSelectorLoadTerrain(StringHash, VariantMap&);
    void HandleFileSelectorSaveTerrain(StringHash, VariantMap&);

    // Tab "Edit"
    void HandleEditUndo(StringHash, VariantMap&);
    void HandleEditRedo(StringHash, VariantMap&);

    // Tab "SegmentTerrain"
    void HandleTerrainCreateNewMap(StringHash, VariantMap&);
    void HandleTerrainClearTerrain(StringHash, VariantMap&);
    void HandleTerrainModeSelectChanged(StringHash, VariantMap&);

    // Tab "Objects"
    void HandleObjectsAdd(StringHash, VariantMap&);

    void HandleMouseDown(StringHash, VariantMap&);
    void HandleButtonRelease(StringHash, VariantMap&);
    void HandleExit(StringHash, VariantMap&);
    void HandleOptions(StringHash, VariantMap&);
    void HandleExitOk(StringHash, VariantMap&);
    void HandleExitCancel(StringHash, VariantMap&);
    void HandleKeyDown(StringHash, VariantMap&);
};