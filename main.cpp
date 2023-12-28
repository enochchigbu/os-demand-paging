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
#include "wsclock.h"
#include "log_helpers.h"
#include "vaddr_tracereader.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#define NORMAL_EXIT 0

int main(int argc, char **argv)
{
    using namespace std;

    FILE *traceFile; // trace and memory access file ptrs
    FILE *memAccessFile;
    p2AddrTr trace; /* traced address */

    int option; // Option for command line arguments

    // Default values
    int addressesToRead = -1; // Negative 1 symbolizes process all arguments
    int framesAvaliable = 999999;
    int ageOfLastAccess = 10;
    LogOptionsType logType;

    logType.addressTranslation = false;
    logType.offset = false;
    logType.pagetable_bitmasks = false;
    logType.vpn2pfn_with_pagereplace = false;
    logType.vpns_pfn = false;
    logType.summary = true;

    // number of total bits, to dynamically create pageTable arguments
    int totalBits = 0;

    // Init PageTable arguments
    int level = 0;
    std::vector<unsigned int> bitMaskAry;
    std::vector<int> shiftAry;
    std::vector<int> entryCountAry;

    // Process optional arguments
    while ((option = getopt(argc, argv, "n:f:a:l:")) != -1)
    {
        switch (option)
        {
        // First optional argument
        case 'n':
            if (atoi(optarg) < 1)
            {
                std::cout << "Number of memory accesses must be a number, greater than 0" << std::endl;
                exit(NORMAL_EXIT);
            }
            addressesToRead = atoi(optarg);
            break;
        // Second optional argument
        case 'f':
            if (atoi(optarg) < 1)
            {
                std::cout << "Number of available frames must be a number, greater than 0" << std::endl;
                exit(NORMAL_EXIT);
            }
            framesAvaliable = atoi(optarg);
            break;
        // Third optional argument
        case 'a':
            if (atoi(optarg) < 1)
            {
                std::cout << "Age of last access considered recent must be a number, greater than 0 " << std::endl;
                exit(NORMAL_EXIT);
            }
            ageOfLastAccess = atoi(optarg);
            break;
        // Fourth Optional Argument
        case 'l':
            if (strcmp(optarg, "bitmasks") == 0)
            {
                logType.pagetable_bitmasks = true;
                logType.summary = false;
            }
            else if (strcmp(optarg, "va2pa") == 0)
            {
                logType.addressTranslation = true;
                logType.summary = false;
            }
            else if (strcmp(optarg, "vpns_pfn") == 0)
            {
                logType.vpns_pfn = true;
                logType.summary = false;
            }
            else if (strcmp(optarg, "vpn2pfn_pr") == 0)
            {
                logType.vpn2pfn_with_pagereplace = true;
                logType.summary = false;
            }
            else if (strcmp(optarg, "offset") == 0)
            {
                logType.offset = true;
                logType.summary = false;
            }
            else // default case
            {
                logType.summary = true;
            }
            break;
        // Invalid argument
        default:
            std::cout << "Invalid Argument, ending program" << std::endl;
            exit(NORMAL_EXIT);
        }
    }

    int idx = optind; // general purpose index variable

    // Process 2 Mandatory File Arguments
    // Trace file

    traceFile = fopen(argv[idx], "rb");
    if (traceFile == nullptr)
    {
        std::cout << "Unable to open << trace.tr >>" << std::endl;
        exit(NORMAL_EXIT);
    }

    idx++;

    // Address file
    memAccessFile = fopen(argv[idx], "r");
    if (memAccessFile == nullptr)
    {
        std::cout << "No memory access file" << std::endl;
        exit(NORMAL_EXIT);
    }

    idx++;

    // Process arguments for PageTable Levels, build bitMask, shift, entry arrays
    while (argv[idx] != nullptr)
    {
        if (atoi(argv[idx]) < 1)
        {
            std::cout << "Level " << level << " page table must be at least 1 bit" << std::endl;
            exit(NORMAL_EXIT);
        }

        totalBits += atoi(argv[idx]);

        if (totalBits > 28)
        {
            std::cout << "Too many bits used in page tables" << std::endl;
            exit(NORMAL_EXIT);
        }

        // Create bitMask, shiftAry, entryCount based on level and bitsPerLevel

        // EntryCount for corresponding bit
        entryCountAry.push_back(static_cast<int>(std::pow(2, atoi(argv[idx]))));

        // shift for corresponding bit
        shiftAry.push_back(32 - totalBits);

        // Mask for corresponding bit
        unsigned int bitmask = static_cast<unsigned int>(std::pow(2, atoi(argv[idx])) - 1);
        bitmask <<= (32 - (totalBits));
        bitMaskAry.push_back(bitmask);

        // Increment level & argument index
        level++;
        idx++;
    }

    // Variables for summary log
    int numOfAddresses = 0;
    int totalBytesUsed = 0;

    // Create Page Table
    PageTable *table = new PageTable(level, bitMaskAry, shiftAry, entryCountAry, totalBits);

    // Create WSClock
    WSClock *wsClock = new WSClock(framesAvaliable, ageOfLastAccess);

    // Take one address from trace file, see if mapping info exists
    int indexForAddressFile = 0; // index for addresses to read

    // If addressesToRead is default value, then read all addresses
    if (addressesToRead == -1)
    {
        indexForAddressFile = -2;
    }

    int memAccessBit = 0; // Memory access read/write bit, need to do "memAccessBit -'0'"
                          // to convert to 0 or 1

    int currentFrame = 0;
    // Read one address, one mem bit, perform operations
    while (!feof(traceFile) && (memAccessBit = fgetc(memAccessFile)) != EOF && indexForAddressFile < addressesToRead)
    {
        // Get next address from trace file
        if (NextAddress(traceFile, &trace))
        {
            numOfAddresses++;
        }

        // only increment if addressesToRead is not default
        if (addressesToRead != -1)
        {
            indexForAddressFile++;
        }

        // Logging that occurs after trace file has been read
        if (logType.pagetable_bitmasks) // bitmasks
        {
            uint32_t *logBitmasks = new uint32_t[bitMaskAry.size()];
            for (long unsigned int i = 0; i < bitMaskAry.size(); i++)
            {
                logBitmasks[i] = static_cast<uint32_t>(bitMaskAry.at(i));
            }

            log_bitmasks(level, logBitmasks);
            exit(NORMAL_EXIT); // Exit Program
        }

        // Extract the virtual page number (VPN), walk the page table tree to find the
        // Virtual Page Number (VPN) → Physical Frame Number (PFN) mapping
        // information.

        unsigned int vpnMask = static_cast<unsigned int>(std::pow(2, totalBits) - 1) << (32 - totalBits);
        unsigned int vpn = table->getVPNfromVirtualAddress(trace.addr, vpnMask, 0);

        // Run page replacement
        wsClock->insertPage(table, vpn, memAccessBit - '0', logType);

        Map *pfn = table->findVpn2PfnMapping(vpn);

        if (logType.addressTranslation) // va2pa
        {
            unsigned int page = pfn->getFrame();
            unsigned int offsetMask = static_cast<unsigned int>((std::pow(2, 32 - totalBits)) - 1);
            log_va2pa(trace.addr, (page << (32 - totalBits)) | (trace.addr & offsetMask));
        }
        else if (logType.vpns_pfn) // vpns_pfn
        {
            uint32_t *logVPNs = new uint32_t[bitMaskAry.size()];
            for (long unsigned int i = 0; i < bitMaskAry.size(); i++)
            {
                logVPNs[i] = static_cast<uint32_t>(
                    table->getVPNfromVirtualAddress(
                        trace.addr,
                        bitMaskAry.at(i),
                        shiftAry.at(i)));
            }
            if (pfn != nullptr)
            {
                log_vpns_pfn(level, logVPNs, pfn->getFrame());
            }
            else
            {
                log_vpns_pfn(level, logVPNs, wsClock->getClockhand() - 1);
            }
        }
        else if (logType.offset) // offset
        {
            int offsetMask = static_cast<unsigned int>((std::pow(2, 32 - totalBits)) - 1);
            print_num_inHex(trace.addr & offsetMask);
        }
    }
    if (logType.summary)
    {
        log_summary(static_cast<int>((std::pow(2, 32 - totalBits))),
                    table->getReplaces(),
                    table->getHits(), numOfAddresses, table->getFramesAllocated(), table->getBytes() + sizeof(*table));
    }
}