#include "prefetcher.h"
#include <stdio.h>

Prefetcher::Prefetcher() {
    // printf ("Prefetcher()\n");
    num_requests = 0; current_pending_request = 0; num_rpt = 0; oldest_rpt = 0;
}

bool Prefetcher::hasRequest(u_int32_t cycle) { 
    if (num_requests > 0) return true;
    else return false;
}

Request Prefetcher::getRequest(u_int32_t cycle) { 
    // printf ("In prefetcher::getRequest\n");
    num_requests --;
    int request_index = current_pending_request;
    current_pending_request = (current_pending_request + 1) % NUM_MAX_REQUESTS;
    return requests[request_index];
}


void Prefetcher::completeRequest(u_int32_t cycle) { 
    // Nothing. That is because we need to maintain the consistency of the queue. So everything in the 
    // request queue must be handled in a fashion that maintains coherency. 
}

void Prefetcher::cpuRequest(Request req) { 
    // printf ("In prefetcher::cpuRequest\n"); 
    int index = -1;
    for(int i = 0; i < NUM_RPT_ENTRIES; i++){
        if (rpt[i].pc == req.pc){
            index = i;
            break;
        }
    }
    if(index != -1){
        u_int32_t temp_stride = rpt[index].stride;
        rpt[index].stride = req.addr - rpt[index].prev_addr;
        rpt[index].prev_addr = req.addr;
        if (temp_stride == rpt[index].stride) { // stride hit
            rpt[index].state++;
            if (rpt[index].state > 0) {
                if (rpt[index].state >= REQUEST_CUTOFF && req.HitL1 == true) {
                    // do nothing, we have fetched enough
                } else {
                    for (int n = 1; n <= rpt[index].state && n <= REQUEST_CUTOFF; n++){
                        u_int32_t temp_addr = req.addr + rpt[index].stride * n;
                        addRequest(temp_addr);                  
                    }
                }
            } else {
                for (int i = 0; i < DEFAULT_STREAMSIZE; ++i) {
                    addRequest(req.addr + 32 * (i+1));
                }
            }
        } 
        // else does not fetch at all. set state to false.
        else {
            rpt[index].state = -1; // misprediction
        }
    }
    else {
        rpt[oldest_rpt].pc = req.pc;
        rpt[oldest_rpt].prev_addr = req.addr;
        rpt[oldest_rpt].state = 0;
        rpt[oldest_rpt].stride = 4; // default value, size of a byte. 
        oldest_rpt = (oldest_rpt + 1) % NUM_RPT_ENTRIES;

        for (int i = 0; i < DEFAULT_STREAMSIZE; ++i) {
            addRequest(req.addr + 32 * (i+1));
        }
    }
    // some form of stream prefetcher.
}

void Prefetcher::addRequest(u_int32_t addr){
    if(num_requests != NUM_MAX_REQUESTS){
        requests[(current_pending_request + num_requests) % NUM_MAX_REQUESTS].addr = addr; 
        num_requests ++ ;
    }
}










