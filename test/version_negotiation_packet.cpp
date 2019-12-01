#define CATCH_CONFIG_MAIN

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/binary.hpp>

#include <vector>
#include <iostream>

#include <catch2/catch.hpp>

namespace x3 = boost::spirit::x3;

using big_byteuint_least8_t =
  x3::any_binary_parser<std::uint_least8_t, boost::endian::order::big, 8>;

auto const big_byte_ = big_byteuint_least8_t();

x3::rule<struct version_negotation_packet_t> const version_negotiation_packet =
  "version_negotiation_packet";

//  The layout of a Version Negotiation packet is:
//
//   0                   1                   2                   3
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+
//  |1|  Unused (7) |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                          Version (32)                         |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  | DCID Len (8)  |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |               Destination Connection ID (0..2040)           ...
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  | SCID Len (8)  |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                 Source Connection ID (0..2040)              ...
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                    Supported Version 1 (32)                 ...
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                   [Supported Version 2 (32)]                ...
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                                 ...
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                   [Supported Version N (32)]                ...
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//  The value in the Unused field is selected randomly by the server.
//  Clients MUST ignore the value of this field.  Servers SHOULD set the
//  most significant bit of this field (0x40) to 1 so that Version
//  Negotiation packets appear to have the Fixed Bit field.

auto const valid_first = [](auto& ctx) {
  if ((x3::_attr(ctx) & 0xc0) == 0) { x3::_pass(ctx) = false; }
};

auto const version_negotiation_packet_def = big_byte_[valid_first] >> x3::big_dword;
BOOST_SPIRIT_DEFINE(version_negotiation_packet);

TEST_CASE("version_negotiation_packet")
{
  SECTION("should be able to identify a simple test packet")
  {
    auto const packet = std::vector<unsigned char>{0xff, 0x00, 0x00, 0x00, 0x00};

    auto pos = packet.begin();

    auto const match = x3::parse(pos, packet.end(), version_negotiation_packet);

    CHECK(match);
    CHECK(pos == packet.end());
  }

  SECTION("should reject packets without the fixed big field")
  {
    auto const packet = std::vector<unsigned char>{0x3f, 0x00, 0x00, 0x00, 0x00};

    auto pos = packet.begin();

    auto const match = x3::parse(pos, packet.end(), version_negotiation_packet);

    CHECK_FALSE(match);
    CHECK(pos != packet.end());
  }
}
