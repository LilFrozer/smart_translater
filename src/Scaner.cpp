#include "Scaner.h"

void MacKeyScaner::callKeyCombination( CGKeyCode keyCode, CGEventFlags modifier ) {
    CGEventRef keyDown = CGEventCreateKeyboardEvent(NULL, keyCode, true);
    CGEventSetFlags(keyDown, modifier);
    CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, keyCode, false);

    CGEventPost(kCGSessionEventTap, keyDown);
    CGEventPost(kCGSessionEventTap, keyUp);

    CFRelease(keyDown);
    CFRelease(keyUp);
}

CGEventRef MacKeyScaner::systemScaning( CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon ) {
    auto ptr = static_cast<MacKeyScaner*>(refcon);
    if (!ptr) {
        return event;
    }

    /*
        -> Игнорируем все, что не связано с клавиатурой!
    */
    if (type != kCGEventKeyDown && type != kCGEventKeyUp) {
        return event;
    }

    /*
        -> Что нажали, код клавиши + модификатор
    */
    CGKeyCode key_code = static_cast<CGKeyCode>(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
    CGEventFlags flags = CGEventGetFlags(event);

    // -> if cmd + b is pressed => we imit press cmd + c
    bool is_cmd_pressed = (flags & kCGEventFlagMaskCommand) != 0;
    bool is_keyB_pressed = (key_code == 11);
    if (is_keyB_pressed && is_cmd_pressed && type == kCGEventKeyDown) {
        ptr->callKeyCombination(8, kCGEventFlagMaskCommand);
        std::thread([ptr]() {
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("pbpaste", "r"), pclose);
            if( !pipe ) {
                throw std::runtime_error("error pbpaste");
            }
            char buffer[128] = {};
            std::string result = "";
            while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
                result += buffer;
            }
            TranslationRequest info;
            info.sourceLanguage = "";
            info.targetLanguage = "";
            info.text = result;
            std::cout << "base = " << result << std::endl;
            ptr->signal_ready(info);
        }).detach();
    }

    return event;
}

void MacKeyScaner::startScan() {
    CFMachPortRef event_tap = CGEventTapCreate(
        kCGSessionEventTap,     // -> только текущий пользователь
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp),
        &MacKeyScaner::systemScaning,
        this
    );

    if ( !event_tap ) {
        throw std::runtime_error("failed to start scanning");
    }

    CFRunLoopSourceRef run_loop = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), run_loop, kCFRunLoopCommonModes);

    CFRunLoopRun();

    CFRelease(run_loop);
    CFRelease(event_tap);
}

