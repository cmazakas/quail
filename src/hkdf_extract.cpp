#include <quail/hkdf_extract.hpp>

#include <memory>

#include <openssl/kdf.h>
#include <openssl/evp.h>

namespace quail
{
auto
hkdf_extract(boost::spans::span<unsigned char>     salt,
             boost::spans::span<unsigned char>     ikm,
             boost::spans::span<unsigned char, 32> out_key) noexcept -> int
{
  auto ptr = std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>(
    EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, nullptr), &EVP_PKEY_CTX_free);

  auto* pctx = ptr.get();
  if (pctx == nullptr) { return 0; }

  auto res = EVP_PKEY_derive_init(pctx);
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_hkdf_mode(pctx, EVP_PKEY_HKDEF_MODE_EXTRACT_ONLY);
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256());
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_set1_hkdf_salt(pctx, salt.data(), static_cast<int>(salt.size()));
  if (res < 1) { return res; }

  res = EVP_PKEY_CTX_set1_hkdf_key(pctx, ikm.data(), static_cast<int>(ikm.size()));
  if (res < 1) { return res; }

  auto keylen = std::size_t{0};
  res         = EVP_PKEY_derive(pctx, out_key.data(), std::addressof(keylen));
  if (res < 1) { return res; }

  return 1;
}
} // namespace quail
