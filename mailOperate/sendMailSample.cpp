#include "xbSendMail.h"
#include <Windows.h>
#include <stdio.h>

int main()
{

	char chInput = 'a';
	BOOL bRet = FALSE;
	int err;

    CSmtp smtp(
        25,                             /*smtp�˿�*/
        "smtp.sina.com",                 /*smtp��������ַ*/
        "liyang0939@sina.com",         /*��������ַ*/
        "qunimade1234S",                   /*��������*/
        "liyang0939@sina.com",  /*Ŀ�������ַ,��һ�����ÿո�ָ����Ӷ��������*/
        "This is a test",                           /*����*/
        "I do not know what should i send!"      /*�ʼ�����*/
        );
	std::string filePath("C:\\Owntools\\mail\\1.dll");
    smtp.AddAttachment(filePath);

	do{
		scanf("%c",&chInput);
		switch (chInput){
		case 's':
			if ((err = smtp.SendEmail_Ex()) != 0)
			{
				printf("err = %d\n",err);
			}
			break;
		case 'g':
			bRet;
			break;
		case 'f':
			bRet;
			break;
		case 10:
			continue;
			break;
		default:
			break;
		}
		if(bRet)
		{
			printf("Network is connected!\n");
		}
		printf("###################################command success!  :%d\n",chInput);
	}while('q' != chInput);


	return 0;
}





