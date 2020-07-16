/****************************************
Graphic.h
Author: Root
Date: 2019/12/09
Description:
定义了渲染框架的头文件，用Graphic命名空间防止同名污染
各函数说明：
void MovePos()  将当前坐标更新为入参
void ModChar()  将当前位置的字符改为新的newChar
void ModColor() 将当前位置的字符的颜色/背景(具体修改哪个由mode指定)改为新的颜色值
void PutChar()  将当前位置放置一个outChar
void PutCharWithColor()	  将当前位置放置一个outChar，并设置为指定的颜色
void PutString()		  将当前位置放置一个字符串outString
void PutStringWithColor() 将当前位置放置一个字符串outString，并设置颜色
void DrawRect()			  将当前位置绘制一个长为width，宽为height的矩形
void DrawRectWithColor()  将当前位置绘制一个长为width，宽为height的矩形，并带颜色
void ClearScreen()		  强制清屏操作
void InitGraphic()		  初始化图形操作，将屏幕长宽设置为width和height
void DestroyGraphic()	  销毁操作
void Update()			  将所有操作更新至屏幕，也就是说在这里做的所有操作都不是实时的，只有调用这个函数后屏幕才会真正的更新
*****************************************/
#pragma once
#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <stdio.h>
#include <windows.h>

namespace Graphic {
	void MovePos(SHORT x, SHORT y);
	void ModChar(const char newChar);
	void ModColor(DWORD mode, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void PutChar(const char outChar);
	void PutCharWithColor(const char outChar, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void PutString(const char* outString);
	void PutStringWithColor(const char* outString, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void DrawRect(const SHORT width, const SHORT height);
	void DrawRectWithColor(const SHORT width, const SHORT height, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void ClearScreen();
	void InitGraphic(int width, int height);
	void DestroyGraphic();
	void Update();
}
#endif