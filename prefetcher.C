/*
 *
 * File: prefetcher.C
 * Author: Sat Garcia (sat@cs)
 * Description: This simple prefetcher waits until there is a D-cache miss then 
 * requests location (addr + 16), where addr is the address that just missed 
 * in the cache.
 *
 */

#include "prefetcher.h"
#include <stdio.h>

Prefetcher::Prefetcher() {num_requests = 0; current_pending_request = 0; num_rpt = 0;}

bool Prefetcher::hasRequest(u_int32_t cycle) { 
	if (num_requests > 0) return true;
    else return false;
 }

Request Prefetcher::getRequest(u_int32_t cycle) { 

	return requests[current_pending_request];
	
}


void Prefetcher::completeRequest(u_int32_t cycle) { 

	num_requests --;
	current_pending_request = (current_pending_request + 1) % NUM_MAX_REQUESTS;
}

void Prefetcher::cpuRequest(Request req) { 
	int index = -1;
	for(int i = 0; i < NUM_RPT_ENTRIES; i++){
		if (rpt[i].pc == req.pc){
			index = i;
		}
	}

	if(index != -1){
		//u_int32_t temp_stride = rpt[index].stride;
		rpt[index].stride = req.addr - rpt[index].prev_addr;
		rpt[index].prev_addr = req.addr;
		for (int n = 1; n < num_strides_prefetched + 1; n++){
			u_int32_t temp_addr = req.addr + rpt[index].stride *n;
			if( !isFull() ){
				requests[rear_request].addr = temp_addr;
				rear_request = (rear_request+1) % NUM_MAX_REQUESTS;
				num_requests++;
				//printf("%d\n", num_requests );
			}
		}
		
		
	}

	else{
		if(num_rpt < NUM_RPT_ENTRIES){
			rpt[num_rpt].pc = req.pc;
			rpt[num_rpt].prev_addr = req.addr;
			rpt[num_rpt].stride = 0;
			num_rpt++;
		}
		u_int32_t temp_addr = req.addr + 16;
		if( !isFull() ){
			requests[rear_request].addr = temp_addr;
			rear_request = (rear_request+1) % NUM_MAX_REQUESTS;
			num_requests++;
			//printf("%d\n", num_requests );
		}
		
	}
	





	// if(!_ready && !req.HitL1) {
	// 	_nextReq.addr = req.addr + 16;
	// 	_ready = true;
	
}
 
bool Prefetcher::isFull(){
	if( current_pending_request == (rear_request+1) % NUM_MAX_REQUESTS)
		return true;

}


