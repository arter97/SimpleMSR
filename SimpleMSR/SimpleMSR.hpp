//
//  SimpleMSR.hpp
//  SimpleMSR
//
//  Created by Park Ju Hyung on 30/06/2018.
//  Copyright © 2018 Park Ju Hyung. All rights reserved.
//

#ifndef SimpleMSR_h
#define SimpleMSR_h

#include <IOKit/IOService.h>

/*
 * For undervolting, consult
 * https://github.com/mihic/linux-intel-undervolt
 */
static const uint32_t msr_index[] = {
    0x1FC // Disable BD PROCHOT
};
static const uint64_t msr_value[] = {
    0
};

class SimpleMSR : public IOService
{
    OSDeclareDefaultStructors(SimpleMSR)
public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
    virtual IOReturn setPowerState(unsigned long whichState, IOService * whatDevice);
private:
    void setMSR();
};


#endif /* SimpleMSR_h */
