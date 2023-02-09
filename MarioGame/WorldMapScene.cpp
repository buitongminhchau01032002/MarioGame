#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "BottomBar.h"
#include "WorldMapScene.h"
#include "WorldMapKeyEventHandler.h"
#include "debug.h"
#include "Game.h"
#include "Camera.h"
using namespace std;


CWorldMapScene::CWorldMapScene(int id, LPCWSTR filePath):CScene(id, filePath)
{
	key_handler = new CWorldMapKeyHandler(this);
	gateConnections.push_back(new CGateConnection(2, 4, false, true)); //
	gateConnections.push_back(new CGateConnection(3, 4, false, false));
	gateConnections.push_back(new CGateConnection(4, 4, false, true, 0, 0, 0, 1));
	gateConnections.push_back(new CGateConnection(4, 3, false, false));
	gateConnections.push_back(new CGateConnection(4, 2, false, true)); //
	gateConnections.push_back(new CGateConnection(5, 2, false, false));
	gateConnections.push_back(new CGateConnection(6, 2, false, true));
	gateConnections.push_back(new CGateConnection(7, 2, false, false));
	gateConnections.push_back(new CGateConnection(8, 2, false, true)); //
	gateConnections.push_back(new CGateConnection(9, 2, false, false));
	gateConnections.push_back(new CGateConnection(10, 2, false, true)); //
	gateConnections.push_back(new CGateConnection(11, 2, false, false));
	gateConnections.push_back(new CGateConnection(12, 2, false, true));
	gateConnections.push_back(new CGateConnection(12, 3, false, false));
	gateConnections.push_back(new CGateConnection(12, 4, false, true)); //
	gateConnections.push_back(new CGateConnection(11, 4, false, false));
	gateConnections.push_back(new CGateConnection(10, 4, false, true)); //
	gateConnections.push_back(new CGateConnection(9, 4, false, false));


	gateConnections.push_back(new CGateConnection(8, 3, false, false));
	gateConnections.push_back(new CGateConnection(8, 4, false, true));
	gateConnections.push_back(new CGateConnection(8, 5, false, false));
	gateConnections.push_back(new CGateConnection(8, 6, false, true)); //
	gateConnections.push_back(new CGateConnection(7, 6, false, false));
	gateConnections.push_back(new CGateConnection(6, 6, false, true)); //
	gateConnections.push_back(new CGateConnection(5, 6, false, false));
	gateConnections.push_back(new CGateConnection(4, 6, false, true));
	gateConnections.push_back(new CGateConnection(4, 5, false, true)); //

	gateConnections.push_back(new CGateConnection(4, 7, false, false));
	gateConnections.push_back(new CGateConnection(4, 8, false, true));
	gateConnections.push_back(new CGateConnection(4, 9, false, false));
	gateConnections.push_back(new CGateConnection(4, 10, false, true)); //
	gateConnections.push_back(new CGateConnection(5, 10, false, false));
	gateConnections.push_back(new CGateConnection(6, 10, false, true));
	gateConnections.push_back(new CGateConnection(7, 10, false, false));
	gateConnections.push_back(new CGateConnection(8, 10, false, true)); //
	gateConnections.push_back(new CGateConnection(8, 9, false, false));
	gateConnections.push_back(new CGateConnection(8, 8, false, true));
	gateConnections.push_back(new CGateConnection(7, 8, false, false));
	gateConnections.push_back(new CGateConnection(6, 8, false, true)); //

	gateConnections.push_back(new CGateConnection(9, 8, false, false));
	gateConnections.push_back(new CGateConnection(10, 8, false, true));
	gateConnections.push_back(new CGateConnection(11, 8, false, false));
	gateConnections.push_back(new CGateConnection(12, 8, false, true)); // 

}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_UIS	-10
#define SCENE_SECTION_INFOR 3
#define SCENE_SECTION_MAP 4
#define SCENE_SECTION_CAMERA 5

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024


void CWorldMapScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CWorldMapScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CWorldMapScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CWorldMapScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	LPCWSTR path = ToLPCWSTR(tokens[0]);
	map = new CMap(path);
}

void CWorldMapScene::_ParseSection_INFOR(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;
	int tileSize = atof(tokens[0].c_str());
	this->tileSize = tileSize;
}

/*
	Parse a line in section [OBJECTS]
*/
void CWorldMapScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	objects.push_back(obj);
}


/*
	Parse a line in section [UIS]
*/
void CWorldMapScene::_ParseSection_UIS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 1) return;

	int object_type = atoi(tokens[0].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_BOTTOM_BAR: {
		obj = new CBottomBar();
		break;
	}
	default:
		DebugOut(L"[ERROR] Invalid object type (ui): %d\n", object_type);
		return;
	}
	uis.push_back(obj);
}

void CWorldMapScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CWorldMapScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[UIS]") { section = SCENE_SECTION_UIS; continue; };
		if (line == "[INFOR]") { section = SCENE_SECTION_INFOR; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_UIS: _ParseSection_UIS(line); break;
		case SCENE_SECTION_INFOR: _ParseSection_INFOR(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	if (map != NULL) {
		map->Load();
	}

	int w = CGame::GetInstance()->GetBackBufferWidth();
	int h = CGame::GetInstance()->GetBackBufferHeight();
	CGame::GetInstance()->SetCamera(
		new CCamera(0, 0, w, h, NULL,
			0, 0, 0, 0,
			0, 0, w, h,
			0, 0));

	player = new CMarioMap(2, 4);
	objects.push_back(player);

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CWorldMapScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	//if (player == NULL) return;

	PurgeDeletedObjects();
}

void CWorldMapScene::Render()
{
	if (map != NULL) {
		map->Render();
	}
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	for (int i = 0; i < uis.size(); i++)
		uis[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CWorldMapScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();

	for (it = uis.begin(); it != uis.end(); it++)
	{
		delete (*it);
	}
	uis.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CWorldMapScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();

	for (int i = 0; i < uis.size(); i++)
		delete uis[i];
	uis.clear();

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CWorldMapScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CWorldMapScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CWorldMapScene::IsGameObjectDeleted),
		objects.end());


	for (it = uis.begin(); it != uis.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	uis.erase(
		std::remove_if(uis.begin(), uis.end(), CWorldMapScene::IsGameObjectDeleted),
		uis.end());
}