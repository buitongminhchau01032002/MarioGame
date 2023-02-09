#include "MarioMap.h"
#include "Animations.h"
#include "Map.h"
#include "debug.h"
#include "Game.h"
#include "WorldMapScene.h"
#include <vector>
using namespace std;

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
	LPANIMATION ani = CAnimations::GetInstance()->Get(1200);
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
		// check xCell + 1
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
		// check yCell - 1
		CGateConnection* gateConnection = NULL;
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