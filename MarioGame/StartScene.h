#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Map.h"
#include "MarioMap.h"

#include <vector>


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_BG	10
#define SCENE_SECTION_INFOR 3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

using namespace std;

#define ID_SPRITE_START_SCENE_BG -999999
class CStartSceneBg : public CGameObject {
public:
	CStartSceneBg(float x, float y) : CGameObject(x, y) {};
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	};
	int IsBlocking() { return 0; }
};

class CStartScene : public CScene
{
protected:

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_INFOR(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	LPCWSTR saveFile;


public:
	CStartScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	vector<LPGAMEOBJECT>& GetObjects() {
		return objects;
	}

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CStartScene* LPSTARTSCENE;
