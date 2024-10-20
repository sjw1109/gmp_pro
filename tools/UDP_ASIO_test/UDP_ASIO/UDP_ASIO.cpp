
// Headers
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

// json library
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Need to define WIN32 because each version of windows has slightly different ways of handling networking
#include <SDKDDKVer.h>

// ASIO library
#define ASIO_STANDALONE
#include <asio/asio.hpp>

using udp = asio::ip::udp;

// Other Windows functions
#include <Windows.h>

int main(int argc, char *argv[])
{
    std::cout << "Hello World!\n" << std::endl;

    std::string config_file_name("network.json");

    // load config files
    std::fstream network_json(config_file_name);

    if (!network_json.is_open())
    {
        std::cout << "[WARN] Cannot find network config file." << std::endl;

        return 1;
    }

    // config json
    json config;

    try
    {
        config = json::parse(network_json);
    }
    catch (std::exception *e)
    {
        std::cout << "[ERRO] Cannot parse json file, " << config_file_name << std::endl;

        return 1;
    }

    std::cout << "Target config file is loaded: " << config_file_name << std::endl
              << "target receive address: " << config["target_address"] << ":" << config["receive_port"] << std::endl
              << "target transmit address: " << config["target_address"] << ":" << config["transmit_port"] << std::endl;

    // close file handle in time
    network_json.close();

    // asio load library
    asio::error_code ecVAR;
    asio::io_context context;

    udp::endpoint recv_terminal(asio::ip::make_address(config["target_address"]), config["receive_port"]);
    udp::endpoint tran_terminal(asio::ip::make_address(config["target_address"]), config["transmit_port"]);


    asio::io_context recv_context, tran_context;
    udp::socket recv_socket(recv_context), tran_socket(tran_context);


    tran_socket.connect(tran_terminal);

    recv_socket.open(asio::ip::udp::v4());
    recv_socket.bind(recv_terminal);

    // asio buffer for data containing
    char *data_buffer = new char[1024];

    double data = 10;
    double data_t = 0;

    
    tran_socket.send(asio::buffer((char *)&data, sizeof(double)));

    for (int i = 0; i < 100; ++i)
    {
        //recv_socket.receive(asio::buffer((char *)&data_t, sizeof(double)));
        recv_socket.receive_from(asio::buffer((char *)&data_t, sizeof(double)), recv_terminal);

        data = data_t;

        tran_socket.send(asio::buffer((char *)&data, sizeof(double)));
    }

    return 0;
}



