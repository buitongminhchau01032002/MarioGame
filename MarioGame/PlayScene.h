#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "Map.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include <unordered_map>
//#include "Koopas.h"

using namespace std;


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> uis;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_INFOR(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_UIS(string line);
	void _ParseSection_CAMERA(string line);

	void LoadAssets(LPCWSTR assetFile);

	int tileSize = 16; // default value (not hard code)
	int marginScreen = 32; //
	LPMAP map;
	LPCWSTR saveFile;
	int camIdStart;


	unordered_map<int, LPCAMERA> cameras;
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	int GetTileSize() {
		return tileSize;
	}
	int GetMarginScreen() {
		return marginScreen;
	}
	bool IsInScreenBounding(float x, float y);
	void GetObjectXY(int xTile, int yTile, float& x, float& y) {
		x = float(xTile * tileSize + tileSize / 2);
		y = float(yTile * tileSize + tileSize / 2);
	}


	LPGAMEOBJECT GetPlayer() { return player; }
	
	vector<LPGAMEOBJECT>& GetObjects() {
		return objects;
	}

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	LPCWSTR GetSaveFile() { return saveFile; }
	void SwitchCamera(int cameraId);
};

typedef CPlayScene* LPPLAYSCENE;

