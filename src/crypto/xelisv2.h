// replace license

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_XELISV2_H
#define BITCOIN_CRYPTO_XELISV2_H

void xelis_hash_v2(const void* data, size_t len, uint8_t hashResult[32]);

//template<typename T1>
//void pre_xelis_hash_v2(const T1 pbegin, const T1 pend, uint8_t hash_result[32]);

#endif // BITCOIN_CRYPTO_XELISV2_H
