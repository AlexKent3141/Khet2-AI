#ifndef __COMMS_HANDLER_H__
#define __COMMS_HANDLER_H__

#include <string>

// This class processes incoming messages.
class CommsHandler
{
public:
    bool Process(const std::string&);
};

#endif // __COMMS_HANDLER_H__
