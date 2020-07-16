/****************************************
GameMenu.cpp
Author: Root
Date: 2019/12/09
Description:
定义了程序菜单的渲染/管理模块，用GameMenu命名空间防止同名污染
*****************************************/
#include "GameMenu.h"

namespace GameMenu {
	// 引入两个命名空间
	using namespace Graphic;
	using namespace Controller;

	size_t frame = 0;                  // 定义已渲染的帧数 
	COORD pos = { 0 }, hitPos = { 0 }; // 定义鼠标当前的坐标和左键点击的坐标
	int choice = 0, oldChoice = 0;     // 定义选中的菜单项
	int subChoice = 0, subOldChoice = 0;	//定义了在次级菜单中的选择菜单项
	int diyChoice = 0, diyOldChoice = 0;	//定义了在自定义菜单中的输入项
	int chatChoice = 0, chatOldChoice = 0;	//定义了在弹出窗口的输入项
	int modeChoice = 0, modeOldChoice = 0;  //定义了在模式窗口的输入项
	bool isChosen = false;             // 定义是否做出选择
	bool subIsChosen = false;		   // 定义在次级菜单是否做出选择
	bool diyIsChosen = false;		   // 定义了自定义菜单中的输入项是否选择
	bool chatIsChosen = false;		   // 定义了chat窗口的输入项是否选择
	bool modeIsChosen = false;		   // 定义了mode窗口的输入项是否选择
	char key = '\0';                   // 定义当前键盘的按键
	clock_t tic = clock();             // 定义程序上一帧渲染的时刻
	// 如果你有新加的变量，建议加在下方和预置变量做区别
	int boomNum = 0;				   // 定义雷数
	int mapWidth = 10;				   // 定义地图宽度
	int mapHeight = 10;				   // 定义地图高度
	int tempNum[3] = { 0,0,0 };	       // 定义自定义的暂存数值:0 代表雷数	1 代表宽度	2 代表高度
	int pin = 0, pin_ = 0;			   // 输出时使用的工作指针
	int windowWidth = 44, windowHeight = 25; //定义初始化的窗口大小
	bool modeFinallyFlag = 0;		   // 定义最终选择的扭曲模式与否

	/****************************************
	Function:  outChat()
	Parameter: outString(char[])
	Return:    None(void)
	Description:
	用于弹出对话框的框体，对话框文字即为outSring（长度必须为18）
	*****************************************/
	void outChat(const char outString[]) {
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 6);
		PutString("                               ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 5);
		PutString("    ╔══════════════╦══════╗    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 4);
		PutString("    ║              ║  ×  ║    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 3);
		PutString("    ╠══════════════╩══════╣    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 2);
		PutString("    ║                     ║    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 1);
		PutString("    ╚═════════════════════╝    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 );
		PutString("                               ");
		MovePos(windowWidth / 2 / 2 * 2 - 7, windowHeight / 2 - 2);
		PutString(outString);
		Update();
	}

	/****************************************
	Function:  introMenu()
	Parameter: Nnoe(void)
	Return:    None(void)
	Description:
	用于画出介绍菜单
	*****************************************/
	void introMenu() {
		ClearScreen();  // 清屏
		MovePos(16, 2); // 移动坐标到 14,2
		PutString("扫雷游戏简介"); // 在这个坐标放置一个string
		MovePos(6, 4);
		PutString("    《扫雷》是一款大众类的益智小");
		MovePos(6, 5);
		PutString("游戏，于1992年发行。游戏目标是在");
		MovePos(6, 6);
		PutString("最短的时间内根据点击格子出现的数");
		MovePos(6, 7);
		PutString("字找出所有非雷格子，同时避免踩到");
		MovePos(6, 8);
		PutString("雷，踩到一个雷即全盘皆输。");
		MovePos(6, 10);
		PutString("    按下任意键返回。。。。。。");

		Update();
	}

	/****************************************
	Function:  outNum()
	Parameter: outnum(int)
	Return:    None(void)
	Description:
	在自定义环节用于数字的输出
	*****************************************/
	void outNum(int outnum) {
		if (outnum / 10 == 0) {

		}
		else {
			outNum(outnum / 10);
		}
		MovePos(pin_, pin);
		PutChar((char)((outnum % 10 )+ '0'));
		pin_++;
	}

	/****************************************
	Function:  renderMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	清屏并且打印标题和相关菜单选项
	*****************************************/
	void renderMenu() {
		ClearScreen();  // 清屏
		MovePos(14, 2); // 移动坐标到 14,2
		PutString("扫雷大作战！"); // 在这个坐标放置一个string
		MovePos(15, 4);
		PutString("开始战斗！");
		MovePos(15, 6);
		PutString("查看说明！");
		MovePos(15, 8);
		PutString("退出游戏！");
		Update();
	}

	/****************************************
	Function:  FormModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	选择扫雷的模式扭曲与否
	*****************************************/
	void FormModeChoice() {
		ClearScreen();  // 清屏
		MovePos(12, 2);
		PutString(">>请选择模式<<");
		MovePos(16, 4);
		PutString("正  常"); // 在这个坐标放置一个string
		MovePos(16, 6);
		PutString("扭  曲");
		Update();
	}

	/****************************************
	Function:  ModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	选择扫雷的难度或是自定义大小与雷数
	*****************************************/
	void ModeChoice() {
		ClearScreen();  // 清屏
		MovePos(12, 2);
		PutString(">>请选择难度<<");
		MovePos(16, 4);
		PutString("简  单"); // 在这个坐标放置一个string
		MovePos(16, 6);
		PutString("中  等");
		MovePos(16, 8);
		PutString("困  难");
		MovePos(15, 10);
		PutString("自 定 义");
		Update();
	}

	/****************************************
	Function:  DiyChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	选择扫雷的难度或是自定义大小与雷数
	*****************************************/
	void DiyChoice() {
		ClearScreen();		//清屏
		MovePos(11, 2);
		PutString(">>自定义难度：<<");
		MovePos(8, 4);
		PutString("雷数：");
		MovePos(8, 6);
		PutString("宽度：");
		MovePos(8, 8);
		PutString("高度：");
		MovePos(17, 11);
		PutString("确认");
		if (tempNum[0] != 0) {
			pin_ = 22;
			pin = 4;
			outNum(tempNum[0]);
		}
		if (tempNum[1] != 0) {
			pin_ = 22;
			pin = 6;
			outNum(tempNum[1]);
		}
		if (tempNum[2] != 0) {
			pin_ = 22;
			pin = 8;
			outNum(tempNum[2]);
		}
		Update();
	}

	/****************************************
	Function:  InitMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	初始化菜单，同时初始化图形框架和控制器框架
	*****************************************/
	void InitMenu() {
		InitGraphic(windowWidth, windowHeight); // 初始化新的窗口大小为44,25
		InitController();    // 初始化控制器
		renderMenu();        // 绘制菜单
	}

	/****************************************
	Function:  randerChoiceBackground(int choice)
	Parameter: int choice
	Return:    None(void)
	Description:
	将当前选中的菜单项(int choice)背景渲染一个渐变消失的白色
	*****************************************/
	void randerChoiceBackground(int choice) {
		size_t y = 0;  // 定义当前选中的菜单项的实际y坐标
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		switch (choice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y); // 将坐标移到目标点
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
		}
		Update();  // 更新界面
	}

	/****************************************
	Function:  randerChatChoiceBackground(int choice)
	Parameter: int chatChoice
	Return:    None(void)
	Description:
	将当前选中的chat菜单项(int chatChoice)背景渲染一个渐变消失的白色
	*****************************************/
	void randerChatChoiceBackground(int chatChoice) {
		int y = 0;  // 定义当前选中的菜单项的实际y坐标
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		for (int x = (windowWidth / 2 / 2 * 2 + 6); x < (windowWidth / 2 / 2 * 2 + 10); x++) {
			MovePos((SHORT)x, windowHeight / 2 - 4); // 将坐标移到目标点
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
		}
		Update();  // 更新界面
	}

	/****************************************
	Function:  randerDiyChoiceBackground(int choice)
	Parameter: int diyChoice
	Return:    None(void)
	Description:
	将当前选中的diy菜单项(int diyChoice)背景渲染一个渐变消失的白色
	*****************************************/
	void randerDiyChoiceBackground(int diyChoice) {
		size_t y = 0;  // 定义当前选中的菜单项的实际y坐标
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		switch (diyChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 11;
			break;
		}
		for (size_t x = (diyChoice == 3 ? 10 : 21); x < (diyChoice == 3 ? 30 : 28); x++) {
			MovePos((SHORT)x, (SHORT)y); // 将坐标移到目标点
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
		}
		Update();  // 更新界面
	}

	/****************************************
	Function:  randerModeChoiceBackground(int choice)
	Parameter: int modeChoice
	Return:    None(void)
	Description:
	将当前选中的mode菜单项(int modeChoice)背景渲染一个渐变消失的白色
	*****************************************/
	void randerModeChoiceBackground(int modeChoice) {
		size_t y = 0;  // 定义当前选中的菜单项的实际y坐标
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		switch (modeChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		}
		for (size_t x = 10 ; x < 30 ; x++) {
			MovePos((SHORT)x, (SHORT)y); // 将坐标移到目标点
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
		}
		Update();  // 更新界面
	}

	/****************************************
	Function:  randerSubChoiceBackground(int choice)
	Parameter: int subChoice
	Return:    None(void)
	Description:
	将当前选中的次级菜单项(int subChoice)背景渲染一个渐变消失的白色
	*****************************************/
	void randerSubChoiceBackground(int subChoice) {
		size_t y = 0;  // 定义当前选中的菜单项的实际y坐标
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		switch (subChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 10;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y); // 将坐标移到目标点
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
		}
		Update();  // 更新界面
	}

	/****************************************
	Function:  clearChoiceBackground(int choice)
	Parameter: int choice
	Return:    None(void)
	Description:
	将上次选中的菜单项(int choice)背景恢复为黑色，内容大致和上一个函数一致，不多做阐述
	*****************************************/
	void clearChoiceBackground(int choice) {
		size_t y = 0;
		switch (choice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearChatChoiceBackground(int choice)
	Parameter: int diyChoice
	Return:    None(void)
	Description:
	将上次选中的chat菜单项(int chatChoice)背景恢复为黑色，内容大致和上一个函数一致，不多做阐述
	*****************************************/
	void clearChatChoiceBackground(int chatChoice) {
		size_t y = 0;
		for (int x = (windowWidth / 2 / 2 * 2 + 6); x < (windowWidth / 2 / 2 * 2 + 10); x++) {
			MovePos((SHORT)x, windowHeight / 2 - 4);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearDiyChoiceBackground(int choice)
	Parameter: int diyChoice
	Return:    None(void)
	Description:
	将上次选中的diy菜单项(int diyChoice)背景恢复为黑色，内容大致和上一个函数一致，不多做阐述
	*****************************************/
	void clearDiyChoiceBackground(int diyChoice) {
		size_t y = 0;
		switch (diyChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 11;
		}
		for (size_t x = (diyChoice == 3 ? 10 : 21); x < (diyChoice == 3 ? 30 : 28); x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearSubChoiceBackground(int choice)
	Parameter: int subChoice
	Return:    None(void)
	Description:
	将上次选中的次级菜单项(int subChoice)背景恢复为黑色，内容大致和上一个函数一致，不多做阐述
	*****************************************/
	void clearSubChoiceBackground(int subChoice) {
		size_t y = 0;
		switch (subChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 10;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearModeChoiceBackground(int choice)
	Parameter: int modeChoice
	Return:    None(void)
	Description:
	将上次选中的mode菜单项(int modeChoice)背景恢复为黑色，内容大致和上一个函数一致，不多做阐述
	*****************************************/
	void clearModeChoiceBackground(int modeChoice) {
		size_t y = 0;
		switch (modeChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  renderIntro()
	Parameter: None(void)
	Return:    None(void)
	Description:
	绘制游戏说明
	*****************************************/
	void renderIntro() {
		introMenu();
		bool quitFlag = true;					// 按下任意键退出

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

			// 左键和右键分别对应操作1和2
			if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
				quitFlag = false;
			}
			else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
				quitFlag = false;
			}
		}
	}

	/****************************************
	Function:  checkSubChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查键盘/鼠标输入，并更新选中的次级菜单项
	*****************************************/
	void checkSubChoice() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		subIsChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != 4 || pos.X < 10 || pos.X > 30)) {
			subChoice = 0;
		}
		else if (!(pos.Y != 6 || pos.X < 10 || pos.X > 30)) {
			subChoice = 1;
		}
		else if (!(pos.Y != 8 || pos.X < 10 || pos.X > 30)) {
			subChoice = 2;
		}
		else if (!(pos.Y != 10 || pos.X < 10 || pos.X > 30)) {
			subChoice = 3;
		}

		// 检查键盘按键，更新选中项
		switch (key) {
		case VK_UP:
			// 上方向键
			if (subChoice > 0)subChoice--;
			break;
		case VK_DOWN:
			// 下方向键
			if (subChoice < 3)subChoice++;
			break;
		case VK_RETURN:
			// 回车，更新选中
			subIsChosen = true;
			break;
		}

		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != 4 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
		else if (!(hitPos.Y != 6 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
		else if (!(hitPos.Y != 8 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
		else if (!(hitPos.Y != 10 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
	}

	/****************************************
	Function:  checkModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查键盘/鼠标输入，并更新选中的mode菜单项
	*****************************************/
	void checkModeChoice() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		subIsChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != 4 || pos.X < 10 || pos.X > 30)) {
			modeChoice = 0;
		}
		else if (!(pos.Y != 6 || pos.X < 10 || pos.X > 30)) {
			modeChoice = 1;
		}

		// 检查键盘按键，更新选中项
		switch (key) {
		case VK_UP:
			// 上方向键
			if (modeChoice > 0)modeChoice--;
			break;
		case VK_DOWN:
			// 下方向键
			if (modeChoice < 1)modeChoice++;
			break;
		case VK_RETURN:
			// 回车，更新选中
			modeIsChosen = true;
			break;
		}

		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != 4 || hitPos.X < 10 || hitPos.X > 30)) {
			modeIsChosen = true;
		}
		else if (!(hitPos.Y != 6 || hitPos.X < 10 || hitPos.X > 30)) {
			modeIsChosen = true;
		}
	}

	/****************************************
	Function:  checkChatChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查键盘/鼠标输入，并更新选中的次级菜单项
	*****************************************/
	void checkChatChoice() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		chatIsChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != windowHeight / 2 - 4 || pos.X < windowWidth / 2 + 7 || pos.X > windowWidth / 2 + 12)) {
			chatChoice = 0;
		}

		// 检查键盘按键，更新选中项
		switch (key) {
		case VK_RETURN:
			// 回车，更新选中
			chatIsChosen = true;
			break;
		}

		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != windowHeight / 2 - 4 || pos.X < windowWidth / 2 + 7 || pos.X > windowWidth / 2 + 12)) {
			chatIsChosen = true;
		}
	}

	/****************************************
	Function:  WaitChatChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	chat菜单的循环，若不退出将一直渲染下去。
	*****************************************/
	void WaitChatChoice() {
		bool runChatFlag = true;
		while (runChatFlag) {
			checkChatChoice();  // 检查输入

			if (chatChoice != chatOldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearChatChoiceBackground(chatOldChoice);
				chatOldChoice = chatChoice;
			}
			randerChatChoiceBackground(chatChoice); // 绘制选中项的背景

			// 如果选中菜单，执行操作
			if (chatIsChosen) {
				switch (chatChoice) {
				case 0:
					runChatFlag = false;
					break;
				}
			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}
	}

	/****************************************
	Function:  checkDiyChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查键盘/鼠标输入，并更新选中的次级菜单项
	*****************************************/
	void checkDiyChoice() {

		diyIsChosen = false;		// 重置选中状态

		int tempPosition = 0;		//定义输入的位置

		for (;;) {
			FlushInput();           // 刷新输入缓冲区
			pos = GetCursorPos();   // 获取鼠标输入
			key = GetKeyHit();		// 获取键盘输入
			hitPos = GetCursorHitPos();  // 获取鼠标单击输入

			// 检查鼠标位置，更新选中项
			if (!(pos.Y != 4 || pos.X < 20 || pos.X > 29)) {
				diyChoice = 0;
			}
			else if (!(pos.Y != 6 || pos.X < 20 || pos.X > 29)) {
				diyChoice = 1;
			}
			else if (!(pos.Y != 8 || pos.X < 20 || pos.X > 29)) {
				diyChoice = 2;
			}
			else if (!(pos.Y != 11 || pos.X < 10 || pos.X > 30)) {
				diyChoice = 3;
			}

			// 检查键盘按键，更新选中项
			switch (key) {
			case VK_UP:
				// 上方向键
				if (diyChoice > 0) {
					diyChoice--;
					if (diyChoice <= 2) {
						tempPosition = diyChoice;
					}
				}
				break;
			case VK_DOWN:
				// 下方向键
				if (diyChoice < 3) {
					diyChoice++;
					if (diyChoice <= 2) {
						tempPosition = diyChoice;
					}
				}
				break;
			case '0':
			case VK_NUMPAD0:
				// 0键
				if (diyChoice != 3) {
					if (tempNum[tempPosition] != 0) {
						tempNum[tempPosition] = tempNum[tempPosition] * 10;
					}
					DiyChoice();
				}
				break;
			case '1':
			case VK_NUMPAD1:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 1;
					DiyChoice();
				}
				break;
			case '2':
			case VK_NUMPAD2:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 2;
					DiyChoice();
				}
				break;
			case '3':
			case VK_NUMPAD3:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 3;
					DiyChoice();
				}
				break;
			case '4':
			case VK_NUMPAD4:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 4;
					DiyChoice();
				}
				break;
			case '5':
			case VK_NUMPAD5:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 5;
					DiyChoice();
				}
				break;
			case '6':
			case VK_NUMPAD6:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 6;
					DiyChoice();
				}
				break;
			case '7':
			case VK_NUMPAD7:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 7;
					DiyChoice();
				}
				break;
			case '8':
			case VK_NUMPAD8:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 8;
					DiyChoice();
				}
				break;
			case '9':
			case VK_NUMPAD9:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 9;
					DiyChoice();
				}
				break;
			case VK_BACK:
				tempNum[tempPosition] /= 10;
				DiyChoice();
				break;
			case VK_RETURN:
				// 回车
				if (tempPosition < 2) {
					diyChoice++;
					tempPosition++;
				}
				else if (tempPosition >= 2) {
					diyIsChosen = true;
				}
				break;
			}

			// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
			if (!(hitPos.Y != 4 || hitPos.X < 21 || hitPos.X > 27)) {
				tempPosition = 0;
			}
			else if (!(hitPos.Y != 6 || hitPos.X < 21 || hitPos.X > 27)) {
				tempPosition = 1;
			}
			else if (!(hitPos.Y != 8 || hitPos.X < 21 || hitPos.X > 27)) {
				tempPosition = 2;
			}
			else if (!(hitPos.Y != 11 || hitPos.X < 15 || hitPos.X > 23)) {
				diyIsChosen = true;
			}

			if (diyIsChosen) {
				if (tempNum[1] <= 6) {
					outChat("自定义宽度应大于6");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[1] > 50) {
					outChat("自定宽度应小于51 ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[2] < 7) {
					outChat("自定义高度应大于6");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[2] > 25) {
					outChat("自定高度应小于26 ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[0] < 9) {
					outChat("自定雷数至少为10 ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[0] > tempNum[1] * tempNum[2] - 9) {
					outChat("定义的雷数过多！ ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				boomNum = tempNum[0];
				mapWidth = tempNum[1];
				mapHeight = tempNum[2];
				break;
			}

			if (diyChoice != diyOldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearDiyChoiceBackground(diyOldChoice);
				diyOldChoice = diyChoice;
			}
			randerDiyChoiceBackground(diyChoice); // 绘制选中项的背景

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}

	}

	/****************************************
	Function:  checkChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查键盘/鼠标输入，并更新选中的菜单项
	*****************************************/
	void checkChoice() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		isChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != 4 || pos.X < 10 || pos.X > 30)) {
			choice = 0;
		}
		else if (!(pos.Y != 6 || pos.X < 10 || pos.X > 30)) {
			choice = 1;
		}
		else if (!(pos.Y != 8 || pos.X < 10 || pos.X > 30)) {
			choice = 2;
		}

		// 检查键盘按键，更新选中项
		switch (key) {
		case VK_UP:
			// 上方向键
			if (choice > 0)choice--;
			break;
		case VK_DOWN:
			// 下方向键
			if (choice < 3)choice++;
			break;
		case VK_RETURN:
			// 回车，更新选中
			isChosen = true;
			break;
		}

		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != 4 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 6 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 8 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
	}

	/****************************************
	Function:  WaitDiyChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	该菜单用于自定义雷数的设定。
	*****************************************/
	void WaitDiyChoice() {
		checkDiyChoice();  // 检查输入
	}

	/****************************************
	Function:  WaitSubChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	次级菜单的循环，若不退出将一直渲染下去，用于扫雷难度的选定。
	*****************************************/
	void WaitSubChoice() {
		bool runSubFlag = true;
		while (runSubFlag) {
			checkSubChoice();  // 检查输入

			if (subChoice != subOldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearSubChoiceBackground(subOldChoice);
				subOldChoice = subChoice;
			}
			randerSubChoiceBackground(subChoice); // 绘制选中项的背景

			// 如果选中菜单，执行操作
			if (subIsChosen) {
				switch (subChoice) {
				case 0:
					// 开始游戏
					boomNum = 10;
					mapWidth = 10;
					mapHeight = 10;
					runSubFlag = false;
					break;
				case 1:
					boomNum = 40;
					mapWidth = 16;
					mapHeight = 16;
					runSubFlag = false;
					break;
				case 2:
					boomNum = 99;
					mapWidth = 30;
					mapHeight = 16;
					runSubFlag = false;
					break;
				case 3:
					DiyChoice();
					WaitDiyChoice();
					runSubFlag = false;
					break;
				}
			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}
	}

	/****************************************
	Function:  WaitModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	mode菜单的循环，若不退出将一直渲染下去，用于扫雷模式的选定。
	*****************************************/
	void WaitModeChoice() {
		bool runModeFlag = true;
		while (runModeFlag) {
			checkModeChoice();  // 检查输入

			if (modeChoice != modeOldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearModeChoiceBackground(modeOldChoice);
				modeOldChoice = modeChoice;
			}
			randerModeChoiceBackground(modeChoice); // 绘制选中项的背景

			// 如果选中菜单，执行操作
			if (modeIsChosen) {
				switch (modeChoice) {
				case 0:
					// 开始游戏
					ModeChoice();
					WaitSubChoice();
					modeFinallyFlag = false;
					runModeFlag = false;
					break;
				case 1:
					ModeChoice();
					WaitSubChoice();
					modeFinallyFlag = true;
					runModeFlag = false;
					break;
				}
			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}
	}

	/****************************************
	Function:  ResetSize(int width,int height)
	Parameter: width(int), height(int)
	Return:    None(void)
	Description:
	该函数用于根据地图的大小重设命令行窗口的大小，用于正常模块
	*****************************************/
	void ResetSize(int width, int height) {
		windowWidth = 4 * width + 4;
		windowHeight = 2 * height + 5;
		InitMenu();
	}

	/****************************************
	Function:  ProResetSize(int width,int height)
	Parameter: width(int), height(int)
	Return:    None(void)
	Description:
	该函数用于根据地图的大小重设命令行窗口的大小，用于扭曲空间模块
	*****************************************/
	void ProResetSize(int width, int height) {
		windowWidth = (3 * width + 2) / 4 * 4 + 4;
		windowWidth = (windowWidth < 40 ? 40 : windowWidth);
		windowHeight = 2 * (height) + 6;
		InitMenu();
	}

	/****************************************
	Function:  WaitChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	主循环，若不退出将一直渲染下去
	*****************************************/
	void WaitChoice() {
		bool runFlag = true;
		while (runFlag) {
			checkChoice();  // 检查输入

			if (choice != oldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearChoiceBackground(oldChoice);
				oldChoice = choice;
			}
			randerChoiceBackground(choice); // 绘制选中项的背景

			// 如果选中菜单，执行操作
			if (isChosen) {
				switch (choice) {
				case 0:
					// 开始游戏
					FormModeChoice();
					WaitModeChoice();
					if (modeFinallyFlag) {
						ProResetSize(mapWidth, mapHeight);
						GameEnginePro::InitGame(mapWidth, mapHeight, boomNum);
						GameEnginePro::Play();
						GameEnginePro::DestroyGame();
					}
					else {
						ResetSize(mapWidth, mapHeight);
						GameEngine::InitGame(mapWidth, mapHeight, boomNum);
						GameEngine::Play();
						GameEngine::DestroyGame();
					}
					boomNum = 0;
					mapWidth = 0;
					mapHeight = 0;
					windowHeight = 25;
					windowWidth = 44;
					InitMenu();
					renderMenu();
					tempNum[0] = 0;
					tempNum[1] = 0;
					tempNum[2] = 0;
					modeIsChosen = false;
					break;
				case 1:
					// 绘制说明
					renderIntro();
					renderMenu();
					break;
				case 2:
					// 将Flag置否以退出循环
					runFlag = false;
					break;
				}
			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}
	}
}