//
// Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/LeonineKing1199/quail
//

#include <quail/polyfills/span.hpp>

#include <openssl/kdf.h>
#include <openssl/evp.h>

namespace quail
{
template <class DynamicBuffer>
auto
hkdf_extract(boost::spans::span<unsigned char> salt,
             boost::spans::span<unsigned char> ikm,
             DynamicBuffer                     buffer) -> int
{
  auto* pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, nullptr);
  if (pctx == nullptr) { return 0; }

  auto keylen = std::size_t{0};

  auto const old_size = buffer.size();
  buffer.grow(keylen);
  auto mutable_buffer = buffer.data(old_size, keylen);

  auto res = EVP_PKEY_derive_init(pctx);
  if (res < 1) { goto cleanup; }

  res = EVP_PKEY_CTX_hkdf_mode(pctx, EVP_PKEY_HKDEF_MODE_EXTRACT_ONLY);
  if (res < 1) { goto cleanup; }

  res = EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256());
  if (res < 1) { goto cleanup; }

  res = EVP_PKEY_CTX_set1_hkdf_salt(pctx, salt.data(), static_cast<int>(salt.size()));
  if (res < 1) { goto cleanup; }

  res = EVP_PKEY_CTX_set1_hkdf_key(pctx, ikm.data(), static_cast<int>(ikm.size()));
  if (res < 1) { goto cleanup; }

  res = EVP_PKEY_derive(pctx, nullptr, std::addressof(keylen));
  if (res < 1) { goto cleanup; }

  res = EVP_PKEY_derive(pctx, reinterpret_cast<unsigned char*>(mutable_buffer.data()),
                        std::addressof(keylen));
  if (res < 1) { goto cleanup; }

cleanup:
  EVP_PKEY_CTX_free(pctx);
  return res;
}
} // namespace quail
