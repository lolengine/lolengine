//
//  Lol Engine
//
//  Copyright © 2010–2024 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine/net>
#include <lol/msg>

#if LOL_USE_OPENSSL
#   define CPPHTTPLIB_OPENSSL_SUPPORT 1
#endif

#if _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#   undef NEAR // avoids errors in <inaddr.h>
#   undef FAR
#   define NEAR
#   define FAR
#endif

#if __NX__ || __SCE__ || _GAMING_XBOX
    // not implemented (yet)
#elif __EMSCRIPTEN__
#   include <emscripten/fetch.h>
#else
#   include <lol/thread>
#   include <httplib.h>
#endif

#include <utility>
#include <memory>
#include <regex>

namespace lol
{

namespace net
{

namespace http
{

class client_impl
{
public:
#if __NX__ || __SCE__ || _GAMING_XBOX
    void get(std::string const &url)
    {
        msg::error("downloading %s failed: not implemented\n", url.c_str());
        m_status = status::error;
    }
#elif __EMSCRIPTEN__
    ~client_impl()
    {
        emscripten_fetch_close(m_fetch);
    }

    void get(std::string const &url)
    {
        emscripten_fetch_attr_t attr;
        emscripten_fetch_attr_init(&attr);
        strcpy(attr.requestMethod, "GET");
        attr.userData = this;
        attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
        attr.onsuccess = client_impl::on_success;
        attr.onerror = client_impl::on_failure;
        m_fetch = emscripten_fetch(&attr, url.c_str());
    }

    static void on_success(emscripten_fetch_t *fetch)
    {
        auto *that = (client_impl *)fetch->userData;
        that->m_result.assign(fetch->data, fetch->numBytes);
        that->m_status = status::success;
    }

    static void on_failure(emscripten_fetch_t *fetch)
    {
        auto *that = (client_impl *)fetch->userData;
        msg::error("downloading %s failed, HTTP failure status code: %d.\n",
                   fetch->url, fetch->status);
        that->m_status = status::error;
    }

    emscripten_fetch_t *m_fetch = nullptr;
#else
    ~client_impl()
    {
        delete m_thread;
    }

    void get(std::string const& url)
    {
        // This regex is copied from cpp-httplib
        const static std::regex re(
            R"(^((?:[^:/?#]+:)?(?://[^/?#]*)?)([^?#]*(?:\?[^#]*)?)(?:#.*)?)");

        std::smatch m;
        if (!std::regex_match(url, m, re))
        {
            m_status = status::error;
            return;
        }

        auto scheme_and_host = m[1].str();
        auto path = m[2].str();

        m_client = std::make_shared<httplib::Client>(scheme_and_host);
        m_client->set_follow_location(true);
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        m_client->enable_server_certificate_verification(false);
#endif

        m_thread = new lol::thread([this, path](thread *)
        {
            auto res = m_client->Get(path);
            if (res && res->status == 200)
            {
                m_result = std::move(res->body);
                m_status = status::success;
            }
            else
            {
                msg::error("downloading %s failed, HTTP failure status code: %d.\n",
                           m_url.c_str(), res ? res->status : -1);
                m_status = status::error;
            }
        });
    }

    lol::thread *m_thread = nullptr;
    std::shared_ptr<httplib::Client> m_client;
#endif

    status m_status;
    std::string m_url;
    std::string m_result;
};

client::client()
  : impl(std::make_unique<client_impl>())
{
}

client::~client()
{
}

void client::get(std::string const &url)
{
    impl->m_status = status::pending;
    impl->m_url = url;
    impl->get(url);
}

void client::reset()
{
    impl->m_status = status::ready;
    impl->m_url.assign("");
    impl->m_result.assign("");
}

status client::status() const
{
    return impl->m_status;
}

std::string const & client::url() const
{
    return impl->m_url;
}

std::string const & client::result() const
{
    return impl->m_result;
}

} // namespace http

} // namespace net

} // namespace lol
