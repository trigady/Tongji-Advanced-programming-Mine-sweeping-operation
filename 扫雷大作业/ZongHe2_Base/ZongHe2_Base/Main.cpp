/****************************************
Main.cpp
Author: Root
Date: 2019/12/09
Description:
定义了整个程序的入点，引入Menu头文件，初始化菜单，并进入等待选择的循环
*****************************************/
#include "GameMenu.h"

int main()
{
	GameMenu::InitMenu();
	GameMenu::WaitChoice();
}