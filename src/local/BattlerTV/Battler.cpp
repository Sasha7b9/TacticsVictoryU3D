﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Battler.h"
#include "Editor/Editor.h"
#include "Game/Logic/Rotator_.h"
#include "Game/Logic/SunEngine_.h"
#include "Game/Path/TilePath.h"
#include "Game/Path/WaveAlgorithm_.h"
#include "Graphics/2D/Image.h"
#include "GUI/GUI.h"
#include "GUI/GuiEditor/GuiEditor.h"
#include "GUI/Menu/TMenu.h"
#include "GUI/Menu/TMenuEvents.h"
#include "Scene/Level_.h"
#include "Scene/Scene_.h"
#include "Scene/Cameras/Camera.h"
#include "Utils/LogC.h"
#include "Utils/Settings.h"


#pragma warning(push)
#pragma warning(disable:4100)
URHO3D_DEFINE_APPLICATION_MAIN(Battler)
#pragma warning(pop)


Battler::Battler(Context* context) :
    Application(context)
{
    TheContext = context;
}


void Battler::Setup()
{
    ParseArguments(GetArguments());

    TheTacticsVictory = this;
    TheSet = new Settings();
    TheCache = GetSubsystem<ResourceCache>();
    TheFileSystem = GetSubsystem<FileSystem>();
    OpenLog();
//    LOGINFO("Загружаю настройки");
    TheSet->Load();
    //LOGINFO("Загрузка настроек закончена");

    engineParameters_[EP_WINDOW_TITLE] = GetTypeName();
    engineParameters_[EP_LOG_NAME] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_TEXTURE_QUALITY] = 32; //-V112
    engineParameters_[EP_WINDOW_WIDTH] = TheSet->GetInt(TV_SCREEN_WIDTH);
    engineParameters_[EP_WINDOW_HEIGHT] = TheSet->GetInt(TV_SCREEN_HEIGHT);
    engineParameters_[EP_HEADLESS] = false;

    if (!engineParameters_.Contains(EP_RESOURCE_PREFIX_PATHS))
#ifdef DEBUG
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = ";../../../../../../out/debug";
#else
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = ";../../../../../../out/release";
#endif
}


void Battler::Stop()
{
    engine_->DumpResources(true);
    engine_->DumpProfiler();

    TilePath::RemoveAll();

    delete TheScene;
    delete scene;
    delete TheFileSelector;
    delete TheLevel;
    delete TheMenu;
    delete TheGUI;
    delete TheSet;
    delete TheEditor;
    delete TheCamera;
    delete TheLog;
}


void MessageCallback(const asSMessageInfo *msg, void *)
{
    const char *type = "AS ERROR ";
    if(msg->type == asMSGTYPE_WARNING)
        type = "AS WARN ";
    else if(msg->type == asMSGTYPE_INFORMATION)
        type = "AS INFO ";

    LOGINFOF("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message); //-V111
}


void Battler::Start()
{
    TheProfiler = GetSubsystem<Profiler>();
    PROFILER_FUNC_ENTER();
    Application::Start();
    TheCache->AddResourceDir(RESOURCES_DIR);
    SetLocalization();
    TheTime = GetSubsystem<Time>();
    TheFont = TheCache->GetResource<Font>(SET::MENU::FONT::NAME);
    TheProfiler = GetSubsystem<Profiler>();
    TheEngine = GetSubsystem<Engine>();
    TheGraphics = GetSubsystem<Graphics>();

    TheScene = new TScene();

    TheScene->CreateComponent<Octree>();
    ThePhysicsWorld = TheScene->CreateComponent<PhysicsWorld>();
    ThePhysicsWorld->SetGravity(Vector3::ZERO);
    TheScene->CreateComponent<DebugRenderer>();

    CreateScriptSystem();

    SetWindowTitleAndIcon();
    CreateConsoleAndDebugHud();
    TheUI = GetSubsystem<UI>();
    TheInput = GetSubsystem<Input>();
    TheAudio = GetSubsystem<Audio>();
    TheRenderer = GetSubsystem<Renderer>();
    TheCamera = TCamera::Create();
    TheDebugRenderer = TheScene->GetComponent<DebugRenderer>();
    TheUIRoot = TheUI->GetRoot();
    TheUIRoot->SetDefaultStyle(TheCache->GetResource<XMLFile>("UI/MainStyle.xml"));
    TheGUI = new GUI();
    LOGINFO("Загружаю настройки");
    TheMenu = new TMenu();
    TheFileSelector = new FileSelector(TheContext);
    TheFileSelector->GetWindow()->SetModal(false);
    TheFileSelector->GetWindow()->SetVisible(false);

    RegistrationComponets();

    TheLevel = new Level();

    SubscribeToEvents();

    PROFILER_FUNC_LEAVE();
}


void Battler::SetLocalization()
{
    TheLocalization = GetSubsystem<Localization>();
    TheLocalization->LoadJSONFile("Strings.json");
    TheLocalization->SetLanguage("ru");
}


void Battler::CreateScriptSystem()
{
    TheContext->RegisterSubsystem(new Script(TheContext));
    TheScript = GetSubsystem<Script>();
    TheScript->GetScriptEngine()->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
}


void Battler::StartServer(uint16 /*port_*/)
{
//    if (port_)
//    {
//        port = port_;
//    }
//
//    if (port)
//    {
//        TheServer->Start(port);
//        scene = new TScene(TheContext, TScene::Mode_Server);
//        scene->Create();
//    }
//    else
//    {
//        LOGERROR("Can not start server on null port");
//    }
}


void Battler::StopServer()
{

}


void Battler::StartClient(const String &, uint16)
{

}


void Battler::StopClient()
{

}


void Battler::ParseArguments(const Vector<String> & /*arguments*/)
{
}


void Battler::RegistrationComponets()
{
    TImage::RegisterObject();
    SunEngine::RegisterObject();
    Rotator::RegisterObject();

    WaveAlgorithm::RegisterObject();

    TScene::RegisterObject();
}


void Battler::SubscribeToEvents()
{
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Battler, HandleKeyDown));
    SubscribeToEvent(E_MENU, URHO3D_HANDLER(Battler, HandleMenuEvent));
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Battler, HandlePostRenderUpdate));
}


void Battler::SetWindowTitleAndIcon()
{
    Image *icon = TheCache->GetResource<Image>("Textures/TacticsVictoryIcon.png");
    TheGraphics->SetWindowIcon(icon);
    TheGraphics->SetWindowTitle("Тактика победы");
}


void Battler::CreateConsoleAndDebugHud()
{
    XMLFile *xmlFile = TheCache->GetResource<XMLFile>("UI/ConsoleStyle.xml");

    TheEngineConsole = engine_->CreateConsole();
    TheEngineConsole->SetDefaultStyle(xmlFile);
    TheEngineConsole->GetBackground()->SetOpacity(0.8f);

    TheDebugHud = engine_->CreateDebugHud();
    TheDebugHud->SetDefaultStyle(xmlFile);
}


void Battler::CreateEditorSession()
{
    if(!TheEditor)
    {
        TheEditor = new Editor();
    }
    TheGuiEditor->SetVisible(true);
    TheCamera->SetEnabled(true);
    TheEditor->Run();
}


void Battler::OpenLog()
{
    TheLog = new CLog();
    char buffer[50];
    srand(static_cast<uint>(time(static_cast<time_t*>(0)))); //-V202

    sprintf_s(buffer, 50, "Battler.log");

    TheLog->Open(buffer);
    TheLog->SetLevel(LOG_DEBUG);
}