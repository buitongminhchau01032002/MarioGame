#pragma once

#include <d3d10.h>
#include <d3dx10.h>
#include <vector>
#include <string>

using namespace std;

struct Tile {
	int xCell;
	int yCell;
	float l;
	float t;
	float r;
	float b;
};

class CMap
{
private:
	int tileSize = 16;
	int width = 0;
	int height = 0;
	int spacing = 0;
	int margin = 0;
	int texId = -9999;
	int widthTexMap = 0;//cell
	LPCWSTR mapFilePath;
	vector<vector<Tile>> layers;
public:
	CMap (LPCWSTR mapFilePath) {
		this->mapFilePath = mapFilePath;
	};
	int GetWidth() {
		return width;
	}
	int GetHeight() {
		return height;
	}
	int GetTileSize() {
		return tileSize;
	}
	void Load();
	void _ParseSection_INFOR(string line);
	void _ParseSection_LAYERS(string line);
	void Render();
};

typedef CMap* LPMAP;


