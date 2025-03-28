#pragma once
#include <stdio.h>

class GameState
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput() = 0;
	virtual void Exit() = 0;
};