//
// Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/LeonineKing1199/quail
//

#include <quail/hkdf_expand.hpp>

#include <vector>
#include <algorithm>
#include <array>

#include <catch2/catch.hpp>

TEST_CASE("hkdf_expand")
{
  SECTION("Should be able to expand a shared secret using the RFC test vector")
  {
    auto prk =
      std::vector<unsigned char>{0x52, 0x4e, 0x37, 0x4c, 0x6d, 0xa8, 0xcf, 0x8b, 0x49, 0x6f, 0x4b,
                                 0xcb, 0x69, 0x67, 0x83, 0x50, 0x7a, 0xaf, 0xee, 0x61, 0x98, 0xb2,
                                 0x02, 0xb4, 0xbc, 0x82, 0x3e, 0xbf, 0x75, 0x14, 0xa4, 0x23};

    auto client_in =
      std::vector<unsigned char>{0x00, 0x20, 0x0f, 0x74, 0x6c, 0x73, 0x31, 0x33, 0x20, 0x63,
                                 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x20, 0x69, 0x6e, 0x00};

    auto client_initial_secret = std::array<unsigned char, 32>{0};

    auto res = quail::hkdf_expand(prk, client_in, client_initial_secret);
    REQUIRE(res == 1);

    auto const expected_client_initial_secret = std::array<unsigned char, 32>{
      0xfd, 0xa3, 0x95, 0x3a, 0xec, 0xc0, 0x40, 0xe4, 0x8b, 0x34, 0xe2,
      0x7e, 0xf8, 0x7d, 0xe3, 0xa6, 0x09, 0x8e, 0xcf, 0x0e, 0x38, 0xb7,
      0xe0, 0x32, 0xc5, 0xc5, 0x7b, 0xcb, 0xd5, 0x97, 0x5b, 0x84};

    auto const match = std::equal(client_initial_secret.begin(), client_initial_secret.end(),
                                  expected_client_initial_secret.begin());

    REQUIRE(match);
  }
}
