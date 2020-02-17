#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtplibraryversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace jrtplib;

// 打印错误信息
void checkerror(int rtperr)
{
    if (rtperr < 0)
    {
        std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
        exit(-1);
    }
}

// 主循环

int main(void)
{

// windows相关，不需要可以删了
#ifdef RTP_SOCKETTYPE_WINSOCK
    WSADATA dat;
    WSAStartup(MAKEWORD(2,2),&dat);
#endif // RTP_SOCKETTYPE_WINSOCK

    RTPSession sess;
    uint16_t portbase = 6060;
    uint16_t destport = 6060;
    uint32_t destip;
    std::string ipstr("192.168.0.111");
    int status,i;
    int num = 1000;

    // 打印JRTPLIB版本
    std::cout << "Using version " << RTPLibraryVersion::GetVersion().GetVersionString() << std::endl;

    // 获取本地端口，用以接收
    //std::cout << "Enter local portbase:" << std::endl;
    //std::cin >> portbase;
    //std::cout << std::endl;

    // 目标地址，本机使用127.0.0.1
    //std::cout << "Enter the destination IP address" << std::endl;
    //std::cin >> ipstr;
    destip = inet_addr(ipstr.c_str());
    if (destip == INADDR_NONE)
    {
        std::cerr << "Bad IP address specified" << std::endl;
        return -1;
    }

    // inet_addr函数返回的是网络字节序，需要将网络字节序转化为主机字节序
    destip = ntohl(destip);

    // 目标端口，如果发送给本机，与上面的接收端口相同即可
    //std::cout << "Enter the destination port" << std::endl;
    //std::cin >> destport;

    // 发包数量
    //std::cout << std::endl;
    //std::cout << "Number of packets you wish to be sent:" << std::endl;
    //std::cin >> num;

    // 创建传输参数和会话参数
    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;

    // IMPORTANT: The local timestamp unit MUST be set, otherwise
    //            RTCP Sender Report info will be calculated wrong
    // In this case, we'll be sending 10 samples each second, so we'll
    // put the timestamp unit to (1.0/10.0)
    // 设置时间戳
    sessparams.SetOwnTimestampUnit(1.0/10.0);

    // 是否接收自己发送的包
    sessparams.SetAcceptOwnPackets(true);
    // 设置接收端口
    transparams.SetPortbase(portbase);
    // 创建端口
    status = sess.Create(sessparams,&transparams);
    checkerror(status);

    // 写入IPv4地址和端口
    RTPIPv4Address addr(destip,destport);

    // 添加目标地址
    status = sess.AddDestination(addr);
    checkerror(status);

    for (i = 1 ; i <= num ; i++)
    {
        printf("\nSending packet %d/%d\n",i,num);

        // 发包
        status = sess.SendPacket((void *)"1234567890",10,0,false,10);
        checkerror(status);

        sess.BeginDataAccess();

        // 检查收包
        if (sess.GotoFirstSourceWithData())
        {
            do
            {
                RTPPacket *pack;

                while ((pack = sess.GetNextPacket()) != NULL)
                {
                    // 在这里进行数据处理
                    printf("Got packet !\n");

                    // 不再需要这个包了，删除之
                    sess.DeletePacket(pack);
                }
            } while (sess.GotoNextSourceWithData());
        }

        sess.EndDataAccess();

// 这部分与JThread库相关
#ifndef RTP_SUPPORT_THREAD
        status = sess.Poll();
        checkerror(status);
#endif // RTP_SUPPORT_THREAD

        // 等待一秒，发包间隔
        RTPTime::Wait(RTPTime(1,0));
    }

    // 销毁对话
    sess.BYEDestroy(RTPTime(10,0),0,0);

// Windows相关
#ifdef RTP_SOCKETTYPE_WINSOCK
    WSACleanup();
#endif // RTP_SOCKETTYPE_WINSOCK
    return 0;
}
