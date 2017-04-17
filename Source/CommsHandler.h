#ifndef __COMMS_HANDLER_H__
#define __COMMS_HANDLER_H__

#include "Calculator.h"
#include <string>

// This class processes incoming messages.
class CommsHandler
{
public:
    bool Process(const std::string&);

private:
    Calculator _calculator;
    Board* _board = nullptr;

    Board* CreatePosition(const std::vector<std::string>&) const;
    SearchParams* CreateSearchParameters(const std::vector<std::string>&) const;
};

#endif // __COMMS_HANDLER_H__
