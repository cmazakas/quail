//
// Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/LeonineKing1199/quail
//

#include <quail/packet_formats/initial.hpp>

#include <boost/asio/buffer.hpp>
#include <boost/utility/string_view.hpp>

#include <openssl/kdf.h>
#include <openssl/evp.h>

#include <array>
#include <cstring>
#include <iostream>

#include <catch2/catch.hpp>

TEST_CASE("long_header_packet_format_test")
{
  SECTION("we should be able to parse a well-formed initial packet into its constituent parts")
  {
    auto* initial_salt =
      "\xc3\xee\xf7\x12\xc7\x2e\xbb\x5a\x11\xa7\xd2\x43\x2b\xb4\x63\x65\xbe\xf9\xf5\x02";

    auto* client_dst_connection_id = "\x83\x94\xc8\xf0\x3e\x51\x57\x08";

    EVP_PKEY_CTX* pctx = nullptr;

    pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, nullptr);

    auto res = EVP_PKEY_derive_init(pctx);
    REQUIRE(res == 1);

    res = EVP_PKEY_CTX_hkdf_mode(pctx, EVP_PKEY_HKDEF_MODE_EXTRACT_ONLY);
    REQUIRE(res == 1);

    res = EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256());
    REQUIRE(res == 1);

    res =
      EVP_PKEY_CTX_set1_hkdf_salt(pctx, initial_salt, static_cast<int>(std::strlen(initial_salt)));
    REQUIRE(res == 1);

    res = EVP_PKEY_CTX_set1_hkdf_key(pctx, client_dst_connection_id,
                                     static_cast<int>(std::strlen(client_dst_connection_id)));

    REQUIRE(res == 1);

    auto out     = std::array<unsigned char, 4096>{0};
    auto out_len = std::size_t{0};

    res = EVP_PKEY_derive(pctx, out.data(), std::addressof(out_len));
    REQUIRE(res == 1);
    REQUIRE(out_len > 0);

    auto const initial_secret = boost::string_view(reinterpret_cast<char*>(out.data()), out_len);

    REQUIRE(initial_secret ==
            "\x52\x4e\x37\x4c\x6d\xa8\xcf\x8b\x49\x6f\x4b\xcb\x69\x67\x83\x50\x7a\xaf\xee\x61\x98"
            "\xb2\x02\xb4\xbc\x82\x3e\xbf\x75\x14\xa4\x23");

    EVP_PKEY_CTX_free(pctx);
  }
}
