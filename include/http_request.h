#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <string>
#include <regex>

#include "http_type.h"
#include "buffer.h"

/* 请求类，用于解析http请求，获取其所有信息 */
class HttpRequest{
public:
    HttpRequest();
    ~HttpRequest();

    bool parse(Buffer& buff);

    /* 获取请求信息 */
    Http_method get_method() const { return method_ ;};
    Http_version get_version() const { return version_; }
    std::string get_path() const { return path_;};
    std::string get_header(const std::string& key) const {
        return headers_.count(key) ? headers_.find(key)->second : "";}
    std::unordered_map<std::string, std::string> get_headers() const { return headers_; };
    std::string get_body() const { return body_; };


    /* for debug */
    std::string toString();

private:
    /* 转化需要用到的内部函数 */
    bool parse_request_line_(std::string_view line);
    bool parse_path_query_(std::string_view query);
    bool parse_header_(std::string_view header);
    bool parse_body_(std::string_view body);

    std::string decode_(std::string_view str);
private:
    /* 请求解析状态 */
    enum class Parse_state {
        RequestLine,
        Headers,
        Body,
        Finish
    };
    Parse_state state_;    

    /* 获取的请求信息 */
    Http_method method_;        // 请求方法
    std::string path_;          // 请求路径
    Http_version version_;      // http版本
    std::unordered_map<std::string, std::string> querys_;      // 请求参数
    std::unordered_map<std::string, std::string> headers_;    // 请求头及其信息
    std::string body_;          // 请求体



    /* 类型与str相互转换 */
    static const std::unordered_map<std::string, Http_method> str_to_method;
    static const std::unordered_map<Http_method, std::string> method_to_str;
    static const std::unordered_map<std::string, Http_version> str_to_version;
    static const std::unordered_map<Http_version, std::string> version_to_str;

};

#endif