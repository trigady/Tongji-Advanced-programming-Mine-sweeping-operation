/****************************************
GameEngine.h
Author: Root
Date: 2019/12/09
Description:
�����˳�����Ϸģ���ͷ�ļ�����GameEngine�����ռ��ֹͬ����Ⱦ
������˵����
void InitGame()	   ��ʼ����Ϸ
void Play()		   ��ʼ��Ϸ
void DestroyGame() ������Ϸ
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