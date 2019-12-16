#pragma once

#include "../Type.hpp"
#include <map>

struct ParserRequest {
	char address[1024];
	char port[10];
};

// class UrlType {
// public:
// 	int type;
// 	std::string protocol;
// 	std::string host;
// 	std::string path;

// 	static UrlType parse(std::string url) {
// 		UrlType urlType;

// 		std::size_t pos = url.find("://");
// 		if (pos != std::string::npos) {
// 			urlType.type = 1;
// 			urlType.protocol = url.substr(0, pos);
// 			url.erase(0, pos + 3); // "://"
// 		} else {
// 			urlType.protocol = "http";
// 		}

// 		pos = url.find("/");
// 		if (pos != std::string::npos) {
// 			urlType.host = url.substr(0, pos);
// 			urlType.path = url.substr(pos);
// 		} else {
// 			urlType.host = url;
// 			urlType.path = "";
// 		}

// 		return urlType;
// 	}

// 	std::string toString() {
// 		return protocol + "://" + host + path;
// 	}
// };

// struct HttpParser
// {
// 	enum class Type {
// 		Empty,
// 		Request,
// 		Response
// 	};

// 	Type type;
// 	int status;
// 	std::string protocol;
// 	std::string method;
// 	std::string url;
// 	std::map<std::string, std::string> header;

// 	int code;
// 	std::string reason;
// 	std::string data;

// 	HttpParser() {
// 		type = Type::Empty;
// 		status = 0;
// 		protocol = "HTTP/1.1";
// 	}
// };

// HttpParser createHttpRequest() {
// 	HttpParser req;
// 	req.type = HttpParser::Type::Request;
// 	return req;
// }

// HttpParser createHttpResponse() {
// 	HttpParser res;
// 	res.type = HttpParser::Type::Response;
// 	res.code = 200;
// 	res.reason = "OK";
// 	return res;
// }

// void HttpString(HttpParser &http) {
// 	std::string ret;

// 	if (http.type == HttpParser::Type::Request)
// 	{
// 		std::string url = UrlType::parse(http.url).toString();
// 		ret += http.method + " " + url + " " + http.protocol;
// 	}
// 	else
// 	{
// 		ret += http.protocol + " " + std::to_string(http.code) + " " + http.reason;
// 	}
	

// 	for (auto header : http.header)
// 	{
// 		ret += header.first + ": " + header.second + "\r\n";
// 	}

// 	ret += "\r\n";
// 	ret += http.data;
// }