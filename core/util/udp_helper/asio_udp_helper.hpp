
// Headers
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

// json library
#include <nlohmann/json.hpp>

#include <thread>

#include <mutex>

using json = nlohmann::json;

// Need to define WIN32 because each version of windows has slightly different ways of handling networking
#include <SDKDDKVer.h>

// ASIO library
#define ASIO_STANDALONE
#include <asio/asio.hpp>

using udp = asio::ip::udp;

// Other Windows functions
#include <Windows.h>

class asio_udp_helper
{
  public:
    asio::error_code ecVAR;
    udp::endpoint recv_terminal;
    udp::endpoint tran_terminal;
    udp::endpoint cmd_recv_terminal;
    udp::endpoint cmd_tran_terminal;
    asio::io_context recv_context, tran_context;
    asio::io_context cmd_recv_context, cmd_tran_context;
    udp::socket recv_socket, tran_socket;
    udp::socket cmd_recv_socket, cmd_tran_socket;
    uint32_t recv_port;
    uint32_t trans_port;
    uint32_t cmd_recv_port;
    uint32_t cmd_trans_port;
    std::string ip_addr;

  public:
    asio_udp_helper(const std::string ip_addr, uint32_t recv_port, uint32_t trans_port, uint32_t cmd_recv_port,
                    uint32_t cmd_trans_port)
        : ecVAR(), recv_terminal(asio::ip::make_address(ip_addr), recv_port),
          tran_terminal(asio::ip::make_address(ip_addr), trans_port),
          cmd_recv_terminal(asio::ip::make_address(ip_addr), cmd_recv_port),
          cmd_tran_terminal(asio::ip::make_address(ip_addr), cmd_trans_port), recv_context(), tran_context(),
          cmd_recv_context(), cmd_tran_context(), recv_socket(recv_context), tran_socket(tran_context),
          cmd_recv_socket(cmd_recv_context), cmd_tran_socket(cmd_tran_context), recv_port(recv_port),
          trans_port(trans_port), cmd_recv_port(cmd_recv_port), cmd_trans_port(cmd_trans_port), ip_addr(ip_addr)
    {
    }

    // Create link
    void connect_to_target()
    {
        tran_socket.connect(tran_terminal);

        recv_socket.open(asio::ip::udp::v4());
        recv_socket.bind(recv_terminal);

        cmd_tran_socket.connect(cmd_tran_terminal);

        cmd_recv_socket.open(asio::ip::udp::v4());
        cmd_recv_socket.bind(cmd_recv_terminal);
    }

    void release_connect()
    {
        tran_socket.close();
        recv_socket.close();
        cmd_tran_socket.close();
        cmd_recv_socket.close();
    }

    void send_msg(char *msg, uint32_t len)
    {
        tran_socket.send(asio::buffer(msg, len));
    }

    void send_cmd(char *msg, uint32_t len)
    {
        cmd_tran_socket.send(asio::buffer(msg, len));
    }

    void recv_msg(char *msg, uint32_t len)
    {
        // recv_socket.receive(asio::buffer((char *)&data_t, sizeof(double)));
        recv_socket.receive_from(asio::buffer(msg, len), recv_terminal);
    }

    void recv_cmd(char *msg, uint32_t len)
    {
        // recv_socket.receive(asio::buffer((char *)&data_t, sizeof(double)));
        cmd_recv_socket.receive_from(asio::buffer(msg, len), cmd_recv_terminal);
    }

    void register_start_callback()
    {
        // Receive "start" string to judge simulation is start


    }

    void regiser_stop_callback()
    {
        // Receive "stop" string to judge simulation is stop

    }

    static asio_udp_helper *parse_network_config(std::string config_file)
    {
        // file objects
        std::fstream network_json(config_file, std::fstream::in | std::fstream::out | std::fstream::app);

        if (!network_json.is_open())
        {
            std::cout << "[WARN] Cannot find network config file." << std::endl;

            return nullptr;
        }

        // config json
        json config;

        try
        {
            config = json::parse(network_json);
        }
        catch (const std::exception &e)
        {
            std::cout << "[ERRO] Cannot parse .json file, " << config_file << std::endl;
            std::cerr << e.what() << std::endl;

            return nullptr;
        }

        // get result
        try
        {
            std::string target_addr = config["target_address"];
            uint32_t recv_port = config["receive_port"];
            uint32_t trans_port = config["transmit_port"];
            uint32_t cmd_recv_port = config["command_recv_port"];
            uint32_t cmd_trans_port = config["command_trans_port"];

            // create objects
            return new asio_udp_helper(target_addr, recv_port, trans_port, cmd_recv_port, cmd_trans_port);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;

            return nullptr;
        }
    }

    // Data receive and transmit, exchange two ports
    static asio_udp_helper *parse_network_config_server(std::string config_file)
    {
        // file objects
        std::fstream network_json(config_file, std::fstream::in | std::fstream::out | std::fstream::app);

        if (!network_json.is_open())
        {
            std::cout << "[WARN] Cannot find network config file." << std::endl;

            return nullptr;
        }

        // config json
        json config;

        try
        {
            config = json::parse(network_json);
        }
        catch (const std::exception &e)
        {
            std::cout << "[ERRO] Cannot parse .json file, " << config_file << std::endl;
            std::cerr << e.what() << std::endl;

            return nullptr;
        }

        // get result
        try
        {
            std::string target_addr = config["target_address"];
            uint32_t recv_port = config["receive_port"];
            uint32_t trans_port = config["transmit_port"];
            uint32_t cmd_recv_port = config["command_recv_port"];
            uint32_t cmd_trans_port = config["command_trans_port"];

            // create objects
            // The function is different from parse_network_config
            return new asio_udp_helper(target_addr, trans_port, recv_port, cmd_trans_port, cmd_recv_port);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;

            return nullptr;
        }
    }
};