#include "arch/http-request.hpp"

namespace arch {

    std::string HttpRequest::Get(const std::string& server, const std::string& url) noexcept(false) {
        std::string requestData;

        const auto intSession = InternetOpenA("", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);

        if (!intSession) {
            throw HttpRequestException("Couldn't start internet session");
        }

        const auto httpSession =
                InternetConnectA(intSession, server.c_str(), 80, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);

        if (!httpSession) {
            throw HttpRequestException("Couldn't start HTTP session");
        }

        const auto httpRequest =
                HttpOpenRequestA(httpSession, "GET", url.c_str(), nullptr, nullptr, nullptr, INTERNET_FLAG_RELOAD, 0);

        if (!httpRequest) {
            throw HttpRequestException("Couldn't open HTTP Request");
        }

        using namespace std::literals;
        constexpr auto headers = "Content-Type: text/html\r\n"sv;
        char request[1024] = { 0 };

        if (!HttpSendRequestA(httpRequest, headers.data(), static_cast<DWORD>(headers.size()), request, 0)) {
            throw HttpRequestException("Couldn't send HTTP Request");
        }

        char buffer[1024] = { 0 };
        DWORD read = 0;

        while (InternetReadFile(httpRequest, buffer, sizeof(buffer) - 1, &read) && read) {
            requestData.append(buffer, read);
        }

        InternetCloseHandle(httpRequest);
        InternetCloseHandle(httpSession);
        InternetCloseHandle(intSession);

        return requestData;
    }
}// namespace arch
