#include "settings.hpp"

#include "arch/file.hpp"

void Settings::LoadFromFile(const std::filesystem::path& path) noexcept {
    auto data = arch::ReadFile(path);
    auto json = nlohmann::json::parse(std::move(data), nullptr, false, true);
    for (nlohmann::json::iterator entryIterator = json.begin(); entryIterator != json.end(); ++entryIterator) {
        Add(entryIterator.key(), std::invoke([&]() -> Storage {
                auto& value = entryIterator.value();
                switch (value.type()) {
                    case nlohmann::json::value_t::boolean:
                        return value.get<bool>();
                    case nlohmann::json::value_t::number_integer:
                    case nlohmann::json::value_t::number_unsigned:
                        return value.get<usize>();
                    case nlohmann::json::value_t::number_float:
                        return value.get<f64>();
                    case nlohmann::json::value_t::string:
                        return value.get<std::string>();
                    default:
                        LIBTRACKER_ASSERT(false, "Invalid settings type");
                        return nullptr;
                }
            }));
    }
}

void Settings::Add(const std::string& key, const Storage& value) noexcept {
    storage[key] = std::make_shared<Storage>(value);
}

void Settings::Remove(const std::string& key) noexcept {
    if (Contains(key)) {
        storage.erase(key);
    }
}

bool Settings::Contains(const std::string& key) noexcept {
    return storage.find(key) != storage.end();
}

std::shared_ptr<Storage> Settings::GetStorage(const std::string& key) noexcept {
    if (!Contains(key)) {
        return nullptr;
    }
    return storage[key];
}

std::string Settings::GetSerializable() noexcept {
    nlohmann::json serializable;
    for (auto& [key, value] : storage) {
        if (value->HasType<bool>()) {
            serializable[key] = **value->GetReference<bool>();
        } else if (value->HasType<usize>()) {
            serializable[key] = **value->GetReference<usize>();
        } else if (value->HasType<f64>()) {
            serializable[key] = **value->GetReference<f64>();
        } else if (value->HasType<std::string>()) {
            serializable[key] = **value->GetReference<std::string>();
        }
    }
    return serializable.dump(1, '\t');
}