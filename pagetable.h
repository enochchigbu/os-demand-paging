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

#include "map.h"
#include "level.h"
#include <vector>

#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

class Level;

class PageTable
{
public:
    PageTable(int levelCount, std::vector<unsigned int> bitmaskAry, std::vector<int> shiftAry, std::vector<int> entryCount, int totalBits)
    {
        // init all member variables
        this->levelCount = levelCount;
        this->bitmask = bitmaskAry;
        this->bitShift = shiftAry;
        this->entryCount = entryCount;
        this->totalBits = totalBits;
        this->hits = 0;
        this->replaces = 0;
        this->misses = 0;
        this->bytes = 0;
        this->framesAllocated = 0;

        // If levelCount is 1, then instanitate Level 0 (leaf node) with map structures
        if (levelCount == 1)
        {
            Map *rootMap = new Map[entryCount.at(0)];
            this->rootNodePtr = new Level(this, 0, rootMap);
            this->bytes += sizeof(Map) * entryCount.at(0);
        }

        // Init nextLevel with nextLevelPtr
        else
        {
            // create rootLevel array, set all entries to nullptr
            Level **rootLevel = new Level *[entryCount.at(0)]
            { nullptr };

            // init Level 0
            this->rootNodePtr = new Level(this, 0, rootLevel);

            this->bytes += sizeof(Level *) * entryCount.at(0);
        }
    }

    int getLevelCount()
    {
        return this->levelCount;
    }

    int getBitmask(int level)
    {
        return bitmask.at(level);
    }

    int getBitShift(int level)
    {
        return bitShift.at(level);
    }

    int getEntryCount(int level)
    {
        return entryCount.at(level);
    }

    int getTotalBits()
    {
        return totalBits;
    }

    void incrementHits()
    {
        this->hits++;
    }

    void incrementMisses()
    {
        this->misses++;
    }

    void incrementReplaces()
    {
        this->replaces++;
    }

    void incrementBytes(int bytes)
    {
        this->bytes += bytes;
    }

    void incrementFramesAllocated()
    {
        this->framesAllocated++;
    }

    int getHits()
    {
        return this->hits;
    }

    int getReplaces()
    {
        return this->replaces;
    }

    int getMisses()
    {
        return this->misses;
    }

    int getBytes()
    {
        return this->bytes;
    }

    int getFramesAllocated()
    {
        return this->framesAllocated;
    }

    unsigned int getVPNfromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
    void insertVpn2PfnMapping(unsigned int vpn, int frame);
    Map *findVpn2PfnMapping(unsigned int vpn);

private:
    Level *rootNodePtr;                // Level 0
    int levelCount;                    // Number of levels
    int totalBits;                     // Total bits
    int hits;                          // Total number of page table hits
    int replaces;                      // Total number of page table replaces
    int misses;                        // Total number of page table misses
    int framesAllocated;               // Total number of frames allocated
    int bytes;                         // Total number of bytes used for page table
    std::vector<unsigned int> bitmask; // Bit mask for level i
    std::vector<int> bitShift;         // Number of bits to shift level i page bits
    std::vector<int> entryCount;       // # of possible pages for level i
};

#endif