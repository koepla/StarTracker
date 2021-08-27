#ifndef _HTTPREQUEST_H_
#define _HTTPREQUEST_H_

#include <iostream>
#include <Windows.h>
#include <WinInet.h>
#include <string>

namespace Utils::Http {

	class HttpRequestException : public std::exception {

	private:
		std::string message;

	public:
		HttpRequestException(std::string&& message);

		[[nodiscard]] virtual const char* what() const noexcept;
	};

	class HttpRequest {

	public:
		/*
			input:
			server (const reference)
			url (const reference)

			routine:
			Sends Get request to specified server/url and expects text/html

			returns:
			Response-Data as std::string

			throws:
			HttpRequestException

			example: (surrounded by try-catch block)
			If one wants to send a request to the url "https://ip-api.com/json/",
			The function has to be called like this
			auto responseData = Utils::Http::HttpRequest::Get("ip-api.com", "json");
		*/
		[[nodiscard]] static std::string Get(const std::string& server, const std::string& url) noexcept(false);
	};
}

#endif // _HTTPREQUEST_H_