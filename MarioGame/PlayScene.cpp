#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "MyPortal.h"
#include "Coin.h"
#include "Platform.h"
#include "HiddenBlock.h"
#include "HiddenBlockMario.h"
#include "HiddenBlockKoopa.h"
#include "HiddenBlockGoomba.h"
#include "DieBlock.h"
#include "QuestionBox.h"
#include "QuestionBoxCoin.h"
#include "QuestionBoxMushroom.h"
#include "CoinFlyUp.h"
#include "Koopa.h"
#include "GoombaPro.h"
#include "Chomper.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_INFOR 3
#define SCENE_SECTION_MAP 4
#define SCENE_SECTION_CAMERA 5

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	LPCWSTR path = ToLPCWSTR(tokens[0]);
	map = new CMap(path);
}

void CPlayScene::_ParseSection_INFOR(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;
	int tileSize = atof(tokens[0].c_str());
	this->tileSize = tileSize;
	int marginScreen = atof(tokens[1].c_str());
	this->marginScreen = marginScreen;
}

void CPlayScene::_ParseSection_CAMERA(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6 || tokens[0] == "") return;
	float xCam = (float)atof(tokens[0].c_str());
	float yCam = (float)atof(tokens[1].c_str());
	float offsetLeft = (float)atof(tokens[2].c_str());
	float offsetTop = (float)atof(tokens[3].c_str());
	float offsetRight = (float)atof(tokens[4].c_str());
	float offsetBottom = (float)atof(tokens[5].c_str());
	int w = CGame::GetInstance()->GetBackBufferWidth();
	int h = CGame::GetInstance()->GetBackBufferHeight();
	CGame::GetInstance()->SetCamera( new CCamera(xCam, yCam, w, h, this->player, offsetLeft, offsetTop, offsetRight, offsetBottom));
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y); break;
	case OBJECT_TYPE_GOOMBA_PRO: obj = new CGoombaPro(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_QUESTION_BOX: obj = new CQuestionBox(x, y); break;
	case OBJECT_TYPE_QUESTION_BOX_COIN: obj = new CQuestionBoxCoin(x, y); break;
	case OBJECT_TYPE_QUESTION_BOX_MUSHROOM: obj = new CQuestionBoxMushroom(x, y); break;
	case OBJECT_TYPE_KOOPA: obj = new CKoopa(x, y); break;
	case OBJECT_TYPE_CHOMPER: obj = new CChomper(x, y); break;


	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;

	case OBJECT_TYPE_HIDDEN_BLOCK:
	{
		int lCell = atof(tokens[1].c_str());
		int tCell = atof(tokens[2].c_str());
		int rCell = atoi(tokens[3].c_str());
		int bCell = atoi(tokens[4].c_str());
		int isBlockingLeft = atoi(tokens[5].c_str());
		int isBlockingTop = atoi(tokens[6].c_str());
		int isBlockingRight = atoi(tokens[7].c_str());
		int isBlockingBottom = atoi(tokens[8].c_str());
		obj = new CHiddenBlock(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom);
		obj->GetPosition(x, y);
	}

	break;

	case OBJECT_TYPE_HIDDEN_BLOCK_MARIO:
	{
		int lCell = atof(tokens[1].c_str());
		int tCell = atof(tokens[2].c_str());
		int rCell = atoi(tokens[3].c_str());
		int bCell = atoi(tokens[4].c_str());
		int isBlockingLeft = atoi(tokens[5].c_str());
		int isBlockingTop = atoi(tokens[6].c_str());
		int isBlockingRight = atoi(tokens[7].c_str());
		int isBlockingBottom = atoi(tokens[8].c_str());
		obj = new CHiddenBlockMario(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom);
		obj->GetPosition(x, y);
	}
	break;
	case OBJECT_TYPE_HIDDEN_BLOCK_KOOPA:
	{
		int lCell = atof(tokens[1].c_str());
		int tCell = atof(tokens[2].c_str());
		int rCell = atoi(tokens[3].c_str());
		int bCell = atoi(tokens[4].c_str());
		int isBlockingLeft = atoi(tokens[5].c_str());
		int isBlockingTop = atoi(tokens[6].c_str());
		int isBlockingRight = atoi(tokens[7].c_str());
		int isBlockingBottom = atoi(tokens[8].c_str());
		obj = new CHiddenBlockKoopa(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom);
		obj->GetPosition(x, y);
	}
	break;

	case OBJECT_TYPE_HIDDEN_BLOCK_GOOMBA:
	{
		int lCell = atof(tokens[1].c_str());
		int tCell = atof(tokens[2].c_str());
		int rCell = atoi(tokens[3].c_str());
		int bCell = atoi(tokens[4].c_str());
		int isBlockingLeft = atoi(tokens[5].c_str());
		int isBlockingTop = atoi(tokens[6].c_str());
		int isBlockingRight = atoi(tokens[7].c_str());
		int isBlockingBottom = atoi(tokens[8].c_str());
		obj = new CHiddenBlockGoomba(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom);
		obj->GetPosition(x, y);
	}
	break;


	case OBJECT_TYPE_DIE_BLOCK:
	{
		int lCell = atof(tokens[1].c_str());
		int tCell = atof(tokens[2].c_str());
		int rCell = atoi(tokens[3].c_str());
		int bCell = atoi(tokens[4].c_str());
		int isBlockingLeft = atoi(tokens[5].c_str());
		int isBlockingTop = atoi(tokens[6].c_str());
		int isBlockingRight = atoi(tokens[7].c_str());
		int isBlockingBottom = atoi(tokens[8].c_str());
		obj = new CDieBlock(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom);
		obj->GetPosition(x, y);
	}
	break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
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

void CPlayScene::Load()
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
		if (line == "[INFOR]") { section = SCENE_SECTION_INFOR; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[CAMERA]") { section = SCENE_SECTION_CAMERA; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_INFOR: _ParseSection_INFOR(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
			case SCENE_SECTION_CAMERA: _ParseSection_CAMERA(line); break;
		}
	}

	f.close();

	if (map != NULL) {
		map->Load();
	}

	LPCAMERA camera = CGame::GetInstance()->GetCamera();
	float limitRight = float(map->GetWidth() * map->GetTileSize() + map->GetTileSize());
	float limitBottom = float(map->GetHeight() * map->GetTileSize() + map->GetTileSize());
	if (camera != NULL) {
		camera->SetFollowing(this->player);
		camera->SetLimit(0.0f, 0.0f, limitRight, limitBottom);
	}

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
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
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;

	LPCAMERA camera = CGame::GetInstance()->GetCamera();
	if (camera != NULL) {
		camera->Update();
	}

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	if (map != NULL) {
		map->Render();
	}
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsInScreenBounding(float x, float y)
{
	LPCAMERA camera = CGame::GetInstance()->GetCamera();
	if (camera == NULL) {
		return false;
	}
	float xCam, yCam;
	camera->GetPos(xCam, yCam);
	int wScreen = CGame::GetInstance()->GetBackBufferWidth();
	int hScreen = CGame::GetInstance()->GetBackBufferHeight();
	if (x < xCam - marginScreen || x > xCam + wScreen + marginScreen) {
		return false;
	}
	if (y < yCam - marginScreen || y > yCam + hScreen + marginScreen) {
		return false;
	}
	return true;
}


bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
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
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}
