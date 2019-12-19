// #pragma once

// #include <iostream>
// #include <vector>
// #include <string>

// // #define CHECK_SUCCESS(expr) do { std::cout << "[OK] " #expr << std::endl; } while(0)
// // #define CHECK_FAILURE(expr) do { std::cout << "[FAILURE] " #expr << std::endl; } while(0)
// #define CHECK(expr) do { if (expr) { setSuccess(#expr, __FILE__, __LINE__); } else { setError(#expr, __FILE__, __LINE__); } } while(0)
// #define EQUAL(a, b, message) do { if (a == b) { setSuccess(#a #b,  __FILE__, __LINE__); } else { setError(#a" == "#b" message: "#message, __FILE__, __LINE__); } } while(0)

// static int error_count = 0;
// enum class ErrorType {
// 	ERROR,
// 	SUCCESS,
// 	WARNING
// };

// struct ErrorInfo {
// 	ErrorType type;
// 	std::string str;
// };

// static std::vector<ErrorInfo> error_list;

// static void setError(char *str, char *file, int line) {
// 	error_count++;
// 	std::cout << "\x1b[31;1m✘ ERROR \x1b[0m" << file << " (ligne " << line << "):\t" << str << std::endl;
// 	error_list.push_back({ ErrorType::ERROR, "\x1b[31;1m✘ ERROR \x1b[0m" + std::string(file) + " (ligne " + std::to_string(line) + "):\t" + std::string(str) });
// }

// static void setSuccess(char *str, char *file, int line) {
// 	std::cout << "\x1b[32;1m✔ SUCCESS \x1b[0m" << file << " (ligne " << line << "):\t" << str << std::endl;
// 	error_list.push_back({ ErrorType::SUCCESS, std::string("\x1b[32;1m✔ SUCCESS \x1b[0m") + std::string(file) + " (ligne " + std::to_string(line) + "):\t" + std::string(str) });
// }


// static int getNumberError() {
// 	return error_count;
// }

// static void displaySumaryError() {
// 	std::cout << "SUMMARY ERROR" << std::endl;
// 	for (auto &message : error_list)
// 	{
// 		if (message.type == ErrorType::ERROR)
// 		{
// 			std::cout << "\t" << message.str << std::endl;
// 		}
// 	}

// 	std::cout << "total: \x1b[31;1m✘  ERROR \x1b[0m" << getNumberError() <<  ", \x1b[32;1m✔ SUCCESS\x1b[0m " << (error_list.size() - getNumberError())  << std::endl;
// }