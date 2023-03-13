#include "async.hpp"

namespace utility {

    void FireAndForget(AsyncTask&& task) noexcept {
        auto job = std::thread(task);
        job.detach();
    }
}// namespace utility