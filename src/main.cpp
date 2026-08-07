#include <Scaner.h>
using namespace boost::placeholders;

int main(int argv, char *argc[]) {
    try {
        std::shared_ptr<IKeyScaner> key_scanner = std::make_shared<MacKeyScaner>();
        std::shared_ptr<TranslationService> translation_service = std::make_shared<TranslationService>("");
#ifdef __APPLE__
        static_cast<MacKeyScaner*>(key_scanner.get())->signal_ready.connect(
            boost::bind(&TranslationService::doTranslate, translation_service.get(), _1)
        );
#endif
        key_scanner->startScan();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
