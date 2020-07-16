/****************************************
GameMenu.h
Author: Root
Date: 2019/12/09
Description:
定义了程序菜单的渲染/管理模块的头文件，用GameMenu命名空间防止同名污染
各函数说明：
void InitMenu() 	 初始化菜单，并渲染菜单选项
void WaitChoice()	 等待用户输入（选择菜单等）
*****************************************/
#pragma once
#ifndef GAMEMENU_H
#define GAMEMENU_H
#include "Graphic.h"
#include "Controller.h"
#include "GameEngine.h"
#include "GameEnginePro.h"
#include <time.h>

namespace GameMenu {
	void InitMenu();
	void WaitChoice();
	void outChat(const char outString[]);
	void WaitChatChoice();
}
#endif // GAMEMENU_H