//    Single Programmer Affidavit
//    I the undersigned promise that the submitted assignment is my own work. While I was
//    free to discuss ideas with others, the work contained is my own. I recognize that
//    should this not be the case; I will be subject to penalties as outlined in the course
//    syllabus.
//    Name: Enoch Chigbu
//    Red ID: 827078077
//
//    Name: Farhan Talukder
//    Red ID: 827061320

#include "wsclock.h"
#include "pagetable.h"
#include "log_helpers.h"
#include <vector>
#include <iostream>

void WSClock::insertPage(PageTable *pageTable, unsigned int vpn, int bit, LogOptionsType log)
{
    unsigned int vpnShift = 32 - pageTable->getTotalBits();
    // Check if Mapping information already exists for vpn, page table hit
    if (pageTable->findVpn2PfnMapping(vpn) != nullptr)
    {
        pageTable->incrementHits();
        if (bit == 1)
        {
            clockList.at(pageTable->findVpn2PfnMapping(vpn)->getFrame()).setDirty(true);
        }
        if (log.vpn2pfn_with_pagereplace)
        {
            log_mapping(vpn >> vpnShift, pageTable->findVpn2PfnMapping(vpn)->getFrame(), -1, true);
        }
        clockList.at(pageTable->findVpn2PfnMapping(vpn)->getFrame()).setAccessTime(virtualTime);
    }
    else // Page Table Miss
    {
        // If entries still open, increment by one
        if (rotation == false)
        {
            pageTable->incrementMisses();
            pageTable->incrementFramesAllocated();
            if (log.vpn2pfn_with_pagereplace)
            {
                log_mapping(vpn >> vpnShift, clockhand, -1, false);
            }
            pageTable->insertVpn2PfnMapping(vpn, clockhand);
            clockList.at(clockhand).setAccessTime(virtualTime);
            clockList.at(clockhand).setVpn(vpn);
            if (bit == 1)
            {
                clockList.at(clockhand).setDirty(true);
            }
            incrementVirtualTime();
        }
        // all entries taken, time for page replacement, look for victim frame
        else if (((virtualTime - clockList.at(clockhand).getLastAccessTime()) > ageOfLastAccessConsideredRecent))
        {
            if (clockList.at(clockhand).getDirty())
            {
                // Current frame has dirty bit, cant replace yet
                clockList.at(clockhand).setAccessTime(virtualTime);
                clockList.at(clockhand).setDirty(false);
                incrementVirtualTime();
                insertPage(pageTable, vpn, bit, log);
            }
            else
            {
                // time to replace, victim found

                // Invalidate current vpn
                pageTable->insertVpn2PfnMapping(clockList.at(clockhand).getVpn(), -1);
                pageTable->insertVpn2PfnMapping(vpn, clockhand);

                pageTable->incrementMisses();
                pageTable->incrementReplaces();

                if (log.vpn2pfn_with_pagereplace)
                {
                    log_mapping(vpn >> vpnShift, pageTable->findVpn2PfnMapping(vpn)->getFrame(), clockList.at(clockhand).getVpn() >> vpnShift, false);
                }
                clockList.at(clockhand).setAccessTime(virtualTime);
                clockList.at(clockhand).setVpn(vpn);
                if (bit == 1)
                {
                    clockList.at(clockhand).setDirty(bit);
                }
                incrementVirtualTime();
            }
        }
        // Not a victim frame, go to next frame
        else
        {
            clockList.at(clockhand).setAccessTime(virtualTime);
            incrementVirtualTime();
            insertPage(pageTable, vpn, bit, log);
        }
    }
}