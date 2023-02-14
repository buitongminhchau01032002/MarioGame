#include "WorldMapGrass.h"
#include "Animations.h"

void CWorldMapGrass::Render() {
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_WORLDMAP_GRASS);
	ani->Render(x, y);
}