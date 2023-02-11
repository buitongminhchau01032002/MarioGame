#pragma once
#include "Scene.h"
class CStartKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CStartKeyHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};
