// Minimal matjson.hpp shim for CI compilation
// This is intentionally lightweight — it implements the small portion
// of the matjson API used by the Geode loader headers so CI can compile.
// Replace with the real matjson dependency for full behavior.
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace matjson {

enum class Type {
    Null,
    Object,
    Array,
    String,
    Bool,
    Number,
};

// Minimal Result-like return object used by matjson::Value::as<T>()
template <typename T>
struct Result {
    bool ok_ = false;
    std::optional<T> val;
    std::optional<std::string> err;

    explicit operator bool() const noexcept { return ok_; }
    bool ok() const noexcept { return ok_; }
    T unwrap() const { return *val; }
    std::string unwrapErr() const { return err.value_or(std::string{}); }
};

// Very small placeholder Value type that supports the subset of the
// interface used in the Geode headers: type(), as<T>(), asString(), asInt(), asArray().
class Value {
public:
    using Array = std::vector<Value>;

    Value() : m_type(Type::Null), m_value(std::monostate{}) {}
    explicit Value(std::string s) : m_type(Type::String), m_value(std::move(s)) {}
    explicit Value(int n) : m_type(Type::Number), m_value(static_cast<double>(n)) {}
    explicit Value(double d) : m_type(Type::Number), m_value(d) {}
    explicit Value(bool b) : m_type(Type::Bool), m_value(b) {}
    explicit Value(Array a) : m_type(Type::Array), m_value(std::move(a)) {}

    Type type() const noexcept { return m_type; }

    // as<T>() minimal behavior: support int, double, std::string, Array
    Result<int> asInt() const noexcept {
        if (m_type == Type::Number) {
            if (auto p = std::get_if<double>(&m_value)) return Result<int>{true, static_cast<int>(*p), std::nullopt};
        }
        return Result<int>{false, std::nullopt, std::string("not a number")};
    }

    Result<std::string> asString() const noexcept {
        if (m_type == Type::String) {
            if (auto p = std::get_if<std::string>(&m_value)) return Result<std::string>{true, *p, std::nullopt};
        }
        return Result<std::string>{false, std::nullopt, std::string("not a string")};
    }

    Result<Array> asArray() const noexcept {
        if (m_type == Type::Array) {
            if (auto p = std::get_if<Array>(&m_value)) return Result<Array>{true, *p, std::nullopt};
        }
        return Result<Array>{false, std::nullopt, std::string("not an array")};
    }

    template <typename T>
    Result<T> as() const noexcept {
        // Basic scaffolding; tests and upstream usage generally use
        // asInt/asString/asArray. If called for an unsupported type,
        // return error.
        return Result<T>{false, std::nullopt, std::string("not supported by shim")};
    }

private:
    Type m_type;
    std::variant<std::monostate, std::string, double, bool, Array> m_value;
};

} // namespace matjson
