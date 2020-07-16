/****************************************
GameEngine.cpp
Author: Root
Date: 2019/12/09
Description:
定义了程序游戏模块，用GameEngine命名空间防止同名污染，是你主要需要修改的文件
*****************************************/
#include "GameEngine.h"

namespace GameEngine {
	// 引入两个命名空间
	using namespace Graphic;
	using namespace Controller;

	size_t mapWidth = 0, mapHeight = 0;				 // 地图宽度和高度
	UCHAR mapCanvas[50 * 25];						 // 地图数组指针，若做基础项请不要直接尝试操作这个指针
	COORD pos = { 0 }, posOld = { 0 };				 // 鼠标的位置
	COORD posChoice = { 0 }, posChoiceOld = { 0 };	 // 选中地图坐标的位置
	char key = '\0';								 // 键盘的按键
	bool gameFlag = false;							 // 游戏运行状态
	size_t frame = 0;								 // 已渲染帧数
	clock_t tic = clock();							 // 游戏开始时刻
	int operation = 0;								 // 执行的操作
	bool isFirst = true;							 // 是否为第一步
	//int mapArray[50][50];							 // 这里直接操作mapCanvas，不使用mapArray数组

	// 如果你有新加的变量，建议加在下方和预置变量做区别
	int boomTotal = 0;								 // 定义雷的总数
	int safeZone = 0;								 // 定义不可能为雷的安全区的中心位置
	UCHAR mapVisible[50 * 25];					 // 定义当前可见的地图信息
	int remainBlock = 0;							 // 定义未点开的砖块数
	int flagNum = 0;								 // 定义已经插旗的数目
	int alreadySlice = 1;							 // 定义已走的步数
	double timeStart, timeNow, timeEnd = 1000;		 // 定义开始时的时间


	/****************************************
	Function:  renderMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	绘制当前地图
	*****************************************/
	void renderMap() {

		// 在0,0处放置当前选择位置的字符串，注意结尾有空格留白，可以思考为什么要加这么多空格
		MovePos(0, 1);
		char strPos[32] = "";
		sprintf_s(strPos, "当前选择(%u, %u)         ", posChoice.X, posChoice.Y);
		PutString(strPos);
		Update();

		// 在0,20处放置字符串，显示时间
		MovePos(20, 1);
		char strTime[32] = "";
		if (isFirst) {
			sprintf_s(strTime, "已用时间：%u         ", 0);
		}
		else {
			sprintf_s(strTime, "已用时间：%u         ", (int)(timeEnd - timeStart) / 1000);
		}
		PutString(strTime);
		Update();

		// 在1,0处放置字符串，显示剩余雷数
		MovePos(0, 2);
		char strBoom[32] = "";
		sprintf_s(strBoom, "剩余雷数：%d         ", boomTotal - flagNum);
		PutString(strBoom);
		Update();

		// 在1,20处放置字符串，显示已走步数
		MovePos(20, 2);
		char strSlice[32] = "";
		sprintf_s(strSlice, "已走步数：%u         ", (isFirst ? 0 : alreadySlice));
		PutString(strSlice);
		Update();

		// 从行开始遍历
		for (size_t i = 0; i < mapHeight + 1; i++) {
			// 移动坐标至行首 并绘制行首的字符
			MovePos(0, (SHORT)i * 2 + 3);
			if (i == 0) {
				PutString("╔");
			}
			else if (i == mapHeight) {
				PutString("╚");
			}
			else {
				PutString("╠");
			}

			// 遍历列，绘制边界
			for (size_t j = 0; j < mapWidth; j++) {
				// 移动至确切的坐标，绘制方格的边界
				MovePos(2 + (SHORT)j * 8, (SHORT)i * 2 + 3);
				if (i == 0) {
					if (j < mapWidth - 1) {
						PutString("═══╦");
					}
					else {
						PutString("═══╗");
					}
				}
				else if (i == mapHeight) {
					if (j < mapWidth - 1) {
						PutString("═══╩");
					}
					else {
						PutString("═══╝");
					}
				}
				else {
					if (j < mapWidth - 1) {
						PutString("═══╬");
					}
					else {
						PutString("═══╣");
					}
				}
			}
			// 绘制地雷地图
			if (i > 0 && i < mapHeight + 1) {
				// 移动至行首，绘制边界字符
				MovePos(0, (SHORT)i * 2 + 2);
				PutString("║");
				// 遍历列 绘制地雷
				for (size_t j = 0; j < mapWidth; j++) {
					MovePos(2 + (SHORT)j * 5, (SHORT)i * 2 + 2);    // 移动至确切坐标
					const size_t mapIndex = (i - 1) * mapWidth + j; // 确定地图数组的下标
					char numMap[8] = "   ";							// 确定数字字符串
					numMap[1] = '0' + mapCanvas[mapIndex];			// 当mapVisible[mapIndex]为1到8时，将其转换成字符串
					switch (mapVisible[mapIndex]) {
					case 0:
						switch (mapCanvas[mapIndex]) {
						case 0:
							// 0的时候放置空白
							PutString("   ");
							break;
						case 1:
							// 从1开始绘制数字 带颜色
							PutStringWithColor(numMap, 30, 144, 255, 0, 0, 0);
							break;
						case 2:
							PutStringWithColor(numMap, 0, 255, 127, 0, 0, 0);
							break;
						case 3:
							PutStringWithColor(numMap, 255, 48, 48, 0, 0, 0);
							break;
						case 4:
							PutStringWithColor(numMap, 72, 61, 139, 0, 0, 0);
							break;
						case 5:
							PutStringWithColor(numMap, 255, 105, 180, 0, 0, 0);
							break;
						case 6:
							PutStringWithColor(numMap, 148, 0, 211, 0, 0, 0);
							break;
						case 7:
							PutStringWithColor(numMap, 139, 0, 0, 0, 0, 0);
							break;
						case 8:
							PutStringWithColor(numMap, 139, 34, 82, 0, 0, 0);
							break;

						case 11:
							// 11为地雷
							PutStringWithColor(" ⊙", 255, 215, 0, 0, 0, 0);
							break;
						}
						break;
					case 9:
						// 9为砖块
						PutString(" ▇");
						break;
					case 10:
						// 10为旗帜
						PutStringWithColor(" ☆", 255, 255, 255, 0, 0, 0);
						break;
					case 12:
						//12为暂定
						PutStringWithColor(" ？", 255, 255, 255, 0, 0, 0);
						break;
					}

					MovePos(5 + (SHORT)j * 5, (SHORT)i * 2 + 2);
					PutString("║");
				}
			}
		}
		Update(); // 将地图更新到屏幕
	}

	/****************************************
	Function:  updateMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	将二维数组的数据复制到一维数组中
	*****************************************/
	/*void updateMap() {
		memcpy_s(mapCanvas, mapWidth * mapHeight, mapArray, mapWidth * mapHeight);
	}*/

	/****************************************
	Function:  InitGame(int getWidth,int getHeight, int getBoom)
	Parameter: getWidth(int),getHeight(int),getBoom(int)
	Return:    None(void)
	Description:
	初始化游戏
	*****************************************/
	void InitGame(int getWidth,int getHeight, int getBoom) {
		ClearScreen();								 // 清屏
		mapWidth = getWidth; 
		mapHeight = getHeight;						// 设置宽高
		boomTotal = getBoom;

		for (int i = 0; i < mapWidth * mapHeight; i++) {		// 初始化两个数组
			mapCanvas[i] = 0;
			mapVisible[i] = 9;
		}
		//updateMap();	// 更新地图
		remainBlock = mapWidth * mapHeight;
		renderMap();	// 绘制地图
	}

	/****************************************
	Function:  renderChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	绘制选中的地块的背景
	*****************************************/
	void renderChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);        // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // 确定地图下标

		MovePos(2 + x * 4, y * 2 + 3);

		// 以下内容同renderMap中绘制地雷部分相同，不详做介绍
		char numMap[8] = "  ";
		numMap[0] = '0' + mapCanvas[mapIndex];
		switch (mapVisible[mapIndex]) {
		case 0:
			switch (mapCanvas[mapIndex]) {
			case 0:
				PutStringWithColor("  ", 255, 255, 255, alpha, alpha, alpha);
				break;
			case 1:
				PutStringWithColor(numMap, 30, 144, 255, alpha, alpha, alpha);
				break;
			case 2:
				PutStringWithColor(numMap, 0, 255, 127, alpha, alpha, alpha);
				break;
			case 3:
				PutStringWithColor(numMap, 255, 48, 48, alpha, alpha, alpha);
				break;
			case 4:
				PutStringWithColor(numMap, 72, 61, 139, alpha, alpha, alpha);
				break;
			case 5:
				PutStringWithColor(numMap, 255, 105, 180, alpha, alpha, alpha);
				break;
			case 6:
				PutStringWithColor(numMap, 148, 0, 211, alpha, alpha, alpha);
				break;
			case 7:
				PutStringWithColor(numMap, 139, 0, 0, alpha, alpha, alpha);
				break;
			case 8:
				PutStringWithColor(numMap, 139, 34, 82, alpha, alpha, alpha);
				break;
			case 11:
				PutStringWithColor("⊙", 255, 215, 0, alpha, alpha, alpha);
				break;
			}
			break;
		case 9:
			PutStringWithColor("▇", 255, 255, 255, alpha, alpha, alpha);
			break;
		case 10:
			PutStringWithColor("☆", 255, 255, 255, alpha, alpha, alpha);
			break;
		case 12:
			PutStringWithColor("？", 255, 255, 255, alpha, alpha, alpha);
			break;
		}
	}

	/****************************************
	Function:  clearChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	清除choice中指示位置的背景，功能同上，不详做介绍
	*****************************************/
	void clearChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x;

		MovePos(2 + x * 4, y * 2 + 3);

		char numMap[8] = "  ";
		numMap[0] = '0' + mapCanvas[mapIndex];
		switch (mapVisible[mapIndex]) {
		case 0:
			switch (mapCanvas[mapIndex]) {
			case 0:
				PutStringWithColor("  ", 255, 255, 255, 0, 0, 0);
				break;
			case 1:
				PutStringWithColor(numMap, 30, 144, 255, 0, 0, 0);
				break;
			case 2:
				PutStringWithColor(numMap, 0, 255, 127, 0, 0, 0);
				break;
			case 3:
				PutStringWithColor(numMap, 255, 48, 48, 0, 0, 0);
				break;
			case 4:
				PutStringWithColor(numMap, 72, 61, 139, 0, 0, 0);
				break;
			case 5:
				PutStringWithColor(numMap, 255, 105, 180, 0, 0, 0);
				break;
			case 6:
				PutStringWithColor(numMap, 148, 0, 211, 0, 0, 0);
				break;
			case 7:
				PutStringWithColor(numMap, 139, 0, 0, 0, 0, 0);
				break;
			case 8:
				PutStringWithColor(numMap, 139, 34, 82, 0, 0, 0);
				break;
			case 11:
				PutStringWithColor("⊙", 255, 215, 0, 0, 0, 0);
				break;
			}
			break;
		case 9:
			PutStringWithColor("▇", 255, 255, 255, 0, 0, 0);
			break;
		case 10:
			PutStringWithColor("☆", 255, 255, 255, 0, 0, 0);
			break;
		case 12:
			PutStringWithColor("？", 255, 255, 255, 0, 0, 0);
			break;
		}
	}

	/****************************************
	Function:  checkChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查输入和操作，并设置相关变量
	*****************************************/
	void checkChoice() {
		FlushInput();							// 刷新输入缓冲区
		pos = GetCursorPos();					// 获取鼠标坐标
		COORD hitLeftPos = GetCursorHitPos();	// 获取左键单击坐标
		COORD hitRightPos = GetCursorHitPos(2);	// 获取右键单击坐标
		key = GetKeyHit();						// 获取键盘输入
		operation = 0;							// 当前操作 (0无操作，1为挖开地块，2为标值旗帜，或者自己指定)

		// 检查键盘输入
		switch (key) {
		case VK_ESCAPE:
			// ESC键，将游戏运行状态置否以退出游戏
			gameFlag = false;
			break;

		// 上下左右键移动选中的坐标
		case VK_UP:
			if (posChoice.Y > 0)posChoice.Y--;
			break;
		case VK_RIGHT:
			if (posChoice.X < (SHORT)mapWidth - 1)posChoice.X++;
			break;
		case VK_DOWN:
			if (posChoice.Y < (SHORT)mapHeight - 1)posChoice.Y++;
			break;
		case VK_LEFT:
			if (posChoice.X > 0)posChoice.X--;
			break;

		// 回车和空格分别对应操作1和2
		case VK_RETURN:
			operation = 1;
			break;
		case VK_SPACE:
			operation = 2;
			break;
		}

		// 将鼠标控制台的坐标放缩至地图坐标
		const size_t mouseY = (size_t)((size_t)pos.Y + 1) / 2 - 2;
		const size_t mouseX = (size_t)((size_t)pos.X - 1) / 4;
		if (mouseY < mapHeight && mouseX < mapWidth) {
			// 若没有超边界，更新选择的坐标
			posChoice.X = (SHORT)mouseX;
			posChoice.Y = (SHORT)mouseY;
		}

		// 左键和右键分别对应操作1和2
		if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
			operation = 1;
		}
		else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
			operation = 2;
		}
	}

	/****************************************
	Function:  isInArray()
	Parameter: ins(int),arr(int[9])
	Return:    1/0 (bool)
	Description:
	判断ins是否和arr中的某个元素相等，相等的话返回true
	*****************************************/
	bool isInArray(int ins, int arr[]) {
		for (int i = 0; i < 9; i++) {
			if (ins == arr[i]) {
				return true;
			}
		}
		return false;
	}

	/****************************************
	Function:  initMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	初始化函数，请通过posChoice变量判断操作，并操作mapArray数组，请不要操作其他程序预置好的变量
	*****************************************/
	void initMap() {
		int boomCounter = 0;									//定义已经埋下雷的计数器

		int tempBomb = 0;										//定义可能为雷的工作点
		UCHAR* pin = mapCanvas;									//定义埋雷工作指针

		int safePlace[9] = { -1,-1,-1,-1,-1,-1,-1,-1,-1 };		//定义安全区具体范围
		if (safeZone >= mapWidth) {
			for (int i = 0; i < 3; i++) {
				safePlace[i] = safeZone - mapWidth - 1 + i;
			}
		}
		for (int i = 3; i < 6; i++) {
			safePlace[i] = safeZone - 4 + i;
		}
		if (safeZone < (mapHeight - 1) * mapWidth) {
			for (int i = 6; i < 9; i++) {
				safePlace[i] = safeZone + mapWidth - 7 + i;
			}
		}
		if (safeZone % mapWidth == 0) {
			for (int i = 0; i < 3; i++) {
				safePlace[3 * i] = -1;
			}
		}
		if (safeZone % mapWidth == mapWidth - 1) {
			for (int i = 0; i < 3; i++) {
				safePlace[3 * i + 2] = -1;
			}
		}

		int safeSize = 9;										//定义安全区的大小
		if (posChoice.X == 0 || posChoice.X == mapWidth - 1) {
			safeSize -= 3;
		}
		if (posChoice.Y == 0 || posChoice.Y == mapHeight - 1) {
			safeSize = (safeSize == 9 ? 6 : 4);
		}

		srand((unsigned)time(NULL));							//定义时间作为随机数种子

		for (;;) {
			pin = mapCanvas;
			tempBomb = rand() % (mapWidth * mapHeight - safeSize - boomCounter);
			for (int i = 0;;) {
				for (;;) {
					if (*pin >= 9) {				//判断没有埋雷
						pin++;
					}
					else if (isInArray((int)(pin-mapCanvas),safePlace)) {
						pin++;						//防止把雷埋在安全区
					}
					else {
						break;
					}
				}
				if (i == tempBomb) {							//这里根据埋雷的情况对周围位块进行赋值
					*pin = 11;
					if ((pin - mapCanvas) % mapWidth == 0) {
						if ((pin - mapCanvas) >= 0 && (pin - mapCanvas) < mapWidth) {
							*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
						}
						else if ((pin - mapCanvas) >= mapWidth * (mapHeight - 1) && (pin - mapCanvas) < mapHeight * mapWidth) {
							*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
						}
						else {
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
							*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
						}
					}
					else if ((pin - mapCanvas) % mapWidth == mapWidth - 1) {
						if ((pin - mapCanvas) >= 0 && (pin - mapCanvas) < mapWidth) {
							*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						}
						else if ((pin - mapCanvas) >= mapWidth * (mapHeight - 1) && (pin - mapCanvas) < mapHeight * mapWidth) {
							*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
						}
						else {
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
							*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						}
					}
					else if ((pin - mapCanvas) >= 0 && (pin - mapCanvas) < mapWidth) {
						*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
						*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
						*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
						*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
					}
					else if ((pin - mapCanvas) >= mapWidth * (mapHeight - 1) && (pin - mapCanvas) < mapHeight * mapWidth) {
						*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
						*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
						*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
						*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
						*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
					}
					else {
						*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
						*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
						*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
						*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
						*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
						*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
						*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
					}
					break;
				}
				else {
					pin++;
					i++;
				}
			}
			boomCounter++;
			if (boomCounter == boomTotal) {
				break;
			}
		}
	}

	/****************************************
	Function:  gameOver()
	Parameter: Nnoe(void)
	Return:    None(void)
	Description:
	游戏失败
	*****************************************/
	void gameOver() {
		timeEnd = GetTickCount64();
		GameMenu::outChat("很遗憾，您失败了 ");
		GameMenu::WaitChatChoice();
		ClearScreen();
		for (int i = 0; i < mapWidth * mapHeight; i++) {
			if (*(mapCanvas + i) == 11) {
				*(mapVisible + i) = 0;
			}
		}
		renderMap();
		bool quitFlag = true;					// 强制退出
		
		while (quitFlag) {
			FlushInput();							// 刷新输入缓冲区
			pos = GetCursorPos();					// 获取鼠标坐标
			COORD hitLeftPos = GetCursorHitPos();	// 获取左键单击坐标
			COORD hitRightPos = GetCursorHitPos(2);	// 获取右键单击坐标
			key = GetKeyHit();						// 获取键盘输入

			// 检查键盘输入
			if (key) {
				quitFlag = false;
			}

			// 将鼠标控制台的坐标放缩至地图坐标
			const size_t mouseY = (size_t)((size_t)pos.Y + 1) / 2 - 2;
			const size_t mouseX = (size_t)((size_t)pos.X - 1) / 4;
			if (mouseY < mapHeight && mouseX < mapWidth) {
				// 若没有超边界，更新选择的坐标
				posChoice.X = (SHORT)mouseX;
				posChoice.Y = (SHORT)mouseY;
			}

			// 左键和右键分别对应操作1和2
			if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
				quitFlag = false;
			}
			else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
				quitFlag = false;
			}
		}
		gameFlag = false;
	}

	/****************************************
	Function:  digBlock(int digPlace)
	Parameter: digPlace(int)
	Return:    None(void)
	Description:
	实现翻开操作
	*****************************************/
	void digBlock(int digPlace) {
		if (isFirst) {
			// 如果是第一步走，则先初始化地图，注意不要在落点设置一个地雷
			safeZone = posChoice.Y * mapWidth + posChoice.X;	//设立安全区
			initMap();
			isFirst = false; // 将第一步设置为否
			timeStart = GetTickCount64();
		}
		if (*(mapVisible + digPlace) == 0) {
			return;
		}
		if (*(mapCanvas + digPlace) == 11) {
			gameOver();
		}
		else if (*(mapCanvas + digPlace) >= 1 && *(mapCanvas + digPlace) <= 8) {
			*(mapVisible + digPlace) = 0;
			remainBlock--;
			renderMap();
		}
		else if (*(mapCanvas + digPlace) == 0){		//这里用函数迭代的方式翻开邻近无雷方块
			*(mapVisible + digPlace) = 0;
			remainBlock--;
			renderMap();
			if (digPlace >= mapWidth) {
				digBlock(digPlace - mapWidth);
				if (digPlace % mapWidth != mapWidth - 1) {
					digBlock(digPlace - mapWidth + 1);
				}
				if (digPlace % mapWidth != 0) {
					digBlock(digPlace - mapWidth - 1);
				}
			}
			if (digPlace < mapWidth * (mapHeight - 1)) {
				digBlock(digPlace + mapWidth);
				if (digPlace % mapWidth != mapWidth - 1) {
					digBlock(digPlace + mapWidth + 1);
				}
				if (digPlace % mapWidth != 0) {
					digBlock(digPlace + mapWidth - 1);
				}
			}
			if (digPlace % mapWidth != mapWidth - 1) {
				digBlock(digPlace + 1);
			}
			if (digPlace % mapWidth != 0) {
				digBlock(digPlace - 1);
			}
		}
	}

	/****************************************
	Function:  flagBlock(int flagPlace)
	Parameter: flagPlace(int)
	Return:    None(void)
	Description:
	实现插旗操作
	*****************************************/
	void flagBlock(int flagPlace) {
		if (*(mapVisible + flagPlace) == 0) {
			return;
		}
		switch(*(mapVisible + flagPlace)) {
		case 9:
			*(mapVisible + flagPlace) = 10;
			flagNum++;
			break;
		case 10:
			*(mapVisible + flagPlace) = 12;
			flagNum--;
			break;
		case 12:
			*(mapVisible + flagPlace) = 9;
			break;
		}
		renderMap();
	}

	/****************************************
	Function:  judgeWin()
	Parameter: None(void)
	Return:    winFlag(bool)
	Description:
	用于判断是否完全清空雷
	*****************************************/
	bool judgeWin() {
		if (remainBlock == boomTotal) {
			timeEnd = GetTickCount64();
			renderMap();
			GameMenu::outChat("恭喜您，营救成功 ");
			GameMenu::WaitChatChoice();
			ClearScreen();
			
			for (int i = 0; i < mapWidth * mapHeight; i++) {
				if (*(mapCanvas + i) == 11) {
					*(mapVisible + i) = 0;
				}
			}
			renderMap();
			
			bool quitFlag = true;					// 强制退出

			while (quitFlag) {
				FlushInput();							// 刷新输入缓冲区
				pos = GetCursorPos();					// 获取鼠标坐标
				COORD hitLeftPos = GetCursorHitPos();	// 获取左键单击坐标
				COORD hitRightPos = GetCursorHitPos(2);	// 获取右键单击坐标
				key = GetKeyHit();						// 获取键盘输入
				operation = 0;							// 当前操作 (0无操作，1为挖开地块，2为标值旗帜，或者自己指定)

				// 检查键盘输入
				if (key) {
					quitFlag = false;
				}

				// 将鼠标控制台的坐标放缩至地图坐标
				const size_t mouseY = (size_t)((size_t)pos.Y + 1) / 2 - 2;
				const size_t mouseX = (size_t)((size_t)pos.X - 1) / 4;
				if (mouseY < mapHeight && mouseX < mapWidth) {
					// 若没有超边界，更新选择的坐标
					posChoice.X = (SHORT)mouseX;
					posChoice.Y = (SHORT)mouseY;
				}

				// 左键和右键分别对应操作1和2
				if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
					quitFlag = false;
				}
				else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
					quitFlag = false;
				}
			}
			return true;
		}
		else {
			return false;
		}

	}

	/****************************************
	Function:  Play()
	Parameter: None(void)
	Return:    None(void)
	Description:
	开始游戏
	*****************************************/
	void Play() {
		gameFlag = true;
		while (gameFlag) {
			checkChoice(); // 检查输入

			// 查看当前坐标是否需要更新背景
			if (posChoice.X != posChoiceOld.X || posChoice.Y != posChoiceOld.Y) {
				clearChoiceBackground(posChoiceOld);
				posChoiceOld = posChoice;
			}
			renderChoiceBackground(posChoice);

			// 在0,0处放置当前选择位置的字符串，注意结尾有空格留白，可以思考为什么要加这么多空格
			MovePos(0, 0);
			char strPos[32] = "";
			sprintf_s(strPos, "当前选择(%u, %u)         ", posChoice.X, posChoice.Y);
			PutString(strPos);
			Update();

			// 在0,20处放置字符串，显示时间
			MovePos(20, 0);
			timeNow = GetTickCount64();
			char strTime[32] = "";
			sprintf_s(strTime, "已用时间：%u         ", (isFirst ? 0 : (int)(timeNow - timeStart) / 1000));
			PutString(strTime);
			Update();

			// 在1,0处放置字符串，显示剩余雷数
			MovePos(0, 1);
			char strBoom[32] = "";
			sprintf_s(strBoom, "剩余雷数：%d         ", boomTotal - flagNum);
			PutString(strBoom);
			Update();

			// 在1,20处放置字符串，显示已走步数
			MovePos(20, 1);
			char strSlice[32] = "";
			sprintf_s(strSlice, "已走步数：%u         ", (isFirst ? 0 : alreadySlice));
			PutString(strSlice);
			Update();

			// 执行相应操作
			switch (operation) {
			case 1:
				// 翻开地块
				if (!isFirst) {
					alreadySlice++;
				}
				digBlock(posChoice.Y * mapWidth + posChoice.X);
				operation = 0;
				if (judgeWin()) {
					gameFlag = false;
				}
				break;
			case 2:
				// 标记为地雷
				if (!isFirst) {
					alreadySlice++;
				}
				flagBlock(posChoice.Y * mapWidth + posChoice.X);
				operation = 0;
				break;
			}


			// 以下内容不建议修改 处理每帧的事务
			//updateMap(); // 更新地图画板

			Update();    // 更新操作到屏幕


			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);	    // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();						    // 更新上一次记录的时间
		}
	}

	void DestroyGame() {
		for (int i = 0; i < mapWidth * mapWidth; i++) {		// 初始化两个数组
			mapCanvas[i] = 0;
			mapVisible[i] = 9;
		}
		isFirst = true;
		remainBlock = 0;
		flagNum = 0;
		alreadySlice = 1;
	}

}