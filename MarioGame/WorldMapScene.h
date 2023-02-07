#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Map.h"
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

public:
	CWorldMapScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
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

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CWorldMapScene* LPWORLDMAPSCENE;

