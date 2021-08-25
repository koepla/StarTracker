#ifndef _STAR_HTTPREQUEST_H_
#define _STAR_HTTPREQUEST_H_

#include <iostream>
#include <Windows.h>
#include <WinInet.h>
#include <string>

#pragma comment(lib, "wininet.lib")

namespace Util {

	class HTTPRequestException : std::exception {

	private:
		std::string message;

	public:
		HTTPRequestException(std::string&& message) : message(std::move(message)) {


		}

		[[nodiscard]] virtual const char* what() const noexcept {

			return message.c_str();
		}
	};

	class HTTPRequest {

	public:
		[[nodiscard]] static std::string Get(const std::string& server, const std::string& url) noexcept(false);
	};
}

#endif // _STAR_HTTPREQUEST_H_