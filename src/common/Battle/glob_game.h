// 2021/02/15 16:12:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by 
#pragma once

class Battle;
class ConfigurationFile;
class ServerT;
class SceneS;
class ServerS;
class TerrainT;

extern ConfigurationFile TheConfig;
extern ConfigurationFile TheSettings;
extern ServerT      TheMasterServer;

extern Battle       *TheBattle;
extern SceneS       *TheScene;
extern ServerS      *TheServer;
extern TerrainT     *TheTerrain;
