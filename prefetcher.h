/*
 *
 * File: prefetcher.h
 * Author: Sat Garcia (sat@cs)
 * Description: Header file for prefetcher implementation
 *
 */

#ifndef PREFETCHER_H
#define PREFETCHER_H

#include <sys/types.h>
#include "mem-sim.h"

#define NUM_RPT_ENTRIES 128 
#define NUM_MAX_REQUESTS 40 
#define REQUEST_CUTOFF 20
#define DEFAULT_STREAMSIZE 2

struct RPT {
    u_int32_t pc;
    u_int32_t prev_addr;
    u_int32_t stride;
    int32_t state;
} ;// reference prediction table.


class Prefetcher {
    private:
        int num_requests;
        int num_rpt;
        Request requests[NUM_MAX_REQUESTS];
        RPT rpt[NUM_RPT_ENTRIES];
        int current_pending_request;
        int oldest_rpt;
    public:
        Prefetcher();
        // should return true if a request is ready for this cycle
        bool hasRequest(u_int32_t cycle);

        // request a desired address be brought in
        Request getRequest(u_int32_t cycle);

        // this function is called whenever the last prefetcher request was successfully sent to the L2
        void completeRequest(u_int32_t cycle);

        /*
         * This function is called whenever the CPU references memory.
         * Note that only the addr, pc, load, issuedAt, and HitL1 should be considered valid data
         */
        void cpuRequest(Request req);
        void addRequest(u_int32_t addr);
        bool ifAlreadyInQueue(u_int32_t addr);
};

#endif
