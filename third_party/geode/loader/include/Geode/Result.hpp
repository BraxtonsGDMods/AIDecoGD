// Minimal Result.hpp shim for CI builds
// This is a lightweight, self-contained implementation to satisfy
// compile-time dependencies while the full Geode SDK headers are
// not available in CI. It intentionally provides a very small API
// surface used by the loader headers in this repository.
#pragma once

#include <optional>
#include <string>
#include <utility>

namespace geode {

template <typename T = void, typename E = std::string>
class Result;

// Specialization for void
template <typename E>
class Result<void, E> {
public:
    Result() noexcept : m_ok(true) {}
    static Result ok() { return Result(true); }
    static Result err(E e) { return Result(false, std::move(e)); }

    explicit operator bool() const noexcept { return m_ok; }
    bool ok() const noexcept { return m_ok; }
    const E& error() const { return *m_err; }

private:
    Result(bool ok, E e = E()) : m_ok(ok), m_err(ok ? std::nullopt : std::optional<E>(std::move(e))) {}
    bool m_ok;
    std::optional<E> m_err;
};

// General Result<T, E>
template <typename T, typename E>
class Result {
public:
    Result() noexcept : m_ok(true), m_val(std::nullopt) {}
    static Result ok(T v) { Result r; r.m_ok = true; r.m_val = std::move(v); return r; }
    static Result err(E e) { Result r; r.m_ok = false; r.m_err = std::move(e); return r; }

    explicit operator bool() const noexcept { return m_ok; }
    bool ok() const noexcept { return m_ok; }
    T& value() { return *m_val; }
    const T& value() const { return *m_val; }
    const E& error() const { return *m_err; }

private:
    bool m_ok{true};
    std::optional<T> m_val;
    std::optional<E> m_err;
};

} // namespace geode
