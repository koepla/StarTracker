#include "HTTPRequest.hpp"

namespace Util {

	std::string HTTPRequest::Get(const std::string& server, const std::string& url) noexcept(false) {

		std::string requestData = "";

		HINTERNET hIntSession = InternetOpenA("", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

		if (!hIntSession) {

			throw HTTPRequestException("Couldn't start internet session");
		}

		HINTERNET hHttpSession = InternetConnectA(hIntSession, server.c_str(), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);

		if (!hHttpSession) {

			throw HTTPRequestException("Couldn't start HTTP session");
		}

		HINTERNET hHttpRequest = HttpOpenRequestA(hHttpSession, "GET", url.c_str(), 0, 0, 0, INTERNET_FLAG_RELOAD, 0);

		if (!hHttpRequest) {

			throw HTTPRequestException("Couldn't open HTTP Request");
		}

		const char* szHeaders = "Content-Type: text/html\r\n";
		char szRequest[1024] = { 0 };

		if (!HttpSendRequestA(hHttpRequest, szHeaders, strlen(szHeaders), szRequest, strlen(szRequest))) {

			throw HTTPRequestException("Couldn't send HTTP Request");
		}

		CHAR szBuffer[1024] = { 0 };
		DWORD dwRead = 0;

		while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead) {

			requestData.append(szBuffer, dwRead);
		}

		InternetCloseHandle(hHttpRequest);
		InternetCloseHandle(hHttpSession);
		InternetCloseHandle(hIntSession);

		return requestData;
	}
}