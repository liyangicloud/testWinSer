// testWC.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lyWinService.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char chInput = 'a';

	do{
		scanf("%c",&chInput);
		switch (chInput){
		case 'i':
			fnInstallServiceTemp();
			break;
		case 'd':
			fnDeleteService();
			break;
		case 'c':
			fnFindSomeServiceWithCommitString();
			break;
		case 10:
			continue;
			break;
		default:
			break;
		}

		printf("command success!  :%d\n",chInput);
	}while('q'!=chInput);

	//fnInstallService();
	//fnTestSomeFunc();
	//fnFindSomeServiceWithCommitString();

	return 0;
}

