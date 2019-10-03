//
// Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/LeonineKing1199/quail
//

#include <quail/polyfills/span.hpp>
#include <array>

namespace quail
{
auto
hkdf_extract(boost::spans::span<unsigned char>     salt,
             boost::spans::span<unsigned char>     ikm,
             boost::spans::span<unsigned char, 32> out_key) -> int;

} // namespace quail
