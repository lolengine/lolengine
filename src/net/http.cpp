//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if __EMSCRIPTEN__
#   include <emscripten/fetch.h>
#else
#   include <httplib.h>
#endif

namespace lol
{

namespace net
{

namespace http
{

class client_impl
{
public:
#if __EMSCRIPTEN__
    static void on_success(emscripten_fetch_t *fetch)
    {
        auto *that = (client_impl *)fetch->userData;
        msg::info("finished downloading %llu bytes from URL %s.\n",
                  fetch->numBytes, fetch->url);
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
    void get(std::string const& url)
    {

    }

    //httplib::Client &client;
#endif

    status m_status;
    std::string m_result;
};

client::client()
  : impl(std::make_unique<client_impl>())
{
}

client::~client()
{
#if __EMSCRIPTEN__
    emscripten_fetch_close(impl->m_fetch);
#endif
}

void client::get(std::string const &url)
{
    impl->m_status = status::pending;
#if __EMSCRIPTEN__
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.userData = impl.get();
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = client_impl::on_success;
    attr.onerror = client_impl::on_failure;
    impl->m_fetch = emscripten_fetch(&attr, url.c_str());
#else
#endif
}

void client::reset()
{
    impl->m_status = status::ready;
    impl->m_result.assign("");
}

status client::get_status() const
{
    return impl->m_status;
}

std::string const & client::get_result() const
{
    return impl->m_result;
}

} // namespace http

} // namespace net

} // namespace lol

