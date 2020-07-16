/****************************************
Controller.h
Author: Root
Date: 2019/12/09
Description:
�����˿��ƿ�ܵ�ͷ�ļ�����Controller�����ռ��ֹͬ����Ⱦ
������˵����
void FlushInput()	  �������뻺����
char GetKeyHit()	  ��ȡ��������
COORD GetCursorPos()  ��ȡ��굱ǰ���꣨�ڿ���̨�ڵ����꣩
COORD GetCursorHitPos(DWORD type = 1) ��ȡ��ǰ����������꣬type�������ǻ�ȡ���/�Ҽ��������˫��������
void InitController() ��ʼ��������
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