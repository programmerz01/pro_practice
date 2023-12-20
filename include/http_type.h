#ifndef HTTP_CONN_H
#define HTTP_CONN_H


// http 报文请求方法
enum class Http_method {
    Unknown,
    Get,
    Post,
    Head,
    Put,
    Delete,
    Connect,
    Options,
    Trace
};

// http 报文版本
enum class Http_version {
    Unknown,
    Http10,
    Http11,
    Http20
};

// http 报文回复状态码
enum class Http_status_code {
    Unknown = 0,
    OK = 200,
    Moved_Permanently = 301,
    Bad_Request = 400,
    Not_Found = 404,
    Internal_Error = 500
};

#endif //HTTP_CONN_H