#include <curl/curl.h>

#include "arch/http-request.hpp"

namespace arch {

    namespace {

        usize CurlWriteCallback(void* content, usize size, usize memberSize, std::string* str) {
            const auto length = size * memberSize;
            try {
                str->append(static_cast<char*>(content), length);
            } catch (std::bad_alloc& e) {
                LIBTRACKER_ERROR("CURL error: {}", e.what());
                return 0;
            }
            return length;
        }
    }// namespace

    std::string HttpRequest::Get(const std::string& server, const std::string& url) noexcept(false) {
        if (const auto curl = curl_easy_init()) {
            std::string buffer;
            const auto path = fmt::format("{}/{}", server, url);
            curl_easy_setopt(curl, CURLOPT_URL, path.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

            const auto result = curl_easy_perform(curl);
            if (result != CURLE_OK) {
                return "";
            }

            curl_easy_cleanup(curl);
            return buffer;
        }
        return "";
    }
}// namespace arch
