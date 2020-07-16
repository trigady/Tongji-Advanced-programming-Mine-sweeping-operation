/****************************************
Controller.h
Author: Root
Date: 2019/12/09
Description:
定义了控制框架的头文件，用Controller命名空间防止同名污染
各函数说明：
void FlushInput()	  更新输入缓冲区
char GetKeyHit()	  获取键盘输入
COORD GetCursorPos()  获取鼠标当前坐标（在控制台内的坐标）
COORD GetCursorHitPos(DWORD type = 1) 获取当前鼠标点击的坐标，type将决定是获取左键/右键还是左键双击的坐标
void InitController() 初始化控制器
*****************************************/
#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <windows.h>

namespace Controller {
	void FlushInput();
	char GetKeyHit();
	COORD GetCursorPos();
	COORD GetCursorHitPos(DWORD type = 1);
	void InitController();
}
#endif