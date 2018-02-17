#include "CSmtp.h"
#include <iostream>

int main()
{
	bool bError = false;

	try
	{
		CSmtp mail;

#define test_gmail_tls

#if defined(test_gmail_tls)
		mail.SetSMTPServer("smtp-mail.outlook.com",587);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_gmail_ssl)
		mail.SetSMTPServer("smtp.gmail.com",465);
		mail.SetSecurityType(USE_SSL);
#elif defined(test_hotmail_TLS)
		mail.SetSMTPServer("smtp.live.com",25);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_aol_tls)
		mail.SetSMTPServer("smtp.aol.com",587);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_yahoo_ssl)
		mail.SetSMTPServer("plus.smtp.mail.yahoo.com",465);
		mail.SetSecurityType(USE_SSL);
#endif

		mail.SetLogin("aaaaa@outlook.com");
		mail.SetPassword("aaaaa");
  		mail.SetSenderName("SetSenderName");
  		mail.SetSenderMail("aaaaa@outlook.com");
  		mail.SetReplyTo("aaaaa@outlook.com");
  		mail.SetSubject("Suject only english");
  		mail.AddRecipient("aaaaaaa@aa.com","destmailname");
		mail.SetCharSet("GB2312");
  		mail.SetXPriority(XPRIORITY_NORMAL);
  		mail.SetXMailer("The Bat! (v3.02) Professional");
		mail.AddMsgLine("Fellows:");
  		mail.AddMsgLine("  你好这是真的，个人Hello!");
		mail.AddMsgLine("  How are you today?");
		mail.AddMsgLine("  This is a test message.");
		mail.AddMsgLine("");
		mail.AddMsgLine("Regards!");
		//mail.ModMsgLine(5,"regards");
		//mail.DelMsgLine(2);
		mail.AddMsgLine("Leon");

  		//mail.AddAttachment("../test1.jpg");
  		//mail.AddAttachment("c:\\test2.exe");
		mail.AddAttachment("C:\\间.txt");
		mail.Send();
	}
	catch(ECSmtp e)
	{
		std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
		bError = true;
	}
	if(!bError)
		std::cout << "Mail was send successfully.\n";
	char chIn;
	std::cin >> chIn; 

	return 0;
}