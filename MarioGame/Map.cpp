#include <iostream>
#include <fstream>
#include "Map.h"
#include "debug.h"
#include "Utils.h"
#include "Textures.h"
#include "Game.h"

#define MAP_SECTION_UNKNOWN -1
#define MAP_SECTION_INFOR	1
#define MAP_SECTION_LAYERS	2

#define MAX_MAP_LINE 100000


void CMap::Load()
{
	DebugOut(L"[INFO] Start loading map from : %s \n", mapFilePath);
	ifstream f;
	f.open(mapFilePath);

	int section = MAP_SECTION_UNKNOWN;

	char str[MAX_MAP_LINE];
	while (f.getline(str, MAX_MAP_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[INFOR]") { section = MAP_SECTION_INFOR; continue; };
		if (line == "[LAYERS]") { section = MAP_SECTION_LAYERS; continue; };
		if (line[0] == '[') { section = MAP_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case MAP_SECTION_INFOR: _ParseSection_INFOR(line); break;
		case MAP_SECTION_LAYERS: _ParseSection_LAYERS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading map from %s\n", mapFilePath);
}

void CMap::_ParseSection_INFOR(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6 || tokens[0] == "") return;
	this->width = atof(tokens[0].c_str());
	this->height = atof(tokens[1].c_str());
	this->tileSize = atof(tokens[2].c_str());
	this->spacing = atof(tokens[3].c_str());
	this->margin = atof(tokens[4].c_str());
	this->texId = atof(tokens[5].c_str());
	this->widthTexMap = atof(tokens[6].c_str());
}

void CMap::_ParseSection_LAYERS(string line)
{
	vector<string> tokens = split(line, ",");
	vector<Tile> layer;
	if (tokens.size() < 1 || tokens[0] == "") return;
	for (int i = 0; i < tokens.size(); i++) {
		int value = atof(tokens[i].c_str());
		if (value == 0)
			continue;
		int xCell = i % width;
		int yCell = i / width;
		int xCellTex = (value-1) % widthTexMap;
		int yCellTex = (value-1) / widthTexMap;
		float l = float(margin + xCellTex * tileSize + xCellTex * spacing);
		float r = float(l + tileSize);
		float t = float(margin + yCellTex * tileSize + yCellTex * spacing);
		float b = float(t + tileSize);
		Tile tile = { xCell, yCell, l, t, r, b };
		layer.push_back(tile);
	}
	layers.push_back(layer);
}

void CMap::Render()
{
	for (int iLayers = 0; iLayers < layers.size(); iLayers++) {
		for (int iTiles = 0; iTiles < layers[iLayers].size(); iTiles++) {

			float l = layers[iLayers][iTiles].l;
			float t = layers[iLayers][iTiles].t;
			float r = layers[iLayers][iTiles].r;
			float b = layers[iLayers][iTiles].b;
			float x = layers[iLayers][iTiles].xCell * tileSize + float(tileSize) / 2;
			float y = layers[iLayers][iTiles].yCell * tileSize + float(tileSize) / 2;
																			  
			RECT rect;
			LPTEXTURE tex = CTextures::GetInstance()->Get(texId);
			rect.left = l;
			rect.top = t;
			rect.right = r;
			rect.bottom = b;

			float cx, cy;
			//CGame::GetInstance()->GetCamPos(cx, cy);
			CGame::GetInstance()->GetCamera()->GetPos(cx, cy);
			cx = (FLOAT)floor(cx);
			cy = (FLOAT)floor(cy);
			CGame::GetInstance()->Draw(x - cx, y - cy, tex, &rect);

			
		}
	}
	
}
