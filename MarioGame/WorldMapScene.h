#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Map.h"
#include "MarioMap.h"

#include <vector>


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_UIS	-10
#define SCENE_SECTION_INFOR 3
#define SCENE_SECTION_MAP 4
#define SCENE_SECTION_CAMERA 5
#define SCENE_SECTION_GATE_CONNECTIONS 6
#define SCENE_SECTION_GATES 7
#define SCENE_SECTION_PLAYER 10
#define SCENE_SECTION_PREV_PLAYER 11

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

using namespace std;

class CWorldMapScene: public CScene
{
protected:

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> uis;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_INFOR(string line);
	void _ParseSection_UIS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_CAMERA(string line);
	void _ParseSection_GATE_CONNECTIONS(string line);
	void _ParseSection_GATES(string line);
	void _ParseSection_PLAYER(string line);
	void _ParseSection_PREV_PLAYER(string line);

	void LoadAssets(LPCWSTR assetFile);

	int tileSize = 16; // default value (not hard code)
	LPMAP map;
	LPGAMEOBJECT player;
	LPGAMEOBJECT prevPlayer;
	vector<CGateConnection*> gateConnections;
	vector<CGate*> gates;
	LPCWSTR saveFile;
	bool isInit = true;

public:
	CWorldMapScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	LPGAMEOBJECT GetPlayer() { return player; }
	int GetTileSize() {
		return tileSize;
	}
	void GetObjectXY(int xTile, int yTile, float& x, float& y) {
		x = float(xTile * tileSize + tileSize / 2);
		y = float(yTile * tileSize + tileSize / 2);
	}

	vector<LPGAMEOBJECT>& GetObjects() {
		return objects;
	}
	vector<CGateConnection*> GetGateConnection() { return gateConnections; }
	vector<CGate*> GetGate() { return gates; }

	void SaveToFile();

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CWorldMapScene* LPWORLDMAPSCENE;

