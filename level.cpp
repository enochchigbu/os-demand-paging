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

#include "level.h"
#include "map.h"
#include "pagetable.h"

#include <iostream>

void Level::insertVpn2PfnMapping(Level *levelPtr, unsigned int vpn, int frame)
{

    // entryCount for creating new level or map
    int entryCount = levelPtr->pageTable->getEntryCount(levelPtr->depth);

    // get index for corresponding level
    int level = levelPtr->getVPNfromVirtualAddress(vpn, levelPtr->pageTable->getBitmask(levelPtr->depth),
                                                   levelPtr->pageTable->getBitShift(levelPtr->depth));
    // Leaf Node
    if (levelPtr->depth == levelPtr->pageTable->getLevelCount() - 1)
    {
        // Create map*, assign to current leaf node
        if (levelPtr->map == nullptr)
        {
            Map *rootMap = new Map[entryCount];
            levelPtr->pageTable->incrementBytes(sizeof(Map) * entryCount);
            levelPtr->setMap(rootMap);
        }

        // Set page to invalid because of page replacement
        if (frame == -1)
        {
            levelPtr->map[level].setInvalid();
        }
        // set valid page to corresponding address, frame
        else
        {
            levelPtr->map[level].setValid(frame);
        }
    }
    // Interior Node
    else
    {
        // Create level**, assign to current interior node if level doesnt exist
        if (levelPtr->nextLevel == nullptr)
        {
            Level **nextLevel = new Level *[entryCount]
            { nullptr };
            levelPtr->pageTable->incrementBytes(sizeof(Level *) * entryCount);
            levelPtr->setNextLevel(nextLevel);
        }

        // set next level to corresponding address if nextLevel doesnt exist
        if (levelPtr->nextLevel[level] == nullptr)
        {
            levelPtr->nextLevel[level] = new Level(levelPtr->pageTable, (levelPtr->depth + 1));
            levelPtr->pageTable->incrementBytes(sizeof(Level));
        }

        // call function recursively
        insertVpn2PfnMapping(levelPtr->nextLevel[level], vpn, frame);
    }
}

unsigned int Level::getVPNfromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift)
{
    unsigned int maskedAddress = mask & virtualAddress;
    return maskedAddress >> shift;
}

Map *Level::findVpn2PfnMapping(Level *levelPtr, unsigned int vpn)
{
    int level = getVPNfromVirtualAddress(vpn, levelPtr->pageTable->getBitmask(levelPtr->depth),
                                         levelPtr->pageTable->getBitShift(levelPtr->depth));

    // Leaf Node, extract map information
    if (levelPtr->nextLevel == nullptr)
    {
        if (levelPtr->map[level].isValid())
        {
            return &levelPtr->map[level];
        }
        else
        {
            return nullptr;
        }
    }

    // Interior Node
    else if (levelPtr->nextLevel[level] != nullptr)
    {
        return findVpn2PfnMapping(levelPtr->nextLevel[level], vpn);
    }
    else
    {
        return nullptr;
    }
}