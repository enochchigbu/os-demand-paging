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

#include "pagetable.h"
#include "level.h"
#include "map.h"

#include <vector>
#include <iostream>

unsigned int PageTable::getVPNfromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift)
{
    unsigned int maskedAddress = mask & virtualAddress;
    return maskedAddress >> shift;
}

void PageTable::insertVpn2PfnMapping(unsigned int vpn, int frame)
{
    rootNodePtr->insertVpn2PfnMapping(this->rootNodePtr, vpn, frame);
}

Map *PageTable::findVpn2PfnMapping(unsigned int vpn)
{
    return rootNodePtr->findVpn2PfnMapping(this->rootNodePtr, vpn);
}