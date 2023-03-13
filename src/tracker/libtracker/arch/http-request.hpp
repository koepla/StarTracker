#ifndef LIBTRACKER_ARCH_HTTPREQUEST_H
#define LIBTRACKER_ARCH_HTTPREQUEST_H

#include <string>

#include "arch/os.hpp"
#include "core/core.hpp"
#include "utility/types.hpp"

namespace arch {

    class HttpRequestException : public std::exception {
    private:
        std::string message;

    public:
        explicit HttpRequestException(std::string_view message) noexcept;

        const char* what() const noexcept override;
    };

    class HttpRequest {
    public:
        /**
         * @brief Performs HttpRequest to the specified server and url
         * @param server name of the server (without the protocol name as http is the only supported one)
         * @param url url path
         * @return response data as std::string
         * @throws HttpRequestException if the HttpRequest fails
         */
        static std::string Get(const std::string& server, const std::string& url) noexcept(false);
    };
}// namespace arch

#endif// LIBTRACKER_ARCH_HTTPREQUEST_H