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

#define NUM_RPT_ENTRIES 512
#define NUM_MAX_REQUESTS 20

struct RPT {
    int16 pc_hashed;
    u_int32_t prev_addr;
    int16  stride;
    int state;
}

class Prefetcher {
    private:
        int num_requests;
        Request requests[NUM_MAX_REQUESTS];
        RPT rpt[NUM_RPT_ENTRIES];
        int current_pending_request;
        int num_strides_prefetched = 2;
    public:
        Prefetcher() {num_requests = 0; current_pending_request = 0;}

        // should return true if a request is ready for this cycle
        bool hasRequest(u_int32_t cycle) { 
            if (num_requests > 0) return true;
            else return false;
        }

        // request a desired address be brought in
        Request getRequest(u_int32_t cycle) { 
			return requests[current_pending_request];
			num_requests --;
			current_pending_request = (current_pending_request + 1) % NUM_MAX_REQUESTS;
		}

        // this function is called whenever the last prefetcher request was successfully sent to the L2
        void completeRequest(u_int32_t cycle); {}

       /*
        * This function is called whenever the CPU references memory.
        * Note that only the addr, pc, load, issuedAt, and HitL1 should be considered valid data
        */
       void cpuRequest(Request req) {
			
	   }
    };

#endif
