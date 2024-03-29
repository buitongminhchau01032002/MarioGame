#include "MarioMap.h"
#include "Animations.h"
#include "Sprites.h"
#include "Map.h"
#include "debug.h"
#include "Game.h"
#include "WorldMapScene.h"
#include "Mario.h"
#include <vector>
using namespace std;

void CGate::Render() {
	LPSPRITE sprite = CSprites::GetInstance()->Get(ID_SPRITE_GATE_COMPLETED);
	if (isCompleted) {
		sprite->Draw(x+1, y-1);
	}
}

CMarioMap::CMarioMap(int xCell, int yCell) 
	{
		float x, y;
		this->xCell = xCell;
		this->yCell = yCell;
		GetObjectXY(xCell, yCell, x, y);
		SetPosition(x, y);
	}

void CMarioMap::Render()
{
	int level = CGame::GetInstance()->GetLevel();
	int aniId = ID_ANI_MARIO_MAP_SMALL;
	if (level == MARIO_LEVEL_SMALL) {
		aniId = ID_ANI_MARIO_MAP_SMALL;
	} else if (level == MARIO_LEVEL_BIG) {
		aniId = ID_ANI_MARIO_MAP_BIG;
	} else if (level == MARIO_LEVEL_CAT) {
		aniId = ID_ANI_MARIO_MAP_CAT;
	}
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
	ani->Render(x, y);
}

void CMarioMap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state != MARIO_MAP_STATE_MOVING) {
		return;
	}

	int isDone = 0;
	float vx, vy;
	float xTo, yTo;
	int dy = abs(yCellTo - yCell);
	int dx = abs(xCellTo - xCell);
	float d = sqrt(dx * dx + dy * dy);
	float k = v / d;
	GetObjectXY(xCellTo, yCellTo, xTo, yTo);

	if (xCellTo > xCell) {
		if (dy == 0) {
			vx = v;
		}
		else {
			vx = dx * k;
		}
		x += vx * dt;
		if (x >= xTo) {
			x = xTo;
			isDone++;
			xCell = xCellTo;
		}
	}
	else if (xCellTo < xCell) {
		if (dy == 0) {
			vx = -v;
		}
		else {
			vx = -dx * k;
		}
		x += vx * dt;
		if (x <= xTo) {
			x = xTo;
			isDone++;
			xCell = xCellTo;
		}
	}
	else {
		isDone++;
	}

	if (yCellTo > yCell) {
		if (dx == 0) {
			vy = v;
		}
		else {
			vy = dy * k;
		}
		y += vy * dt;
		if (y >= yTo) {
			y = yTo;
			isDone++;
			yCell = yCellTo;
		}
	}
	else if (yCellTo < yCell) {
		if (dx == 0) {
			vy = -v;
		}
		else {
			vy = -dy * k;
		}
		y += vy * dt;
		if (y <= yTo) {
			y = yTo;
			isDone++;
			yCell = yCellTo;
		}
	}
	else {
		isDone++;
	}

	DebugOutTitle(L"state: %d, isDone: %d", state, isDone);

	if (isDone == 2) {
		state = MARIO_MAP_STATE_NONE;
	}
}

void CMarioMap::GoRight()
{
	CGame* g = CGame::GetInstance();
	CWorldMapScene* scene = dynamic_cast<CWorldMapScene*>(g->GetCurrentScene());
	if (!scene)
	{ 
		return;
	}
	vector<CGateConnection*> gateConnections = scene->GetGateConnection();
	vector<CGate*> gates = scene->GetGate();


	int xCellTo = xCell;
	CGateConnection* gateConnection = NULL;
	// check yCell
	for (int i = 0; i < gateConnections.size(); i++) {
		if (gateConnections[i]->GetXCell() == xCell && gateConnections[i]->GetYCell() == yCellTo) {
			gateConnection = gateConnections[i];
			break;
		}
	}
	if (gateConnection && gateConnection->IsBlockingRight()) {
		return;
	}
	while(true) {
		// check xCell + 1 gate
		CGate* gate = NULL;
		for (int i = 0; i < gates.size(); i++) {
			if (gates[i]->GetXCell() == xCellTo + 1 && gates[i]->GetYCell() == yCell) {
				gate = gates[i];
				break;
			}
		}
		if (gate) {
			xCellTo++;
			break;
		}

		// check xCell + 1 gateconnection
		gateConnection = NULL;
		for (int i = 0; i < gateConnections.size(); i++) {
			if (gateConnections[i]->GetXCell() == xCellTo + 1 && gateConnections[i]->GetYCell() == yCell) {
				gateConnection = gateConnections[i];
				break;
			}
		}
		if (!gateConnection) {
			break;
		}
		if (gateConnection->IsBlocking() || gateConnection->IsBlockingLeft()) {
			break;
		}
		xCellTo++;
		if (gateConnection->IsNode()) {
			break;
		}
	}
	GoToCell(xCellTo, yCell);
}

void CMarioMap::GoDown()
{
	CGame* g = CGame::GetInstance();
	CWorldMapScene* scene = dynamic_cast<CWorldMapScene*>(g->GetCurrentScene());
	if (!scene)
	{
		return;
	}
	vector<CGateConnection*> gateConnections = scene->GetGateConnection();
	vector<CGate*> gates = scene->GetGate();

	int yCellTo = yCell;
	CGateConnection* gateConnection = NULL;
	// check yCell
	for (int i = 0; i < gateConnections.size(); i++) {
		if (gateConnections[i]->GetXCell() == xCell && gateConnections[i]->GetYCell() == yCellTo) {
			gateConnection = gateConnections[i];
			break;
		}
	}
	if (gateConnection && gateConnection->IsBlockingBottom()) {
		return;
	}
	while (true) {
		// check yCell + 1 gate
		CGate* gate = NULL;
		for (int i = 0; i < gates.size(); i++) {
			if (gates[i]->GetXCell() == xCell && gates[i]->GetYCell() == yCellTo + 1) {
				gate = gates[i];
				break;
			}
		}
		if (gate) {
			yCellTo++;
			break;
		}

		// check yCell + 1
		gateConnection = NULL;
		for (int i = 0; i < gateConnections.size(); i++) {
			if (gateConnections[i]->GetXCell() == xCell && gateConnections[i]->GetYCell() == yCellTo + 1) {
				gateConnection = gateConnections[i];
				break;
			}
		}
		if (!gateConnection) {
			break;
		}
		if (gateConnection->IsBlocking() || gateConnection->IsBlockingTop()) {
			break;
		}
		yCellTo++;
		if (gateConnection->IsNode()) {
			break;
		}
	}
	GoToCell(xCell, yCellTo);
}

void CMarioMap::GoLeft()
{
	CGame* g = CGame::GetInstance();
	CWorldMapScene* scene = dynamic_cast<CWorldMapScene*>(g->GetCurrentScene());
	if (!scene)
	{
		return;
	}
	vector<CGateConnection*> gateConnections = scene->GetGateConnection();
	vector<CGate*> gates = scene->GetGate();

	int xCellTo = xCell;
	CGateConnection* gateConnection = NULL;
	// check yCell
	for (int i = 0; i < gateConnections.size(); i++) {
		if (gateConnections[i]->GetXCell() == xCell && gateConnections[i]->GetYCell() == yCellTo) {
			gateConnection = gateConnections[i];
			break;
		}
	}
	if (gateConnection && gateConnection->IsBlockingLeft()) {
		return;
	}
	while (true) {
		// check xCell - 1 gate
		CGate* gate = NULL;
		for (int i = 0; i < gates.size(); i++) {
			if (gates[i]->GetXCell() == xCellTo - 1 && gates[i]->GetYCell() == yCell) {
				gate = gates[i];
				break;
			}
		}
		if (gate) {
			xCellTo--;
			break;
		}


		// check xCell - 1
		gateConnection = NULL;
		for (int i = 0; i < gateConnections.size(); i++) {
			if (gateConnections[i]->GetXCell() == xCellTo - 1 && gateConnections[i]->GetYCell() == yCell) {
				gateConnection = gateConnections[i];
				break;
			}
		}
		if (!gateConnection) {
			break;
		}
		if (gateConnection->IsBlocking() || gateConnection->IsBlockingRight()) {
			break;
		}
		xCellTo--;
		if (gateConnection->IsNode()) {
			break;
		}
	}
	GoToCell(xCellTo, yCell);
}

void CMarioMap::GoUp()
{
	CGame* g = CGame::GetInstance();
	CWorldMapScene* scene = dynamic_cast<CWorldMapScene*>(g->GetCurrentScene());
	if (!scene)
	{
		return;
	}
	vector<CGateConnection*> gateConnections = scene->GetGateConnection();
	vector<CGate*> gates = scene->GetGate();

	int yCellTo = yCell;
	CGateConnection* gateConnection = NULL;
	// check yCell
	for (int i = 0; i < gateConnections.size(); i++) {
		if (gateConnections[i]->GetXCell() == xCell && gateConnections[i]->GetYCell() == yCellTo) {
			gateConnection = gateConnections[i];
			break;
		}
	}
	if (gateConnection && gateConnection->IsBlockingTop()) {
		return;
	}
	while (true) {
		// check yCell - 1 gate
		CGate* gate = NULL;
		for (int i = 0; i < gates.size(); i++) {
			if (gates[i]->GetXCell() == xCell && gates[i]->GetYCell() == yCellTo - 1) {
				gate = gates[i];
				break;
			}
		}
		if (gate) {
			yCellTo--;
			break;
		}


		// check yCell - 1
		gateConnection = NULL;
		for (int i = 0; i < gateConnections.size(); i++) {
			if (gateConnections[i]->GetXCell() == xCell && gateConnections[i]->GetYCell() == yCellTo - 1) {
				gateConnection = gateConnections[i];
				break;
			}
		}
		if (!gateConnection) {
			break;
		}
		if (gateConnection->IsBlocking() || gateConnection->IsBlockingBottom()) {
			break;
		}
		yCellTo--;
		if (gateConnection->IsNode()) {
			break;
		}
	}
	GoToCell(xCell, yCellTo);
}