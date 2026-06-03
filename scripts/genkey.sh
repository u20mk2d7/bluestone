# 1. Generate the standard private key file
openssl genpkey -algorithm ed25519 -out .env/private-key.pem

# 2. Extract the matching public key file
openssl pkey -in .env/private-key.pem -pubout -out .env/public-key.pem

openssl pkey -in .env/private-key.pem -outform DER | tail -c 32 > .env/raw_seed.bin && \
openssl pkey -in .env/public-key.pem -pubin -outform DER | tail -c 32 >> .env/raw_seed.bin && \
openssl base64 -in .env/raw_seed.bin | tr -d '\n' | tr -d '\r' && echo ""
