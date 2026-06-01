// src/gateways/binance/auth.hpp
#pragma once

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

namespace bluestone::binance {

  class Ed25519Signer {
   public:
    Ed25519Signer(const Ed25519Signer&) = delete;
    Ed25519Signer& operator=(const Ed25519Signer&) = delete;

    explicit Ed25519Signer(const std::string& private_key_pem) {
      BIO* bio = BIO_new_mem_buf(private_key_pem.data(), -1);
      if (!bio)
        throw std::runtime_error("Failed to create BIO for private key");

      pkey_ = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
      BIO_free(bio);

      if (!pkey_) {
        throw std::runtime_error(
            "Failed to load Ed25519 private key. Ensure it is valid PEM "
            "format.");
      }
    }

    ~Ed25519Signer() {
      if (pkey_) EVP_PKEY_free(pkey_);
    }

    // Hot-path consideration: In production, reuse EVP_MD_CTX to avoid
    // allocation
    std::string sign(const std::string& payload) const {
      EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
      if (!md_ctx) throw std::runtime_error("Failed to create EVP_MD_CTX");

      if (EVP_DigestSignInit(md_ctx, nullptr, nullptr, nullptr, pkey_) <= 0) {
        EVP_MD_CTX_free(md_ctx);
        throw std::runtime_error("Failed to initialize Ed25519 signing");
      }

      size_t sig_len = 0;
      // Determine buffer size
      if (EVP_DigestSign(md_ctx, nullptr, &sig_len,
                         reinterpret_cast<const unsigned char*>(payload.data()),
                         payload.size()) <= 0) {
        EVP_MD_CTX_free(md_ctx);
        throw std::runtime_error("Failed to determine signature length");
      }

      std::vector<unsigned char> sig(sig_len);
      // Perform actual signing
      if (EVP_DigestSign(md_ctx, sig.data(), &sig_len,
                         reinterpret_cast<const unsigned char*>(payload.data()),
                         payload.size()) <= 0) {
        EVP_MD_CTX_free(md_ctx);
        throw std::runtime_error("Failed to sign payload");
      }

      EVP_MD_CTX_free(md_ctx);

      // Base64 Encode the signature for Binance
      // EVP_EncodeBlock adds a null terminator, so we allocate sig_len * 2 just
      // to be safe
      std::vector<unsigned char> base64_out(((sig_len + 2) / 3) * 4 + 1);
      int b64_len = EVP_EncodeBlock(base64_out.data(), sig.data(), sig_len);

      return std::string(reinterpret_cast<char*>(base64_out.data()), b64_len);
    }

    // Utility to get current Binance timestamp
    static std::string get_timestamp() {
      auto now = std::chrono::system_clock::now();
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
          now.time_since_epoch());
      return std::to_string(ms.count());
    }

   private:
    EVP_PKEY* pkey_ = nullptr;
  };

}  // namespace bluestone::binance
