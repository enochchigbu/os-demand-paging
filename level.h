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

class PageTable; // class dependency injection

#ifndef LEVEL_H
#define LEVEL_H

class Level
{
public:
    // Interior Node Constructor
    Level(PageTable *pageTable, int depth, Level **nextLevel)
    {
        this->pageTable = pageTable;
        this->depth = depth;
        this->nextLevel = nextLevel;
        this->map = nullptr;
    }

    // Leaf Node Constructor
    Level(PageTable *pageTable, int depth, Map *map)
    {
        this->pageTable = pageTable;
        this->depth = depth;
        this->nextLevel = nullptr;
        this->map = map;
    }

    // Constructor without map or nextLevel, for inserting nodes
    Level(PageTable *pageTable, int depth)
    {
        this->pageTable = pageTable;
        this->depth = depth;
        this->nextLevel = nullptr;
        this->map = nullptr;
    }

    // Functions to set nextLevel and map, for inserting nodes
    void setNextLevel(Level **nextLevel)
    {
        this->nextLevel = nextLevel;
    }

    void setMap(Map *map)
    {
        this->map = map;
    }

    void insertVpn2PfnMapping(Level *levelPtr, unsigned int vpn, int frame);
    unsigned int getVPNfromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
    Map *findVpn2PfnMapping(Level *levelPtr, unsigned int vpn);

private:
    PageTable *pageTable;
    int depth;
    Level **nextLevel; // for interior nodes
    Map *map;          // for leaf nodes
};

#endif