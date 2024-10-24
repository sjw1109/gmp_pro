// ASIO_UDP_Helper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdlib.h>
#include <core/util/udp_helper/asio_udp_helper.hpp>

int main()
{
    std::cout << "[GMP] Hello World!\n";

    asio_udp_helper *helper = asio_udp_helper::parse_network_config("network.json");

    if (helper == nullptr)
    {
        std::cout << "Cannot create ASOI Helper.\r\n" << std::endl;
        exit(1);
    }

    helper->connect_to_target();

    helper->server_ack_cmd();

    double data[2] = {10, 10};
    double data_t[2] = {0, 0};

    helper->send_msg((char *)data, sizeof(data));

    for (int i = 0; i < 1000; i++)
    {
        if (helper->recv_msg((char*)data_t, sizeof(data_t)))
        {
            std::cout << "receive complete." << std::endl;
            system("@pause");
            exit(0);
        }

        // operation here

        helper->send_msg((char *)data, sizeof(data));
    }

    delete helper;
}
