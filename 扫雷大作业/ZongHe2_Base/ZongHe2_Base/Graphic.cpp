/****************************************
GameMenu.cpp
Author: Root
Date: 2019/12/09
Description:
定义了渲染框架，用Graphic命名空间防止同名污染
！*！注意：除非你能完全读懂内部都在做什么操作，否则不建议修改这个文件！*！
*****************************************/
#include "Graphic.h"

namespace Graphic {
	HANDLE hStdOut = NULL;				   // 标准输出句柄
	UCHAR* pCanvas = nullptr;			   // 画版数组指针
	UCHAR* updateQueue = nullptr;		   // 更新队列数组指针
	SHORT nowPos[2] = { 0 };			   // 当前位置坐标数组
	int screenWidth = 0, screenHeight = 0; // 屏幕宽高
	char* outputSequence = nullptr;		   // 输出屏幕字符串序列指针
	size_t sequenceLength = 0;			   // 字符串序列的当前长度
	size_t sequenceCapacity = 0;		   // 字符串序列的容量
	size_t needUpdate = 0;				   // 需要更新的数量

	const DWORD MOD_FRONT = 0, MOD_BACK = 1, MOD_BOTH = 2;  // 几个常量
	
	/****************************************
	Function:  MovePos(SHORT x, SHORT y)
	Parameter: SHORT x, SHORT y
	Return:    None(void)
	Description:
	将当前坐标更新为入参
	*****************************************/
	void MovePos(SHORT x, SHORT y) {
		nowPos[0] = x; nowPos[1] = y;
	}

	/****************************************
	Function:  movePos(SHORT x, SHORT y)
	Parameter: SHORT x, SHORT y
	Return:    None(void)
	Description:
	该函数不作导出，在内部将控制台的光标移动至(x, y)
	*****************************************/
	void movePos(SHORT x, SHORT y) {
		SetConsoleCursorPosition(hStdOut, { x, y });
	}

	/****************************************
	Function:  pushUpdateQueue(const int screenIndex)
	Parameter: int screenIndex
	Return:    None(void)
	Description:
	将要更新的屏幕坐标值推入需要更新的队列
	*****************************************/
	void pushUpdateQueue(const int screenIndex) {
		// 若没有超更新队列的容量，则推入队列
		if (needUpdate < 512) {
			// 确认更新队列当前位置下标的常量
			const size_t updateIndex = needUpdate * 9;

			// 分别设置队列的各个值
			updateQueue[updateIndex] = pCanvas[screenIndex];
			updateQueue[updateIndex + 1] = (UCHAR)nowPos[1] + 1;
			updateQueue[updateIndex + 2] = (UCHAR)nowPos[0] + 1;
			updateQueue[updateIndex + 3] = pCanvas[screenIndex + 1];
			updateQueue[updateIndex + 4] = pCanvas[screenIndex + 2];
			updateQueue[updateIndex + 5] = pCanvas[screenIndex + 3];
			updateQueue[updateIndex + 6] = pCanvas[screenIndex + 4];
			updateQueue[updateIndex + 7] = pCanvas[screenIndex + 5];
			updateQueue[updateIndex + 8] = pCanvas[screenIndex + 6];
		}
		needUpdate++;
	}

	/****************************************
	Function:  ModChar(const char newChar)
	Parameter: char newChar
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)的字符改为新的newChar
	*****************************************/
	void ModChar(const char newChar) {
		const int screenIndex = ((int)nowPos[1] * screenWidth + (int)nowPos[0]) * 7;
		pCanvas[screenIndex] = newChar;

		pushUpdateQueue(screenIndex);
	}

	/****************************************
	Function:  ModColor(DWORD mode, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB)
	Parameter: DWORD mode, BYTE textR, textG, textB, backR, backG, backB
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)的字符的颜色/背景(具体修改哪个由mode指定)改为新的颜色值
	*****************************************/
	void ModColor(DWORD mode, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB) {
		const int screenIndex = ((int)nowPos[1] * screenWidth + (int)nowPos[0]) * 7; // 确定当前坐标的数组中的下标
		switch (mode) {
		case MOD_BOTH:
			// 更新前景色与背景色，但不break，switch语句将继续执行
			pCanvas[screenIndex + 1] = textR;
			pCanvas[screenIndex + 2] = textG;
			pCanvas[screenIndex + 3] = textB;
		case MOD_BACK:
			// 更新背景色
			pCanvas[screenIndex + 4] = backR;
			pCanvas[screenIndex + 5] = backG;
			pCanvas[screenIndex + 6] = backB;
			break;
		case MOD_FRONT:
			// 更新前景色
			pCanvas[screenIndex + 1] = textR;
			pCanvas[screenIndex + 2] = textG;
			pCanvas[screenIndex + 3] = textB;
		}
		pushUpdateQueue(screenIndex);
	}

	/****************************************
	Function:  PutChar(const char outChar)
	Parameter: char outChar
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)放置一个outChar
	*****************************************/
	void PutChar(const char outChar) {
		const int screenIndex = ((int)nowPos[1] * screenWidth + (int)nowPos[0]) * 7; // 确定当前坐标的数组中的下标
		pCanvas[screenIndex] = outChar;  // 设置字符

		pCanvas[screenIndex + 1] = 255;  // 设置前景色
		pCanvas[screenIndex + 2] = 255;
		pCanvas[screenIndex + 3] = 255;
		pCanvas[screenIndex + 4] = 0;	 // 设置背景色
		pCanvas[screenIndex + 5] = 0;
		pCanvas[screenIndex + 6] = 0;

		pushUpdateQueue(screenIndex);	 // 推入更新队列
	}

	/****************************************
	Function:  PutCharWithColor(const char outChar, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB)
	Parameter: char outChar, BYTE textR, textG, textB, backR, backG, backB
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)放置一个outChar，并设置为指定的颜色
	*****************************************/
	void PutCharWithColor(const char outChar, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB) {
		const int screenIndex = ((int)nowPos[1] * screenWidth + (int)nowPos[0]) * 7; // 确定当前坐标的数组中的下标
		pCanvas[screenIndex] = outChar;    // 设置字符

		pCanvas[screenIndex + 1] = textR;  // 设置前景色
		pCanvas[screenIndex + 2] = textG;
		pCanvas[screenIndex + 3] = textB;
		pCanvas[screenIndex + 4] = backR;  // 设置背景色
		pCanvas[screenIndex + 5] = backG;
		pCanvas[screenIndex + 6] = backB;

		pushUpdateQueue(screenIndex);	   // 推入更新队列
	}

	/****************************************
	Function:  PutString(const char* outString)
	Parameter: char* outString
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)放置一个字符串outString
	*****************************************/
	void PutString(const char* outString) {
		// 检查字符串是否超过屏幕宽度，如果超过则退出操作
		const size_t stringLength = strlen(outString);
		if (stringLength + (size_t)nowPos[0] > (size_t)screenWidth) {
			return;
		}
		// 若不操作，则逐字符的设置字符串
		for (size_t i = 0; i < stringLength; i++) {
			PutChar(outString[i]);
			nowPos[0]++;
		}
	}

	/****************************************
	Function:  PutStringWithColor(const char* outString, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB)
	Parameter: char* outString, BYTE textR, textG, textB, backR, backG, backB
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)放置一个字符串outString，并设置颜色，功能基本同上
	*****************************************/
	void PutStringWithColor(const char* outString, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB) {
		const size_t stringLength = strlen(outString);
		if (stringLength + (size_t)nowPos[0] > (size_t)screenWidth) {
			return;
		}
		for (size_t i = 0; i < stringLength; i++) {
			PutCharWithColor(outString[i], textR, textG, textB, backR, backG, backB);
			nowPos[0]++;
		}
	}

	/****************************************
	Function:  DrawRect(const SHORT width, const SHORT height)
	Parameter: SHORT width, SHORT height
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)绘制一个长为width，宽为height的矩形
	*****************************************/
	void DrawRect(const SHORT width, const SHORT height) {
		const SHORT x = nowPos[0], y = nowPos[1];
		// 检查长宽是否超过屏幕
		if (x + width > (SHORT)screenWidth || nowPos[1] + height > (SHORT)screenHeight) {
			return;
		}
		for (SHORT i = x; i < x + width; i++) {
			for (SHORT j = y; j < y + height; j++) {
				// 先设置位置，然后放置一个带颜色的空白
				MovePos(i, j);
				PutCharWithColor(' ', 255, 255, 255, 255, 255, 255);
			}
		}
		nowPos[0] = x; nowPos[1] = y;
	}

	/****************************************
	Function:  DrawRectWithColor(const SHORT width, const SHORT height, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB)
	Parameter: SHORT width, height, BYTE textR, textG, textB, backR, backG, backB
	Return:    None(void)
	Description:
	将当前位置(nowPos数组所指示的位置)绘制一个长为width，宽为height的矩形，并带颜色，操作基本相同
	*****************************************/
	void DrawRectWithColor(const SHORT width, const SHORT height, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB) {
		const SHORT x = nowPos[0], y = nowPos[1];
		if (x + width > (SHORT)screenWidth || nowPos[1] + height > (SHORT)screenHeight) {
			return;
		}
		for (SHORT i = x; i < x + width; i++) {
			for (SHORT j = y; j < y + height; j++) {
				MovePos(i, j);
				PutCharWithColor(' ', textR, textG, textB, backR, backG, backB);
			}
		}
		nowPos[0] = x; nowPos[1] = y;
	}

	/****************************************
	Function:  ClearScreen()
	Parameter: None(void)
	Return:    None(void)
	Description:
	强制清屏操作
	*****************************************/
	void ClearScreen() {
		memset(pCanvas, 0, screenWidth * screenHeight * 7 * sizeof(UCHAR)); // 清空画板数组
		memset(updateQueue, 0, 512 * 9 * sizeof(UCHAR));					// 清空队列数组
		needUpdate = 999;													// 将更新值设置超出容量的数量以应用强制更新
		Update();															// 更新屏幕
	}

	/****************************************
	Function:  InitGraphic(int width, int height)
	Parameter: int width, height
	Return:    None(void)
	Description:
	初始化图形操作，将屏幕长宽设置为width和height
	*****************************************/
	void InitGraphic(int width, int height) {
		screenWidth = width * 2; screenHeight = height;					  // 设置长宽
		pCanvas = new UCHAR[width * height * 14];						  // 设置画板数组，动态申请
		updateQueue = new UCHAR[512 * 9];								  // 设置更新队列数组，动态申请
		outputSequence = new char[(size_t)width * (size_t)height * 128];  // 设置输出字符串序列数组，动态申请
		sequenceCapacity = (int)width * (int)height * 128;				  // 设置字符串序列的容量

		system("chcp 936");												  // 更新控制台代码页

		// 将控制台设置为这个长宽
		char cmd_buffer[32] = "mode con: cols=0000 lines=0000";
		cmd_buffer[15] = '0' + (width / 1000 % 10);
		cmd_buffer[16] = '0' + (width / 100 % 10);
		cmd_buffer[17] = '0' + (width / 10 % 10);
		cmd_buffer[18] = '0' + width % 10;

		cmd_buffer[26] = '0' + (height / 1000 % 10);
		cmd_buffer[27] = '0' + (height / 100 % 10);
		cmd_buffer[28] = '0' + (height / 10 % 10);
		cmd_buffer[29] = '0' + height % 10;

		system(cmd_buffer);

		// 隐藏光标
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取标准输出句柄
		CONSOLE_CURSOR_INFO cci;
		cci.bVisible = 0;
		cci.dwSize = 1;
		SetConsoleCursorInfo(hStdOut, &cci);
	}

	/****************************************
	Function:  DestroyGraphic()
	Parameter: None(void)
	Return:    None(void)
	Description:
	销毁操作，将动态初始化的数组回收
	*****************************************/
	void DestroyGraphic() {
		delete[] pCanvas;
		delete[] updateQueue;
		delete[] outputSequence;
	}

	/****************************************
	Function:  constructUpdateSequence()
	Parameter: None(void)
	Return:    None(void)
	Description:
	构建更新操作的字符串序列，注意：这里面的内容需要Win10系统且已打上2017年秋季创意者更新，理论上目前电脑都已经打过这个补丁
	若操作无效，请咨询助教领取另一份Graphic文件使用旧的版本
	另本函数没有办法注释清楚，若需要理解，请访问下列网站
	https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	*****************************************/
	void constructUpdateSequence() {
		sequenceLength = 0;
		UCHAR oldColor[8] = { 0 };
		UCHAR oldY = 255;
		memset(oldColor, 33, 8);
		for (size_t i = 0; i < needUpdate; i++) {
			const size_t queueIndex = i * 9;
			if (memcmp(oldColor, updateQueue + queueIndex + 3, 6) == 0) {
				if (oldY != updateQueue[queueIndex + 1]) {
					char sequence[16] = "\x1b[000;000H ";

					sequence[10] = updateQueue[queueIndex];
					if (sequence[10] == '\0') sequence[10] = ' ';

					sequence[2 + 2] = '0' + updateQueue[queueIndex + 1] % 10;
					sequence[2 + 1] = '0' + updateQueue[queueIndex + 1] / 10 % 10;
					sequence[2] = '0' + updateQueue[queueIndex + 1] / 100;

					sequence[6 + 2] = '0' + updateQueue[queueIndex + 2] % 10;
					sequence[6 + 1] = '0' + updateQueue[queueIndex + 2] / 10 % 10;
					sequence[6] = '0' + updateQueue[queueIndex + 2] / 100;

					memcpy_s(outputSequence + sequenceLength, sequenceCapacity, sequence, 11);
					sequenceLength += 11;

					oldY = updateQueue[queueIndex + 1];
				}
				else {
					outputSequence[sequenceLength] = updateQueue[queueIndex];
					if (outputSequence[sequenceLength] == '\0') {
						outputSequence[sequenceLength] = ' ';
					}
					sequenceLength++;
				}
			}
			else {
				char sequence[64] = "\x1b[000;000H\x1b[38;2;000;000;000m\x1b[48;2;000;000;000m ";//\x1b[0m

				sequence[48] = updateQueue[queueIndex];
				if (sequence[48] == '\0')sequence[48] = ' ';

				sequence[2 + 2] = '0' + updateQueue[queueIndex + 1] % 10;
				sequence[2 + 1] = '0' + updateQueue[queueIndex + 1] / 10 % 10;
				sequence[2] = '0' + updateQueue[queueIndex + 1] / 100;

				sequence[6 + 2] = '0' + updateQueue[queueIndex + 2] % 10;
				sequence[6 + 1] = '0' + updateQueue[queueIndex + 2] / 10 % 10;
				sequence[6] = '0' + updateQueue[queueIndex + 2] / 100;

				sequence[17 + 2] = '0' + updateQueue[queueIndex + 3] % 10;
				sequence[17 + 1] = '0' + updateQueue[queueIndex + 3] / 10 % 10;
				sequence[17] = '0' + updateQueue[queueIndex + 3] / 100;

				sequence[21 + 2] = '0' + updateQueue[queueIndex + 4] % 10;
				sequence[21 + 1] = '0' + updateQueue[queueIndex + 4] / 10 % 10;
				sequence[21] = '0' + updateQueue[queueIndex + 4] / 100;

				sequence[25 + 2] = '0' + updateQueue[queueIndex + 5] % 10;
				sequence[25 + 1] = '0' + updateQueue[queueIndex + 5] / 10 % 10;
				sequence[25] = '0' + updateQueue[queueIndex + 5] / 100;

				sequence[36 + 2] = '0' + updateQueue[queueIndex + 6] % 10;
				sequence[36 + 1] = '0' + updateQueue[queueIndex + 6] / 10 % 10;
				sequence[36] = '0' + updateQueue[queueIndex + 6] / 100;

				sequence[40 + 2] = '0' + updateQueue[queueIndex + 7] % 10;
				sequence[40 + 1] = '0' + updateQueue[queueIndex + 7] / 10 % 10;
				sequence[40] = '0' + updateQueue[queueIndex + 7] / 100;

				sequence[44 + 2] = '0' + updateQueue[queueIndex + 8] % 10;
				sequence[44 + 1] = '0' + updateQueue[queueIndex + 8] / 10 % 10;
				sequence[44] = '0' + updateQueue[queueIndex + 8] / 100;

				memcpy_s(outputSequence + sequenceLength, sequenceCapacity, sequence, 49);
				sequenceLength += 49;

				memcpy_s(oldColor, 8, updateQueue + queueIndex + 3, 6);
				oldY = updateQueue[queueIndex + 1];
			}
		}
		outputSequence[sequenceLength] = '\0';
	}

	/****************************************
	Function:  constructScreenSequence()
	Parameter: None(void)
	Return:    None(void)
	Description:
	更新全屏幕的字符串，无法注释，若需理解请参考上述网站
	*****************************************/
	void constructScreenSequence() {
		sequenceLength = 0;
		UCHAR oldColor[8] = { 0 };
		memset(oldColor, 33, 8);
		for (size_t i = 0; i < (size_t)screenHeight; i++) {
			char posSequence[16] = "\x1b[001;001H";
			posSequence[2 + 2] = '0' + (i + 1) % 10;
			posSequence[2 + 1] = '0' + (i + 1) / 10 % 10;
			posSequence[2] = '0' + (char)(i + 1) / 100;

			memcpy_s(outputSequence + sequenceLength, sequenceCapacity, posSequence, 10);
			sequenceLength += 10;

			for (size_t j = 0; j < (size_t)screenWidth; j++) {
				const size_t screenIndex = (i * screenWidth + j) * 7;
				if (j > (size_t)screenWidth / 2 && pCanvas[screenIndex] == 0) {
					break;
				}
				if (memcmp(oldColor, pCanvas + screenIndex + 1, 6) == 0) {
					outputSequence[sequenceLength] = pCanvas[screenIndex];
					if (outputSequence[sequenceLength] == '\0') {
						outputSequence[sequenceLength] = ' ';
					}
					sequenceLength++;
				}
				else {
					char sequence[64] = "\x1b[38;2;000;000;000m\x1b[48;2;000;000;000m ";//\x1b[0m
					sequence[38] = pCanvas[screenIndex];
					if (sequence[38] == '\0')sequence[38] = ' ';

					sequence[7 + 2] = '0' + pCanvas[screenIndex + 1] % 10;
					sequence[7 + 1] = '0' + pCanvas[screenIndex + 1] / 10 % 10;
					sequence[7] = '0' + pCanvas[screenIndex + 1] / 100;

					sequence[11 + 2] = '0' + pCanvas[screenIndex + 2] % 10;
					sequence[11 + 1] = '0' + pCanvas[screenIndex + 2] / 10 % 10;
					sequence[11] = '0' + pCanvas[screenIndex + 2] / 100;

					sequence[15 + 2] = '0' + pCanvas[screenIndex + 3] % 10;
					sequence[15 + 1] = '0' + pCanvas[screenIndex + 3] / 10 % 10;
					sequence[15] = '0' + pCanvas[screenIndex + 3] / 100;

					sequence[26 + 2] = '0' + pCanvas[screenIndex + 4] % 10;
					sequence[26 + 1] = '0' + pCanvas[screenIndex + 4] / 10 % 10;
					sequence[26] = '0' + pCanvas[screenIndex + 4] / 100;

					sequence[30 + 2] = '0' + pCanvas[screenIndex + 5] % 10;
					sequence[30 + 1] = '0' + pCanvas[screenIndex + 5] / 10 % 10;
					sequence[30] = '0' + pCanvas[screenIndex + 5] / 100;

					sequence[34 + 2] = '0' + pCanvas[screenIndex + 6] % 10;
					sequence[34 + 1] = '0' + pCanvas[screenIndex + 6] / 10 % 10;
					sequence[34] = '0' + pCanvas[screenIndex + 6] / 100;

					memcpy_s(outputSequence + sequenceLength, sequenceCapacity, sequence, 39);
					sequenceLength += 39;

					memcpy_s(oldColor, 8, pCanvas + screenIndex + 1, 6);
				}
			}
		}
		outputSequence[sequenceLength] = '\0';
	}

	/****************************************
	Function:  Update()
	Parameter: None(void)
	Return:    None(void)
	Description:
	将所有操作更新至屏幕，也就是说在这里做的所有操作都不是实时的，只有调用这个函数后屏幕才会真正的更新
	*****************************************/
	void Update() {
		movePos(0, 0);		 // 将光标移动至0,0，硬移动

		if (needUpdate < 512) {
			constructUpdateSequence();  // 如果要更新的数量小于512个，那么则构建更新序列
		}
		else {
			constructScreenSequence();	// 否则构建全屏幕更新序列
		}
		needUpdate = 0;					// 重置更新数量

		// 将所有操作写入到控制台窗口屏幕
		DWORD dWrite = 0;
		WriteConsoleA(hStdOut, outputSequence, sequenceLength, &dWrite, NULL);
	}
}