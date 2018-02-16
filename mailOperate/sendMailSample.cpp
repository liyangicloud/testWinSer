#include "xbSendMail.h"
#include <Windows.h>
#include <stdio.h>

int main()
{

	char chInput = 'a';
	BOOL bRet = FALSE;
	int err;

    CSmtp smtp(
        25,                             /*smtp端口*/
        "smtp.sina.com",                 /*smtp服务器地址*/
        "liyang0939@sina.com",         /*你的邮箱地址*/
        "qunimade1234S",                   /*邮箱密码*/
        "liyang0939@sina.com",  /*目的邮箱地址,这一部分用空格分割可添加多个收信人*/
        "This is a test",                           /*主题*/
        "I do not know what should i send!"      /*邮件正文*/
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





