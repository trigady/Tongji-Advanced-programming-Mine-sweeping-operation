/****************************************
GameEngine.h
Author: Root
Date: 2019/12/09
Description:
定义了程序游戏模块的头文件，用GameEngine命名空间防止同名污染
各函数说明：
void InitGame()	   初始化游戏
void Play()		   开始游戏
void DestroyGame() 结束游戏
*****************************************/
#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "Graphic.h"
#include "Controller.h"
#include "GameMenu.h"
#include <time.h>
namespace GameEngine {
	void InitGame(int getWidth, int getHeight, int getBoom);
	void Play();
	void DestroyGame();
}
#endif // GAMEENGINE_H