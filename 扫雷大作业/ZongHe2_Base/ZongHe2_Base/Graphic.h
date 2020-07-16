/****************************************
Graphic.h
Author: Root
Date: 2019/12/09
Description:
��������Ⱦ��ܵ�ͷ�ļ�����Graphic�����ռ��ֹͬ����Ⱦ
������˵����
void MovePos()  ����ǰ�������Ϊ���
void ModChar()  ����ǰλ�õ��ַ���Ϊ�µ�newChar
void ModColor() ����ǰλ�õ��ַ�����ɫ/����(�����޸��ĸ���modeָ��)��Ϊ�µ���ɫֵ
void PutChar()  ����ǰλ�÷���һ��outChar
void PutCharWithColor()	  ����ǰλ�÷���һ��outChar��������Ϊָ������ɫ
void PutString()		  ����ǰλ�÷���һ���ַ���outString
void PutStringWithColor() ����ǰλ�÷���һ���ַ���outString����������ɫ
void DrawRect()			  ����ǰλ�û���һ����Ϊwidth����Ϊheight�ľ���
void DrawRectWithColor()  ����ǰλ�û���һ����Ϊwidth����Ϊheight�ľ��Σ�������ɫ
void ClearScreen()		  ǿ����������
void InitGraphic()		  ��ʼ��ͼ�β���������Ļ��������Ϊwidth��height
void DestroyGraphic()	  ���ٲ���
void Update()			  �����в�����������Ļ��Ҳ����˵�������������в���������ʵʱ�ģ�ֻ�е��������������Ļ�Ż������ĸ���
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