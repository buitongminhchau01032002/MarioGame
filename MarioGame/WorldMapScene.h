#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Map.h"
#include "MarioMap.h"

#include <vector>
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

	void LoadAssets(LPCWSTR assetFile);

	int tileSize = 16; // default value (not hard code)
	LPMAP map;
	LPGAMEOBJECT player;
	vector<CGateConnection*> gateConnections;

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

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CWorldMapScene* LPWORLDMAPSCENE;

