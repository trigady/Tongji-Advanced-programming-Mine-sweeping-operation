/****************************************
GameMenu.h
Author: Root
Date: 2019/12/09
Description:
�����˳���˵�����Ⱦ/����ģ���ͷ�ļ�����GameMenu�����ռ��ֹͬ����Ⱦ
������˵����
void InitMenu() 	 ��ʼ���˵�������Ⱦ�˵�ѡ��
void WaitChoice()	 �ȴ��û����루ѡ��˵��ȣ�
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