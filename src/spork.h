// Copyright (c) 2014-2024 The Dash Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORK_H
#define SPORK_H

#include "hash.h"
#include "net.h"
#include "utilstrencodings.h"

#include <string>

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what
*/
static const int SPORK_START                                            = 10001;
static const int SPORK_END                                              = 10021;

static const int SPORK_2_INSTANTSEND_ENABLED                            = 10001;
static const int SPORK_3_INSTANTSEND_BLOCK_FILTERING                    = 10002;
static const int SPORK_5_INSTANTSEND_MAX_VALUE                          = 10004;
static const int SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT                 = 10007;
static const int SPORK_9_SUPERBLOCKS_ENABLED                            = 10008;
static const int SPORK_10_MASTERNODE_PAY_UPDATED_NODES                  = 10009;
static const int SPORK_12_RECONSIDER_BLOCKS                             = 10011;
static const int SPORK_13_OLD_SUPERBLOCK_FLAG                           = 10012;
static const int SPORK_14_REQUIRE_SENTINEL_FLAG                         = 10013;
static const int SPORK_15_REQUIRE_FOUNDATION_FEE                        = 10014;
static const int SPORK_16_XELISV2		                        = 10015;
static const int SPORK_17_TIERED_MN		                        = 10016;
static const int SPORK_18_AUTOSPORK		                        = 10018;
static const int SPORK_21_FREEZE_BLACKLIST 				= 10021;

static const int64_t SPORK_2_INSTANTSEND_ENABLED_DEFAULT                = 0;            // ON
static const int64_t SPORK_3_INSTANTSEND_BLOCK_FILTERING_DEFAULT        = 0;            // ON
static const int64_t SPORK_5_INSTANTSEND_MAX_VALUE_DEFAULT              = 1000;         // 1000 PEPEW
static const int64_t SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT_DEFAULT     = 1710183624;   // 11th March 2024 19:00 UTC  - now superblock logic is fixed
static const int64_t SPORK_9_SUPERBLOCKS_ENABLED_DEFAULT                = 4070908800ULL;// OFF
static const int64_t SPORK_10_MASTERNODE_PAY_UPDATED_NODES_DEFAULT      = 4070908800ULL;// OFF
static const int64_t SPORK_12_RECONSIDER_BLOCKS_DEFAULT                 = 0;            // 0 BLOCKS
static const int64_t SPORK_13_OLD_SUPERBLOCK_FLAG_DEFAULT               = 4070908800ULL;// OFF
static const int64_t SPORK_14_REQUIRE_SENTINEL_FLAG_DEFAULT             = 4070908800ULL;// OFF
static const int64_t SPORK_15_REQUIRE_FOUNDATION_FEE_DEFAULT            = 1706814000ULL;// 1st Feb 2024 19:00 UTC
static const int64_t SPORK_16_XELISV2_DEFAULT 			        = 1724905600ULL;// Thursday, 29 August 2024 04:26:40
static const int64_t SPORK_17_TIERED_MN_DEFAULT 		        = 1747418400ULL;// Friday, 16 May 2025 18:00 UTC
static const int64_t SPORK_18_AUTOSPORK_DEFAULT       		        = 1752516000ULL;// Monday, July 14, 2025 18:00:00  GMT
static const int64_t SPORK_21_FREEZE_BLACKLIST_DEFAULT  	        = 1752516000ULL;// Monday, July 14, 2025 18:00:00  GMT

extern std::map<uint256, class CSporkMessage> mapSporks;
extern class CSporkManager sporkManager;
//
// Spork classes
// Keep track of all of the network spork settings
//

class CSporkMessage
{
private:
    std::vector<unsigned char> vchSig;

public:
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;
    std::string sValue; // New: string spork payload

    CSporkMessage(int nSporkID, int64_t nValue, int64_t nTimeSigned) :
        nSporkID(nSporkID),
        nValue(nValue),
        nTimeSigned(nTimeSigned),
        sValue("")
        {}

    // New constructor for string payloads
    CSporkMessage(int nSporkID, const std::string& sValue, int64_t nTimeSigned) :
        nSporkID(nSporkID),
        nValue(0),
        nTimeSigned(nTimeSigned),
        sValue(sValue)
        {}

    CSporkMessage() :
        nSporkID(0),
        nValue(0),
        nTimeSigned(0),
        sValue("")
        {}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
        READWRITE(sValue);
    }

    uint256 GetHash() const
    {
        CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
        ss << nSporkID;
        ss << nValue;
        ss << nTimeSigned;
        ss << sValue;
        return ss.GetHash();
    }

    bool Sign(std::string strSignKey);
    bool CheckSignature();
    void Relay(CConnman& connman);

    std::string GetStringValue() const { return sValue; }
};

class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;
    std::map<int, CSporkMessage> mapSporksActive;

public:

    CSporkManager() {}

    void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv, CConnman& connman);
    void ExecuteSpork(int nSporkID, int nValue);

    // Integer spork interface (legacy/backward compatible)
    bool UpdateSpork(int nSporkID, int64_t nValue, CConnman& connman);
    bool IsSporkActive(int nSporkID);
    int64_t GetSporkValue(int nSporkID);

    // String spork interface (new)
    bool UpdateSpork(int nSporkID, const std::string& sValue, CConnman& connman); // NEW
    bool GetSporkStringValue(int nSporkID, std::string& sValueRet); // NEW

    // Unified getter for string/int (string preferred, falls back to int)
    bool GetSporkValueString(int nSporkID, std::string& sRet); // NEW

    int GetSporkIDByName(std::string strName);
    std::string GetSporkNameByID(int nSporkID);

    bool SetPrivKey(std::string strPrivKey);
};

#endif

