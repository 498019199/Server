#include <iostream>
#include <string>
#include <iomanip>

#include "filessystem.h"
#include "shared/base/logger.h"
#include "net/http/http_server.h"
#include "net/http/http_request.h"
#include "net/http/http_response.h"
#include "net/address.h"

extern char favicon[555];
bool benchmark = false;

std::string get_current_time()
{
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    std::time_t c = std::chrono::system_clock::to_time_t(t);
    std::stringstream os;
    os << std::put_time(std::localtime(&c), "%F %T\n");
    return os.str();
}

void on_request(const http_request& req, http_response* resp)
{
    std::cout << "Headers " << req.method_to_string() << " " << req.get_path() << std::endl;
    if (!benchmark)
    {
        const std::map<std::string, std::string>& headers = req.get_hearder_map();
        for (const auto& header : headers)
        {
            std::cout << header.first << ": " << header.second << std::endl;
        }
    }

    if (req.get_path() == "/")
    {
        resp->set_status_code(http_response::k200Ok);
        resp->set_status_message("OK");
        resp->set_content_type("text/html");
        resp->add_header("Server", "Muduo");
        std::string now = get_current_time();
        resp->set_body("<html><head><title>This is title</title></head>"
                      "<body><h1>Hello</h1>Now is " + now +
                      "</body></html>");
    }
    else if (req.get_path() == "/favicon.ico")
    {
        resp->set_status_code(http_response::k200Ok);
        resp->set_status_message("OK");
        resp->set_content_type("image/png");
        resp->set_body(std::string(favicon, sizeof favicon));
    }
    else if (req.get_path() == "/hello")
    {
        resp->set_status_code(http_response::k200Ok);
        resp->set_status_message("OK");
        resp->set_content_type("text/plain");
        resp->add_header("Server", "Muduo");
        resp->set_body("hello, world!\n");
    }
    else
    {
        resp->set_status_code(http_response::k404NotFound);
        resp->set_status_message("Not Found");
        resp->set_close_connection(true);
    }
}

logger::ptr g_logger;
int main()
{
    auto path = fs::current_path();
    g_logger = std::make_shared<logger>();
    g_logger->init("net", path.c_str(), 1024*1024*50, 1);

    event_loop loop;
    LOG_DEBUG << "begin init server";

    faddress server_addr("127.0.0.1", 6379);
    //echo_server server(&loop, server_addr);
    http_server server(&loop, server_addr, "test");
    server.set_http_callback(on_request);
    LOG_DEBUG << "begin start server";
    server.start();
    LOG_DEBUG << "begin loop server";
    loop.loop();
    return 0;
}

char favicon[555] = {
        '\x89', 'P', 'N', 'G', '\xD', '\xA', '\x1A', '\xA',
        '\x0', '\x0', '\x0', '\xD', 'I', 'H', 'D', 'R',
        '\x0', '\x0', '\x0', '\x10', '\x0', '\x0', '\x0', '\x10',
        '\x8', '\x6', '\x0', '\x0', '\x0', '\x1F', '\xF3', '\xFF',
        'a', '\x0', '\x0', '\x0', '\x19', 't', 'E', 'X',
        't', 'S', 'o', 'f', 't', 'w', 'a', 'r',
        'e', '\x0', 'A', 'd', 'o', 'b', 'e', '\x20',
        'I', 'm', 'a', 'g', 'e', 'R', 'e', 'a',
        'd', 'y', 'q', '\xC9', 'e', '\x3C', '\x0', '\x0',
        '\x1', '\xCD', 'I', 'D', 'A', 'T', 'x', '\xDA',
        '\x94', '\x93', '9', 'H', '\x3', 'A', '\x14', '\x86',
        '\xFF', '\x5D', 'b', '\xA7', '\x4', 'R', '\xC4', 'm',
        '\x22', '\x1E', '\xA0', 'F', '\x24', '\x8', '\x16', '\x16',
        'v', '\xA', '6', '\xBA', 'J', '\x9A', '\x80', '\x8',
        'A', '\xB4', 'q', '\x85', 'X', '\x89', 'G', '\xB0',
        'I', '\xA9', 'Q', '\x24', '\xCD', '\xA6', '\x8', '\xA4',
        'H', 'c', '\x91', 'B', '\xB', '\xAF', 'V', '\xC1',
        'F', '\xB4', '\x15', '\xCF', '\x22', 'X', '\x98', '\xB',
        'T', 'H', '\x8A', 'd', '\x93', '\x8D', '\xFB', 'F',
        'g', '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f', 'v',
        'f', '\xDF', '\x7C', '\xEF', '\xE7', 'g', 'F', '\xA8',
        '\xD5', 'j', 'H', '\x24', '\x12', '\x2A', '\x0', '\x5',
        '\xBF', 'G', '\xD4', '\xEF', '\xF7', '\x2F', '6', '\xEC',
        '\x12', '\x20', '\x1E', '\x8F', '\xD7', '\xAA', '\xD5', '\xEA',
        '\xAF', 'I', '5', 'F', '\xAA', 'T', '\x5F', '\x9F',
        '\x22', 'A', '\x2A', '\x95', '\xA', '\x83', '\xE5', 'r',
        '9', 'd', '\xB3', 'Y', '\x96', '\x99', 'L', '\x6',
        '\xE9', 't', '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',
        '\xA7', '\xC4', 'b', '1', '\xB5', '\x5E', '\x0', '\x3',
        'h', '\x9A', '\xC6', '\x16', '\x82', '\x20', 'X', 'R',
        '\x14', 'E', '6', 'S', '\x94', '\xCB', 'e', 'x',
        '\xBD', '\x5E', '\xAA', 'U', 'T', '\x23', 'L', '\xC0',
        '\xE0', '\xE2', '\xC1', '\x8F', '\x0', '\x9E', '\xBC', '\x9',
        'A', '\x7C', '\x3E', '\x1F', '\x83', 'D', '\x22', '\x11',
        '\xD5', 'T', '\x40', '\x3F', '8', '\x80', 'w', '\xE5',
        '3', '\x7', '\xB8', '\x5C', '\x2E', 'H', '\x92', '\x4',
        '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g', '\x98',
        '\xE9', '6', '\x1A', '\xA6', 'g', '\x15', '\x4', '\xE3',
        '\xD7', '\xC8', '\xBD', '\x15', '\xE1', 'i', '\xB7', 'C',
        '\xAB', '\xEA', 'x', '\x2F', 'j', 'X', '\x92', '\xBB',
        '\x18', '\x20', '\x9F', '\xCF', '3', '\xC3', '\xB8', '\xE9',
        'N', '\xA7', '\xD3', 'l', 'J', '\x0', 'i', '6',
        '\x7C', '\x8E', '\xE1', '\xFE', 'V', '\x84', '\xE7', '\x3C',
        '\x9F', 'r', '\x2B', '\x3A', 'B', '\x7B', '7', 'f',
        'w', '\xAE', '\x8E', '\xE', '\xF3', '\xBD', 'R', '\xA9',
        'd', '\x2', 'B', '\xAF', '\x85', '2', 'f', 'F',
        '\xBA', '\xC', '\xD9', '\x9F', '\x1D', '\x9A', 'l', '\x22',
        '\xE6', '\xC7', '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15',
        '\x90', '\x7', '\x93', '\xA2', '\x28', '\xA0', 'S', 'j',
        '\xB1', '\xB8', '\xDF', '\x29', '5', 'C', '\xE', '\x3F',
        'X', '\xFC', '\x98', '\xDA', 'y', 'j', 'P', '\x40',
        '\x0', '\x87', '\xAE', '\x1B', '\x17', 'B', '\xB4', '\x3A',
        '\x3F', '\xBE', 'y', '\xC7', '\xA', '\x26', '\xB6', '\xEE',
        '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
        '\xA', '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X', '\x0',
        '\x27', '\xEB', 'n', 'V', 'p', '\xBC', '\xD6', '\xCB',
        '\xD6', 'G', '\xAB', '\x3D', 'l', '\x7D', '\xB8', '\xD2',
        '\xDD', '\xA0', '\x60', '\x83', '\xBA', '\xEF', '\x5F', '\xA4',
        '\xEA', '\xCC', '\x2', 'N', '\xAE', '\x5E', 'p', '\x1A',
        '\xEC', '\xB3', '\x40', '9', '\xAC', '\xFE', '\xF2', '\x91',
        '\x89', 'g', '\x91', '\x85', '\x21', '\xA8', '\x87', '\xB7',
        'X', '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N', 'N',
        'b', 't', '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
        '\xEC', '\x86', '\x2', 'H', '\x26', '\x93', '\xD0', 'u',
        '\x1D', '\x7F', '\x9', '2', '\x95', '\xBF', '\x1F', '\xDB',
        '\xD7', 'c', '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF',
        '\x22', 'J', '\xC3', '\x87', '\x0', '\x3', '\x0', 'K',
        '\xBB', '\xF8', '\xD6', '\x2A', 'v', '\x98', 'I', '\x0',
        '\x0', '\x0', '\x0', 'I', 'E', 'N', 'D', '\xAE',
        'B', '\x60', '\x82',
};
