#pragma once

#include "TransaltionProvider.h"

class TranslationService {
public:
    explicit TranslationService( const std::string &api_key );
    TranslationResult doTranslate( const TranslationRequest& request_info );
private:
    std::shared_ptr<ITranslationProvider> dict_sql_{nullptr};
    std::shared_ptr<ITranslationProvider> local_ai_{nullptr};
    std::shared_ptr<ITranslationProvider> remote_{nullptr};
};