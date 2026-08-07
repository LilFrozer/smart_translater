
#pragma once

#include <memory>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "TranslationService.h"
#include <boost/signals2.hpp>

class IKeyScaner {
public:
    virtual ~IKeyScaner() = default;
    virtual void startScan() = 0;
};

// class Win32Scaner final : public IKeyScaner, public std::enable_shared_from_this<Win32Scaner> {
// public:
//     explicit Win32Scaner();
//     ~Win32Scaner() override;
//     void startScan() override;
// };

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

class MacKeyScaner final : public IKeyScaner, public std::enable_shared_from_this<MacKeyScaner> {
public:
    explicit MacKeyScaner() = default;
    ~MacKeyScaner() override = default;
    void startScan() override;
    boost::signals2::signal<void( const TranslationRequest &request_info )> signal_ready;
private:
    static CGEventRef systemScaning( CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon );
    void callKeyCombination( CGKeyCode keyCode, CGEventFlags modifier );
};