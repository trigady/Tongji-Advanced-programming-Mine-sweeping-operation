/****************************************
GameMenu.cpp
Author: Root
Date: 2019/12/09
Description:
��������Ⱦ��ܣ���Graphic�����ռ��ֹͬ����Ⱦ
��*��ע�⣺����������ȫ�����ڲ�������ʲô���������򲻽����޸�����ļ���*��
*****************************************/
#include "Graphic.h"

namespace Graphic {
	HANDLE hStdOut = NULL;				   // ��׼������
	UCHAR* pCanvas = nullptr;			   // ��������ָ��
	UCHAR* updateQueue = nullptr;		   // ���¶�������ָ��
	SHORT nowPos[2] = { 0 };			   // ��ǰλ����������
	int screenWidth = 0, screenHeight = 0; // ��Ļ���
	char* outputSequence = nullptr;		   // �����Ļ�ַ�������ָ��
	size_t sequenceLength = 0;			   // �ַ������еĵ�ǰ����
	size_t sequenceCapacity = 0;		   // �ַ������е�����
	size_t needUpdate = 0;				   // ��Ҫ���µ�����

	const DWORD MOD_FRONT = 0, MOD_BACK = 1, MOD_BOTH = 2;  // ��������
	
	/****************************************
	Function:  MovePos(SHORT x, SHORT y)
	Parameter: SHORT x, SHORT y
	Return:    None(void)
	Description:
	����ǰ�������Ϊ���
	*****************************************/
	void MovePos(SHORT x, SHORT y) {
		nowPos[0] = x; nowPos[1] = y;
	}

	/****************************************
	Function:  movePos(SHORT x, SHORT y)
	Parameter: SHORT x, SHORT y
	Return:    None(void)
	Description:
	�ú����������������ڲ�������̨�Ĺ���ƶ���(x, y)
	*****************************************/
	void movePos(SHORT x, SHORT y) {
		SetConsoleCursorPosition(hStdOut, { x, y });
	}

	/****************************************
	Function:  pushUpdateQueue(const int screenIndex)
	Parameter: int screenIndex
	Return:    None(void)
	Description:
	��Ҫ���µ���Ļ����ֵ������Ҫ���µĶ���
	*****************************************/
	void pushUpdateQueue(const int screenIndex) {
		// ��û�г����¶��е����������������
		if (needUpdate < 512) {
			// ȷ�ϸ��¶��е�ǰλ���±�ĳ���
			const size_t updateIndex = needUpdate * 9;

			// �ֱ����ö��еĸ���ֵ
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
	����ǰλ��(nowPos������ָʾ��λ��)���ַ���Ϊ�µ�newChar
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
	����ǰλ��(nowPos������ָʾ��λ��)���ַ�����ɫ/����(�����޸��ĸ���modeָ��)��Ϊ�µ���ɫֵ
	*****************************************/
	void ModColor(DWORD mode, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB) {
		const int screenIndex = ((int)nowPos[1] * screenWidth + (int)nowPos[0]) * 7; // ȷ����ǰ����������е��±�
		switch (mode) {
		case MOD_BOTH:
			// ����ǰ��ɫ�뱳��ɫ������break��switch��佫����ִ��
			pCanvas[screenIndex + 1] = textR;
			pCanvas[screenIndex + 2] = textG;
			pCanvas[screenIndex + 3] = textB;
		case MOD_BACK:
			// ���±���ɫ
			pCanvas[screenIndex + 4] = backR;
			pCanvas[screenIndex + 5] = backG;
			pCanvas[screenIndex + 6] = backB;
			break;
		case MOD_FRONT:
			// ����ǰ��ɫ
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
	����ǰλ��(nowPos������ָʾ��λ��)����һ��outChar
	*****************************************/
	void PutChar(const char outChar) {
		const int screenIndex = ((int)nowPos[1] * screenWidth + (int)nowPos[0]) * 7; // ȷ����ǰ����������е��±�
		pCanvas[screenIndex] = outChar;  // �����ַ�

		pCanvas[screenIndex + 1] = 255;  // ����ǰ��ɫ
		pCanvas[screenIndex + 2] = 255;
		pCanvas[screenIndex + 3] = 255;
		pCanvas[screenIndex + 4] = 0;	 // ���ñ���ɫ
		pCanvas[screenIndex + 5] = 0;
		pCanvas[screenIndex + 6] = 0;

		pushUpdateQueue(screenIndex);	 // ������¶���
	}

	/****************************************
	Function:  PutCharWithColor(const char outChar, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB)
	Parameter: char outChar, BYTE textR, textG, textB, backR, backG, backB
	Return:    None(void)
	Description:
	����ǰλ��(nowPos������ָʾ��λ��)����һ��outChar��������Ϊָ������ɫ
	*****************************************/
	void PutCharWithColor(const char outChar, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB) {
		const int screenIndex = ((int)nowPos[1] * screenWidth + (int)nowPos[0]) * 7; // ȷ����ǰ����������е��±�
		pCanvas[screenIndex] = outChar;    // �����ַ�

		pCanvas[screenIndex + 1] = textR;  // ����ǰ��ɫ
		pCanvas[screenIndex + 2] = textG;
		pCanvas[screenIndex + 3] = textB;
		pCanvas[screenIndex + 4] = backR;  // ���ñ���ɫ
		pCanvas[screenIndex + 5] = backG;
		pCanvas[screenIndex + 6] = backB;

		pushUpdateQueue(screenIndex);	   // ������¶���
	}

	/****************************************
	Function:  PutString(const char* outString)
	Parameter: char* outString
	Return:    None(void)
	Description:
	����ǰλ��(nowPos������ָʾ��λ��)����һ���ַ���outString
	*****************************************/
	void PutString(const char* outString) {
		// ����ַ����Ƿ񳬹���Ļ��ȣ�����������˳�����
		const size_t stringLength = strlen(outString);
		if (stringLength + (size_t)nowPos[0] > (size_t)screenWidth) {
			return;
		}
		// ���������������ַ��������ַ���
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
	����ǰλ��(nowPos������ָʾ��λ��)����һ���ַ���outString����������ɫ�����ܻ���ͬ��
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
	����ǰλ��(nowPos������ָʾ��λ��)����һ����Ϊwidth����Ϊheight�ľ���
	*****************************************/
	void DrawRect(const SHORT width, const SHORT height) {
		const SHORT x = nowPos[0], y = nowPos[1];
		// ��鳤���Ƿ񳬹���Ļ
		if (x + width > (SHORT)screenWidth || nowPos[1] + height > (SHORT)screenHeight) {
			return;
		}
		for (SHORT i = x; i < x + width; i++) {
			for (SHORT j = y; j < y + height; j++) {
				// ������λ�ã�Ȼ�����һ������ɫ�Ŀհ�
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
	����ǰλ��(nowPos������ָʾ��λ��)����һ����Ϊwidth����Ϊheight�ľ��Σ�������ɫ������������ͬ
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
	ǿ����������
	*****************************************/
	void ClearScreen() {
		memset(pCanvas, 0, screenWidth * screenHeight * 7 * sizeof(UCHAR)); // ��ջ�������
		memset(updateQueue, 0, 512 * 9 * sizeof(UCHAR));					// ��ն�������
		needUpdate = 999;													// ������ֵ���ó���������������Ӧ��ǿ�Ƹ���
		Update();															// ������Ļ
	}

	/****************************************
	Function:  InitGraphic(int width, int height)
	Parameter: int width, height
	Return:    None(void)
	Description:
	��ʼ��ͼ�β���������Ļ��������Ϊwidth��height
	*****************************************/
	void InitGraphic(int width, int height) {
		screenWidth = width * 2; screenHeight = height;					  // ���ó���
		pCanvas = new UCHAR[width * height * 14];						  // ���û������飬��̬����
		updateQueue = new UCHAR[512 * 9];								  // ���ø��¶������飬��̬����
		outputSequence = new char[(size_t)width * (size_t)height * 128];  // ��������ַ����������飬��̬����
		sequenceCapacity = (int)width * (int)height * 128;				  // �����ַ������е�����

		system("chcp 936");												  // ���¿���̨����ҳ

		// ������̨����Ϊ�������
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

		// ���ع��
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);  // ��ȡ��׼������
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
	���ٲ���������̬��ʼ�����������
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
	�������²������ַ������У�ע�⣺�������������ҪWin10ϵͳ���Ѵ���2017���＾�����߸��£�������Ŀǰ���Զ��Ѿ�����������
	��������Ч������ѯ������ȡ��һ��Graphic�ļ�ʹ�þɵİ汾
	������û�а취ע�����������Ҫ��⣬�����������վ
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
	����ȫ��Ļ���ַ������޷�ע�ͣ����������ο�������վ
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
	�����в�����������Ļ��Ҳ����˵�������������в���������ʵʱ�ģ�ֻ�е��������������Ļ�Ż������ĸ���
	*****************************************/
	void Update() {
		movePos(0, 0);		 // ������ƶ���0,0��Ӳ�ƶ�

		if (needUpdate < 512) {
			constructUpdateSequence();  // ���Ҫ���µ�����С��512������ô�򹹽���������
		}
		else {
			constructScreenSequence();	// ���򹹽�ȫ��Ļ��������
		}
		needUpdate = 0;					// ���ø�������

		// �����в���д�뵽����̨������Ļ
		DWORD dWrite = 0;
		WriteConsoleA(hStdOut, outputSequence, sequenceLength, &dWrite, NULL);
	}
}