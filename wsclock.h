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
#include "log_helpers.h"

#include <vector>
#include <iostream>

#ifndef WS_CLOCK_ENTRY
#define WS_CLOCK_ENTRY

// WSClockEntry class
class WSClockEntry
{
public:
    WSClockEntry()
    {
        this->vpn = 0;
        this->lastAccessTime = 0;
        this->dirty = false;
    }

    void setAccessTime(int accessTime)
    {
        this->lastAccessTime = accessTime;
    }

    int getLastAccessTime()
    {
        return this->lastAccessTime;
    }

    void setVpn(unsigned int vpn)
    {
        this->vpn = vpn;
    }

    unsigned int getVpn()
    {
        return this->vpn;
    }

    void setDirty(bool dirty)
    {
        this->dirty = dirty;
    }

    bool getDirty()
    {
        return this->dirty;
    }

private:
    unsigned int vpn;
    int lastAccessTime;
    bool dirty; // 0 (false) = read, 1 (true) = write
};
#endif

#ifndef WS_CLOCK
#define WS_CLOCK

class WSClock
{
public:
    WSClock(int frames, int ageOfLastAccessConsideredRecent)
    {
        this->frames = frames;
        this->clockhand = 0;
        this->virtualTime = 0;
        this->rotation = false;
        this->ageOfLastAccessConsideredRecent = ageOfLastAccessConsideredRecent;
        this->clockList = std::vector<WSClockEntry>(frames);
    }

    void incrementVirtualTime()
    {
        this->virtualTime++;
        this->clockhand++;

        if (this->clockhand == frames)
        {
            this->clockhand = 0;
            this->rotation = true;
        }
    }

    int getClockhand()
    {
        return this->clockhand;
    }

    WSClockEntry getClocklistEntry(int frame)
    {
        return this->clockList.at(frame);
    }

    void insertPage(PageTable *pageTable, unsigned int vpn, int bit, LogOptionsType log);

private:
    int frames;
    int clockhand;
    bool rotation;
    int virtualTime;
    int ageOfLastAccessConsideredRecent;
    std::vector<WSClockEntry> clockList;
};
#endif