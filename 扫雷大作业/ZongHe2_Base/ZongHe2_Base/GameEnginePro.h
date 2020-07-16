/****************************************
GameEnginePro.h
Author: 1950000 һ����
Date: 2019/12/29
Description:
�����˳���Ť����Ϸģ���ͷ�ļ�����GameEnginePro�����ռ��ֹͬ����Ⱦ
������˵����
void InitGame()	   ��ʼ����Ϸ
void Play()		   ��ʼ��Ϸ
void DestroyGame() ������Ϸ
*****************************************/
#pragma once
#ifndef GAMEENGINEPRO_H
#define GAMEENGINE_H
#include "Graphic.h"
#include "Controller.h"
#include "GameMenu.h"
#include <time.h>
namespace GameEnginePro {
	void InitGame(int getWidth, int getHeight, int getBoom);
	void Play();
	void DestroyGame();
}
#endif // GAMEENGINEPRO_H
