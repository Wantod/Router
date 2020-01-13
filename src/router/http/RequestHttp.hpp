#pragma once

#include <string>
#include <map>

enum class HttpMethod {
    GET,
    HEAD,
    POST,
    POST,
    OPTIONS,
    CONNECT,
    TRACE,
    PUT,
    PATH,
    DELETE
};

enum class HttpVersion {
    HTTP1,
    HTTP1_1,
    HTTP2
};

struct RequestHttp
{
    struct Header {
        std::string key;
        std::string value;
    };

    HttpVersion version;
    HttpMethod method;
    std::string resource;
    std::map<std::string, std::string> headers;
};

static RequestHttp::Header deserialize(const std::string& header)
{
    std::size_t  pos = header.find(':');
    if (pos == std::string::npos) {
        return RequestHttp::Header();
    }
    std::string key(header.substr(0, pos));

    std::size_t  first = header.find_first_not_of(" \t", pos + 1);
    std::size_t  last  = header.find_last_not_of(" \t");
    std::string value(header.substr(first, last - first + 1));

    return RequestHttp::Header { key, value };
}