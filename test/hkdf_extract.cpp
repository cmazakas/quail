//
// Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/LeonineKing1199/quail
//

#include <quail/hkdf_extract.hpp>

#include <vector>
#include <algorithm>
#include <array>

#include <catch2/catch.hpp>

TEST_CASE("hkdf_extract")
{
  SECTION("Should be able to extract a shared secret using the RFC test vector")
  {
    auto initial_salt =
      std::vector<unsigned char>{0xc3, 0xee, 0xf7, 0x12, 0xc7, 0x2e, 0xbb, 0x5a, 0x11, 0xa7,
                                 0xd2, 0x43, 0x2b, 0xb4, 0x63, 0x65, 0xbe, 0xf9, 0xf5, 0x02};

    auto client_dst_connection_id =
      std::vector<unsigned char>{0x83, 0x94, 0xc8, 0xf0, 0x3e, 0x51, 0x57, 0x08};

    auto initial_secret = std::array<unsigned char, 32>{0};

    auto const res = quail::hkdf_extract(initial_salt, client_dst_connection_id, initial_secret);
    REQUIRE(res == 1);

    auto const expected_initial_secret =
      std::vector<unsigned char>{0x52, 0x4e, 0x37, 0x4c, 0x6d, 0xa8, 0xcf, 0x8b, 0x49, 0x6f, 0x4b,
                                 0xcb, 0x69, 0x67, 0x83, 0x50, 0x7a, 0xaf, 0xee, 0x61, 0x98, 0xb2,
                                 0x02, 0xb4, 0xbc, 0x82, 0x3e, 0xbf, 0x75, 0x14, 0xa4, 0x23};

    auto const match =
      std::equal(initial_secret.begin(), initial_secret.end(), expected_initial_secret.begin());

    REQUIRE(match);
  }
}
