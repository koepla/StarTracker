#ifndef STARTRACKER_UTILS_HTTPREQUEST_H
#define STARTRACKER_UTILS_HTTPREQUEST_H

#include <Windows.h>
#include <WinInet.h>
#include <string>

namespace StarTracker::Utils::Http {

	class HttpRequestException : public std::exception {

	private:
		std::string message;

	public:
		explicit HttpRequestException(std::string&& message) noexcept;

		[[nodiscard]] virtual const char* what() const noexcept;
	};

	class HttpRequest {

	public:
		/**
		* Performs HttpRequest to the specified server and url
		*
		* @param server name of the server (without the protocol name as http is the only supported one)
		* 
		* @param url url path
		*
		* @return response data as std::string
		*
		* @throws HttpRequestException if the HttpRequest fails
		* 
		*/
		[[nodiscard]] static std::string Get(const std::string& server, const std::string& url) noexcept(false);
	};
}

#endif // STARTRACKER_UTILS_HTTPREQUEST_H