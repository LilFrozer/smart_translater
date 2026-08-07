#pragma once

#include <string>
#include <memory>

struct TranslationRequest {
    std::string text{""};
    std::string sourceLanguage{""};
    std::string targetLanguage{""};
};

struct TranslationResult {
    std::string text{""};
    bool is_success = false;
    std::string error{""};
};

class ITranslationProvider {
public:
    virtual ~ITranslationProvider() = default;
    virtual TranslationResult doTranslate( const TranslationRequest &request_info ) = 0;
};

// class DictionaryProvider : final public ITranslationProvider, public std::enable_shared_from_this<DictionaryProvider> {
// public:
//     explicit DictionaryProvider( const std::string& dbPath );
//     ~DictionaryProvider() override;
// private:
//     sqlite3* db_ = nullptr;
//     bool available_ = false;
// };

class NetworkProvider final : public ITranslationProvider, public std::enable_shared_from_this<NetworkProvider> {
public:
    explicit NetworkProvider( const std::string &api_key );
    TranslationResult doTranslate( const TranslationRequest &request_info ) override;
private:
    std::string api_key_{""};
    std::string host_{"openrouter.ai"};
    std::string port_{"443"};
    std::string target_{"/api/v1/chat/completions"};
};