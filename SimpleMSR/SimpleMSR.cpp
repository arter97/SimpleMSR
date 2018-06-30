//
//  SimpleMSR.c
//  SimpleMSR
//
//  Created by Park Ju Hyung on 30/06/2018.
//  Copyright © 2018 Park Ju Hyung. All rights reserved.
//

#include <IOKit/IOLib.h>
#include <i386/proc_reg.h> // For msr operators
#include "SimpleMSR.hpp"

#define kPowerStates 2
#define kIOPMPowerOff 0

// Define the driver's superclass.
#define super IOService

OSDefineMetaClassAndStructors(SimpleMSR, IOService)

#define TAG "SimpleMSR: "

bool SimpleMSR::init(OSDictionary *dict)
{
    bool result = super::init(dict);
    IOLog(TAG "Initializing\n");
    return result;
}

void SimpleMSR::free(void)
{
    IOLog(TAG "Freeing\n");
    super::free();
}

IOService *SimpleMSR::probe(IOService *provider,
                                    SInt32 *score)
{
    IOService *result = super::probe(provider, score);
    IOLog(TAG "Probing\n");
    
    return result;
}

bool SimpleMSR::start(IOService *provider)
{
    bool result = super::start(provider);
    IOLog(TAG "Starting\n");
    PMinit();
    provider->joinPMtree(this);
    static IOPMPowerState myPowerStates[kPowerStates] = {
        {1, kIOPMPowerOff, kIOPMPowerOff, kIOPMPowerOff, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, kIOPMPowerOn, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    
    registerPowerDriver (this, myPowerStates, kPowerStates);
    
    return result;
}

void SimpleMSR::stop(IOService *provider)
{
    IOLog(TAG "Stopping\n");
    PMstop();
    super::stop(provider);
}

void SimpleMSR::setMSR()
{
    // Hardcoded atm
    int i;
    char addr[8];
    for (i = 0; i < sizeof(msr_index) / sizeof(uint32_t); i++) {
        snprintf(addr, 8, "%X", msr_index[i]);
        IOLog(TAG "Read MSR 0x%s = %llu\n", addr, rdmsr64(msr_index[i]));
        wrmsr64(msr_index[i], msr_value[i]);
        IOLog(TAG "Changed MSR 0x%s to %llu\n", addr, rdmsr64(msr_index[i]));
    }
    
    IOLog(TAG "MSR all set\n");
}

IOReturn SimpleMSR::setPowerState ( unsigned long whichState, IOService * whatDevice )
{
    if (kIOPMPowerOff != whichState) {
        IOLog(TAG "Waking up\n");
        this->setMSR();
    }
    return kIOPMAckImplied;
}
