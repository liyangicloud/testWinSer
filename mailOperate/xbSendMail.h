#pragma once

#include <list>
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#define __LY_DEBUG

#ifdef __LY_DEBUG
//("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#define lys(format,...) printf("Line%3d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define lys(format,...)
#endif






#define XB_ERRORCODE_ATTACHFILE_FAILED 257




const int MAXLEN = 1024;
const int MAX_FILE_LEN = 6000;



struct FILEINFO /*������¼�ļ���һЩ��Ϣ*/
{
    char fileName[MAX_PATH]; /*�ļ�����*/
    char filePath[MAX_PATH]; /*�ļ�����·��*/
};

class CSmtp
{
public:
    CSmtp(void);
    CSmtp(
        int port,
        std::string srvDomain,  //smtp����������
        std::string userName,   //�û���
        std::string password,   //����
        std::string targetEmail, //Ŀ���ʼ���ַ
        std::string emailTitle,  //����
        std::string content       //����
        );
    ~CSmtp(void);
private:
    int port;
    std::string domain;
    std::string user;
    std::string pass;
    std::string targetAddr;
    std::string title;
    std::string content;
    /*Ϊ�˷�������ļ���ɾ���ļ�����ģ�ʹ��list������Ϊ���㣬���Ŵ�������ݽṹ���涼ѧ��*/
    std::list <FILEINFO *> listFile;

    char buff[MAXLEN + 1];
    int buffLen;
    SOCKET sockClient;  //�ͻ��˵��׽���
	bool m_bSocketUP;
    WSADATA m_wsaData;

public:
    bool CreateConn(); /*��������*/

    bool Send(std::string &message);
    bool Recv();

    void FormatEmailHead(std::string &email);//��ʽ��Ҫ���͵��ʼ�ͷ��
    int Login();
    bool SendEmailHead();       //�����ʼ�ͷ����Ϣ
    bool SendTextBody();        //�����ı���Ϣ
    //bool SendAttachment();        //���͸���
    int SendAttachment_Ex();
    bool SendEnd();
public:
    void AddAttachment(std::string &filePath); //��Ӹ���
    void DeleteAttachment(std::string &filePath); //ɾ������
    void DeleteAllAttachment(); //ɾ�����еĸ���

    void SetSrvDomain(std::string &domain);
    void SetUserName(std::string &user);
    void SetPass(std::string &pass);
    void SetTargetEmail(std::string &targetAddr);
    void SetEmailTitle(std::string &title);
    void SetContent(std::string &content);
    void SetPort(int port);
    int SendEmail_Ex();
    /*���ڴ������˵��:1.��������µĴ���2.�û�������3.�������4.�ļ�������0.�ɹ�*/
    char* base64Encode(char const* origSigned, unsigned origLength);
};


