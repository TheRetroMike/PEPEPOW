#include <set>
#include <string>
#include <sstream>
#include <vector>

constexpr int64_t FREEZE_SPORK_MAX_SECONDS = 2419200; // 4 weeks in seconds

struct FreezeSporkData {
    std::set<std::string> blacklist;
    int64_t expires = 0; // unix timestamp
    bool valid = false;
};

inline int64_t ClampExpiry(int64_t requested, int64_t now) {
    int64_t maxExpiry = now + FREEZE_SPORK_MAX_SECONDS;
    if (requested == 0) return maxExpiry;
    if (requested > maxExpiry) return maxExpiry;
    return requested;
}

inline FreezeSporkData ParseFreezeSpork(const std::string& value, int64_t now) {
    FreezeSporkData data;
    if (value.empty()) return data;
    size_t blPos = value.find("blacklist:");
    if (blPos == std::string::npos) return data;

    size_t expPos = value.find(";expires:");
    std::string addrPart = value.substr(blPos + 10, (expPos == std::string::npos ? std::string::npos : expPos-blPos-10));
    std::string expPart = expPos == std::string::npos ? "" : value.substr(expPos + 9);

    std::stringstream ss(addrPart);
    std::string addr;
    while (std::getline(ss, addr, ',')) {
        if (!addr.empty())
            data.blacklist.insert(addr);
    }
    int64_t requestedExpiry = 0;
    if (!expPart.empty()) {
        try { requestedExpiry = std::stoll(expPart); } catch (...) { requestedExpiry = 0; }
    }
    data.expires = ClampExpiry(requestedExpiry, now);
    data.valid = !data.blacklist.empty();
    return data;
}

