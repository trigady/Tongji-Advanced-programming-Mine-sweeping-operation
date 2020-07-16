/****************************************
Controller.cpp
Author: Root
Date: 2019/12/09
Description:
定义了控制器框架，用Controller命名空间防止同名污染
！*！注意：除非你能完全读懂内部都在做什么操作，否则不建议修改这个文件！*！
*****************************************/
#include "Controller.h"

namespace Controller {
	COORD pos = { 0 };                   // 鼠标坐标
	HANDLE hStdIn = NULL;				 // 标准输入句柄
	INPUT_RECORD inRecArr[256] = { 0 };	 // 输入缓冲区数组
	INPUT_RECORD inRec = { 0 };			 // 最终需要获取的输入
	DWORD dRead = 0;					 // 读取到的输入缓冲区长度
	char key = '0';						 // 键盘按键
	const DWORD MOVE = 0, LEFT_CLICK = 1, RIGHT_CLICK = 2, _DOUBLE_CLICK = 3; // 一些常量

	/****************************************
	Function:  updateInput(DWORD type, DWORD hitMode = MOVE)
	Parameter: DWORD type, hitMode
	Return:    bool
	Description:
	将inRec变量设置为需要获取的输入类型type(鼠标或键盘)，如果找到需要的输入，则返回true并设置inRec变量，否则返回false
	hitMode将指定鼠标的点击模式，是左键单击/右键单击或双击
	*****************************************/
	bool updateInput(DWORD type, DWORD hitMode = MOVE) {
		// 从输入缓冲区的最后一个数据开始循环搜索
		for (int i = (int)dRead; i > 0; i--) {
			// 如果这个数据的类型符合输入
			if (inRecArr[i - 1].EventType == type) {
				// 检查hitMode 如果是MOVE则跳过switch
				switch (hitMode) {
				case LEFT_CLICK:
					// 左键单击
					if (inRecArr[i - 1].Event.MouseEvent.dwButtonState != 1) {
						continue;
					}
					break;
				case RIGHT_CLICK:
					// 右键单击
					if (inRecArr[i - 1].Event.MouseEvent.dwButtonState != RIGHTMOST_BUTTON_PRESSED) {
						continue;
					}
					break;
				case _DOUBLE_CLICK:
					// 双击
					if (inRecArr[i - 1].Event.MouseEvent.dwEventFlags != DOUBLE_CLICK) {
						continue;
					}
					break;
				}
				// 设置inRec并返回true表示找到需要的输入
				inRec = inRecArr[i - 1];
				return true;
			}
		}
		return false;
	}

	/****************************************
	Function:  FlushInput()
	Parameter: None(void)
	Return:    None(void)
	Description:
	刷新输入缓冲区读取缓冲区的全部数据
	*****************************************/
	void FlushInput() {
		if (WaitForSingleObject(hStdIn, 0) == WAIT_OBJECT_0) {
			// 如果缓冲区内有数据，则读出数据，最大256个
			ReadConsoleInputA(hStdIn, &inRecArr[0], 256, &dRead);
		}
		else {
			// 否则将Read置为0
			dRead = 0;
		}
	}

	/****************************************
	Function:  GetKeyHit()
	Parameter: None(void)
	Return:    char
	Description:
	获取键盘输入，如果没有键盘击中，则返回'\0'
	*****************************************/
	char GetKeyHit() {
		if (updateInput(KEY_EVENT)) {
			if (inRec.Event.KeyEvent.bKeyDown) {
				key = (char)inRec.Event.KeyEvent.wVirtualKeyCode;
				return key;
			}
		}
		return '\0';
	}

	/****************************************
	Function:  GetCursorPos()
	Parameter: None(void)
	Return:    COORD
	Description:
	获取鼠标输入，将一直返回上一次更新的坐标
	*****************************************/
	COORD GetCursorPos() {
		if (updateInput(MOUSE_EVENT)) {
			pos = inRec.Event.MouseEvent.dwMousePosition;
		}
		return pos;
	}

	/****************************************
	Function:  GetCursorHitPos()
	Parameter: DWORD type
	Return:    COORD
	Description:
	获取鼠标点击输入，type将指定是左键单击(1)右键单击(2)双击(3)，若没有点击，则将一直返回0,0
	*****************************************/
	COORD GetCursorHitPos(DWORD type) {
		if (updateInput(MOUSE_EVENT, type)) {
			return inRec.Event.MouseEvent.dwMousePosition;
		}
		return { 0,0 };
	}

	/****************************************
	Function:  InitController()
	Parameter: None(void)
	Return:    None(void)
	Description:
	初始化控制器
	*****************************************/
	void InitController() {
		hStdIn = GetStdHandle(STD_INPUT_HANDLE); // 获取标准输入句柄
		SetConsoleMode(hStdIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);  // 将控制台设置为允许鼠标输入
	}
}