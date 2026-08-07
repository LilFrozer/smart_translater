#include "TranslationService.h"
#include <iostream>

TranslationService::TranslationService( const std::string &api_key ) :
    remote_(std::make_shared<NetworkProvider>(api_key)) {
}

TranslationResult TranslationService::doTranslate( const TranslationRequest& request_info ) {
    if (remote_) {
        auto r = remote_->doTranslate(request_info);
        std::cout << "res = " << r.text << std::endl;
        return r;
    }

    return {};
}