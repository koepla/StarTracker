#ifndef LIBTRACKER_CORE_SETTINGS_H
#define LIBTRACKER_CORE_SETTINGS_H

#include <optional>
#include <variant>

#include <nlohmann/json.hpp>

#include "core.hpp"
#include "utility/types.hpp"

class Storage {
public:
    template<typename GenType>
    Storage(const GenType& val) : value{ val } { }

    template<typename GenType>
    std::optional<GenType*> GetReference() noexcept {
        if (const auto type(std::get_if<GenType>(&value)); type) {
            return std::optional<GenType*>{ type };
        }
        return {};
    }

    template<typename GenType>
    bool HasType() const noexcept {
        return std::holds_alternative<GenType>(value);
    }

private:
    std::variant<bool, usize, f64, std::string> value;
};

class Settings {
public:
    static void LoadFromFile(const std::filesystem::path& path) noexcept;

    /**
     * @brief Adds a key-value pair, not that only types of StorageType are allowed for keys
     * @param key The key under which the value shall be stored
     * @param value The actual value
     */
    static void Add(const std::string& key, const Storage& value) noexcept;

    /**
     *
     */
    static void Remove(const std::string& key) noexcept;

    /**
     * @brief Checks if there is an entry with the specified key
     * @param key The key for the lookup
     * @return true If the key was found
     * @return false If the key wasn't found
     */
    static bool Contains(const std::string& key) noexcept;

    /**
     * @brief Get the storage value for the specified key
     * @param key The key for the lookup
     * @return Reference to the storage
     */
    static std::shared_ptr<Storage> GetStorage(const std::string& key) noexcept;

    /**
     * @brief Get the value behind the storage object, if there is no such key, then it will create one
     * @tparam GenType Type for the cast
     * @param key The key for the lookup
     * @param alternative Alternative that is added if there was no such entry in the storage
     * @return Reference to the actual value
     */
    template<typename GenType>
    static GenType& Get(const std::string& key, const GenType& alternative = {}) noexcept {
        if (const auto store = GetStorage(key)) {
            return **store->GetReference<GenType>();
        }
        if (Get<bool>("Output-Verbose")) {
            LIBTRACKER_WARN("Adding property {} as it is not set", key);
        }
        Add(key, alternative);
        return **GetStorage(key)->GetReference<GenType>();
    }

    /**
     * @brief The entire settings formatted as a JSON-string
     * @return JSON
     */
    static std::string GetSerializable() noexcept;

private:
    static inline std::unordered_map<std::string, std::shared_ptr<Storage>> storage;
};

#endif// LIBTRACKER_CORE_SETTINGS_H
