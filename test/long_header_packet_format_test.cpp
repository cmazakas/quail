//
// Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/LeonineKing1199/quail
//

#pragma comment(lib, "crypt32.lib")

#include <quail/packet_formats/initial.hpp>

#include <boost/asio/buffer.hpp>
#include <boost/utility/string_view.hpp>

#include <openssl/kdf.h>
#include <openssl/evp.h>

#include <array>
#include <cstring>

#include <catch2/catch.hpp>

TEST_CASE("long_header_packet_format_test")
{
  SECTION("we should be able to parse a well-formed initial packet into its constituent parts")
  {
    auto* initial_salt             = "0xc3eef712c72ebb5a11a7d2432bb46365bef9f502";
    auto* client_dst_connection_id = "0x1234";

    EVP_PKEY_CTX* pctx = nullptr;

    pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);

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

    EVP_PKEY_CTX_free(pctx);
  }
}
