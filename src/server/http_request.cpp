#include <string_view>
#include <vector>

#include "../../include/http_request.h"
#include "../../include/log.h"

// 类型转换
const std::unordered_map<std::string, Http_method> HttpRequest::str_to_method = {
    {"UNKNOWN", Http_method::Unknown},
    {"GET", Http_method::Get},
    {"POST", Http_method::Post},
    {"HEAD", Http_method::Head},
    {"PUT", Http_method::Put},
    {"DELETE", Http_method::Delete},
    {"CONNECT", Http_method::Connect},
    {"OPTIONS", Http_method::Options},
    {"TRACE", Http_method::Trace}
};
const std::unordered_map<Http_method, std::string> HttpRequest::method_to_str = {
    {Http_method::Get, "GET"},
    {Http_method::Post, "POST"},
    {Http_method::Head, "HEAD"},
    {Http_method::Put, "PUT"},
    {Http_method::Delete, "DELETE"},
    {Http_method::Connect, "CONNECT"},
    {Http_method::Options, "OPTIONS"},
    {Http_method::Trace, "TRACE"}
};
const std::unordered_map<std::string, Http_version> HttpRequest::str_to_version = {
    {"HTTP/1.0", Http_version::Http10},
    {"HTTP/1.1", Http_version::Http11},
    {"HTTP/2.0", Http_version::Http20}
};
const std::unordered_map<Http_version, std::string> HttpRequest::version_to_str = {
    {Http_version::Http10, "HTTP/1.0"},
    {Http_version::Http11, "HTTP/1.1"},
    {Http_version::Http20, "HTTP/2.0"}
};

HttpRequest::HttpRequest() {
    state_ = Parse_state::RequestLine;
    method_ = Http_method::Unknown;
    version_ = Http_version::Unknown;
    path_ = "";
    headers_.clear();
    body_ = "";
}

HttpRequest::~HttpRequest() { }

bool HttpRequest::parse(Buffer& buff) {
    const std::string CRLF = "\r\n";
    std::string_view buffer_view(buff.Peek(), buff.ReadableBytes());
    std::string_view one_line;
    size_t pos = 0; // pos为待分析的行的起始位置
    size_t line_end;    // line_end为待分析的行的结束位置

    if(buff.ReadableBytes() <= 0) {
        return false;
    }

    while(buff.ReadableBytes() && state_ != Parse_state::Finish) {
        line_end = buffer_view.find(CRLF, pos);

        switch(state_)
        {
        // 请求行
        case Parse_state::RequestLine:
            // 未获取完整的行
            if(line_end == std::string_view::npos || line_end == pos) {
                LOG_ERROR("Can't get request line :\n%s\n", std::string(buffer_view).c_str());
                return false;
            }

            // 分析请求行
            one_line = buffer_view.substr(pos, line_end - pos);
            if (!parse_request_line_(one_line))
            {
                LOG_ERROR("Parse request failed: \n%s\n Error line: %s\n", std::string(buffer_view).c_str(), std::string(one_line).c_str());
                return false;
            }
            break;    

        // 头
        case Parse_state::Headers: 
            if(line_end == std::string_view::npos) {
                return false;
            }

            // 逐行分析头
            one_line = buffer_view.substr(pos, line_end - pos);
            if (!parse_header_(one_line))
            {
                LOG_INFO("Parse request failed: \n%s\n Error line: %s\n", std::string(buffer_view), std::string(one_line) );
                return false;
            }
            break;

        // 请求体
        case Parse_state::Body: 
            if(headers_.count("Content-Length") == 0 ){
                if (one_line.empty()){
                    state_ = Parse_state::Finish;
                    pos -= CRLF.size();
                    break;
                }
                body_ = std::string(buffer_view.substr(pos));
            }else{
                size_t len = std::stoi(headers_["Content-Length"]);
                if (buffer_view.size() - pos < len){
                    return false;
                }
                body_ = std::string(buffer_view.substr(pos, len));
            }

            // 读取完请求体，解析结束
            state_ = Parse_state::Finish;
            pos -= CRLF.size();
            break;
        default:
            break;
        }
        pos == line_end + CRLF.size();
    }
    return true;
}

// 分析请求行，获取http方法、资源路径、http版本
bool HttpRequest::parse_request_line_(std::string_view line) {
    std::vector<std::string_view> line_split;
    size_t pos = 0, end = 0, qmark_pos = 0;

    while((end = line.find(' ', pos)) != std::string_view::npos) {
        line_split.push_back(line.substr(pos, end - pos));
        pos = end + 1;
    }
    line_split.push_back(line.substr(pos));

    if(line_split.size() != 3) {
        return false;
    }

    // 解析出的信息
    try {
        method_ = str_to_method.at(std::string(line_split[0]));
        version_ = str_to_version.at(std::string(line_split[2]));
    }
    catch(const std::exception& e) {
        return false;
    }
    path_ = std::string(line_split[1]);
    qmark_pos = path_.find('?');
    if(qmark_pos != std::string::npos) {
        parse_path_query_(path_.substr(qmark_pos + 1));
        path_ = path_.substr(0, qmark_pos);
    }

    return true;
}

// 分析单行头，获取头信息
bool HttpRequest::parse_header_(std::string_view header) {
    if(header.empty()) {
        // 到达空行，头部分析结束
        state_ = Parse_state::Body;
        return true;
    }
    size_t colon_pos = header.find(':');
    if(colon_pos == std::string_view::npos) {
        return false;
    }

    std::string_view key = header.substr(0, colon_pos);
    std::string_view value = header.substr(colon_pos + 2);
    headers_[std::string(key)] = std::string(value);
    return true;
}

// 分析请求体
bool HttpRequest::parse_body_(std::string_view body) {
    if(headers_.count("Content-Length") == 0) {
        state_ = Parse_state::Finish;
        return true;
    }
    return false;
}

bool HttpRequest::parse_path_query_(std::string_view query)
{
    if(query.empty()) {
        return false;
    }

    // 分析参数
    size_t pos = 0, end = 0;
    while((end = query.find('&', pos)) != std::string_view::npos) {
        std::string_view one_query = query.substr(pos, end - pos);
        size_t equal_pos = one_query.find('=');
        if(equal_pos == std::string_view::npos) {
            return false;
        }
        querys_[std::string(one_query.substr(0, equal_pos))] = decode_(one_query.substr(equal_pos + 1));
        pos = end + 1;
    }
    std::string_view one_query = query.substr(pos);
    size_t equal_pos = one_query.find('=');
    if(equal_pos == std::string_view::npos) {
        return false;
    }
    querys_[std::string(one_query.substr(0, equal_pos))] = decode_(one_query.substr(equal_pos + 1));

    return true;
}

std::string HttpRequest::decode_(std::string_view str)
{
    std::string result;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '%' && i + 2 < str.size())
        {
            int value;
            std::istringstream iss(std::string(str.substr(i + 1, 2)));
            if (iss >> std::hex >> value)
            {
                result += static_cast<char>(value);
                i += 2;
            }
            else
                result += '%';
        }
        else if (str[i] == '+')
            result += ' ';
        else
            result += str[i];
    }
    return result;
}