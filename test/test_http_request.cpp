#include <cassert>

#include "../include/http_request.h"

void test_request()
{
    std::string request = "POST /login HTTP/1.1\r\n"
                          "Host: 127.0.0.1:10238\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0\r\n"
                          "Accept: */*\r\n"
                          "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Content-Type: application/json\r\n"
                          "Content-Length: 31\r\n"
                          "Origin: http://127.0.0.1:10238\r\n"
                          "Connection: keep-alive\r\n"
                          "Referer: http://127.0.0.1:10238/login.html\r\n"
                          "Cookie: x-hng=lang=zh-CN\r\n"
                          "Sec-Fetch-Dest: empty\r\n"
                          "Sec-Fetch-Mode: cors\r\n"
                          "Sec-Fetch-Site: same-origin\r\n"
                          "\r\n"
                          "{\"username\":\"1\",\"password\":\"1\"}";
    Buffer buff;
    buff.Append(request);

    HttpRequest req;
    assert(req.parse(buff));

    assert(req.get_method() == Http_method::Post);
    assert(req.get_version() == Http_version::Http11);
    assert(req.get_path() == "/login");
    assert(req.get_header("Host") == "127.0.0.1:10238");
    assert(req.get_header("User-Agent") == "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0");
    assert(req.get_header("Accept") == "*/*");
    assert(req.get_header("Accept-Language") == "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    assert(req.get_header("Accept-Encoding") == "gzip, deflate, br");
    assert(req.get_header("Content-Type") == "application/json");
    assert(req.get_header("Content-Length") == "31");
    assert(req.get_header("Origin") == "http://127.0.0.1:10238");
    assert(req.get_header("Connection") == "keep-alive");
    assert(req.get_header("Referer") == "http://127.0.0.1:10238/login.html");
    assert(req.get_header("Cookie") == "x-hng=lang=zh-CN");
    assert(req.get_header("Sec-Fetch-Dest") == "empty");
    assert(req.get_header("Sec-Fetch-Mode") == "cors");
    assert(req.get_header("Sec-Fetch-Site") == "same-origin");
    assert(req.get_body() == "{\"username\":\"1\",\"password\":\"1\"}");
}
void test_parse_request_invalid()
{
    std::string request = "POST /api/users HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Content-Type: application/json\r\n";

    Buffer buffer(1024);
    buffer.Append(request);
    HttpRequest http_request;
    bool result = http_request.parse(buffer);

    // Verify the parsing result
    assert(result == false);
}

int main()
{
    test_request();
    test_parse_request_invalid();
    return 0;
}