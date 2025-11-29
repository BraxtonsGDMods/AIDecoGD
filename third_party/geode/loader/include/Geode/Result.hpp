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

// Lightweight Result type compatible with the subset of Geode's API used
// by the loader headers. This implements a simple Ok/Err result, with
// unwrap/unwrapErr helpers and convenience functions `Ok` and `Err`.
// Note: this is not feature-complete; it provides the methods required
// for CI to compile and the project to link. Replace with the upstream
// implementation when available.

// Specialization for void
template <typename E>
class Result<void, E> {
public:
    Result() noexcept : m_ok(true) {}
    explicit Result(bool ok, std::optional<E> err = std::nullopt) noexcept : m_ok(ok), m_err(std::move(err)) {}

    explicit operator bool() const noexcept { return m_ok; }
    bool ok() const noexcept { return m_ok; }

    // Returns an optional error value
    std::optional<E> err() const noexcept { return m_err; }

    // If used as a void result, unwrap() does nothing (mimics upstream)
    void unwrap() const {}

    // Get the error (throws std::runtime_error if called on success)
    E unwrapErr() const {
        if (m_ok) throw std::runtime_error("called unwrapErr() on Ok result");
        return *m_err;
    }

private:
    bool m_ok{true};
    std::optional<E> m_err;
};

// General Result<T, E>
template <typename T, typename E>
class Result {
public:
    Result() noexcept : m_ok(true), m_val(std::nullopt) {}
    explicit Result(T v) : m_ok(true), m_val(std::move(v)) {}
    explicit Result(E e, bool isErr) : m_ok(!isErr), m_err(std::move(e)) {}

    explicit operator bool() const noexcept { return m_ok; }
    bool ok() const noexcept { return m_ok; }

    // Accessors
    std::optional<T> value_opt() const noexcept { return m_val; }
    T unwrap() const {
        if (!m_ok) throw std::runtime_error("called unwrap() on Err result");
        return *m_val;
    }
    std::optional<E> err() const noexcept { return m_err; }
    E unwrapErr() const {
        if (m_ok) throw std::runtime_error("called unwrapErr() on Ok result");
        return *m_err;
    }

    // map helpers
    template <typename F>
    auto map(F&& fn) const -> Result<std::invoke_result_t<F, T>, E> {
        using U = std::invoke_result_t<F, T>;
        if (!m_ok) return Result<U, E>(*m_err, true);
        return Result<U, E>(fn(*m_val));
    }

    template <typename G>
    auto mapErr(G&& fn) const -> Result<T, std::invoke_result_t<G, E>> {
        using F = std::invoke_result_t<G, E>;
        if (m_ok) return Result<T, F>(*m_val);
        return Result<T, F>(fn(*m_err), true);
    }

private:
    bool m_ok{true};
    std::optional<T> m_val;
    std::optional<E> m_err;
};

// Convenience helpers to match upstream usage: `Ok(...)` and `Err(...)`.
template <typename E = std::string>
Result<void, E> Ok() { return Result<void, E>(true, std::nullopt); }

template <typename T, typename E = std::string>
Result<T, E> Ok(T v) { return Result<T, E>(std::move(v)); }

template <typename E = std::string>
Result<void, E> Err(E e) { return Result<void, E>(false, std::move(e)); }

template <typename T, typename E = std::string>
Result<T, E> Err(E e) { return Result<T, E>(std::move(e), true); }

} // namespace geode
