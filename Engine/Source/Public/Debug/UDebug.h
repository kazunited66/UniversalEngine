#pragma once
#include <cstdint>
#include <iostream>
#include <string>
enum LELogType : uint8_t {
	LT_LOG = 0U,
    LT_WARN,
	LT_ERROR,
	LT_SUCCESS

};

class UDebug {
public: 
	//log a message to the console based on the log type 
	static void Log(const std::string& msg, const LELogType& logType = LT_LOG) {
		std::cout << msg << std::endl;
	};
};