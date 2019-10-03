//
// Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/LeonineKing1199/quail
//

#include <quail/hkdf_expand.hpp>

#include <memory>

#include <openssl/kdf.h>
#include <openssl/evp.h>

namespace quail
{
auto
hkdf_expand(boost::spans::span<unsigned char>     prk,
            boost::spans::span<unsigned char>     info,
            boost::spans::span<unsigned char, 32> out_key) noexcept -> int
{
  auto ptr = std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>(
    EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, nullptr), &EVP_PKEY_CTX_free);

  auto* pctx = ptr.get();
  if (pctx == nullptr) { return 0; }

  auto res = EVP_PKEY_derive_init(pctx);
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_hkdf_mode(pctx, EVP_PKEY_HKDEF_MODE_EXPAND_ONLY);
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256());
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_set1_hkdf_key(pctx, prk.data(), static_cast<int>(prk.size()));
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_add1_hkdf_info(pctx, info.data(), static_cast<int>(info.size()));
  if (res < 1) { return res; }

  auto keylen = out_key.size();
  res         = EVP_PKEY_derive(pctx, out_key.data(), std::addressof(keylen));
  if (res < 1) { return res; }

  return 1;
}

} // namespace quail
