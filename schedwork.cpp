#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

bool scheduled(
    Worker_T w,
    const std::vector<Worker_T>& schedRow) 
    {
        for (size_t i = 0; i < schedRow.size(); i++) {
            if (schedRow[i] == w) return true;
        }
        return false;
} 

    
bool proceed(size_t day,
            Worker_T w,
            const AvailabilityMatrix& avail,
            const std::vector<size_t>& shiftsWorked,
            size_t maxShifts,
            const DailySchedule& schedRow)   // workers already chosen for this day
{
    //if available, Not over-worked, and not on the list already
    return avail[day][w] && shiftsWorked[w] < maxShifts && !scheduled(w, schedRow[day]);
}



// Add your implementation of schedule() and other helper functions here


bool recurse(
    size_t day,
    size_t slot,                      
    DailySchedule& sched,             
    const AvailabilityMatrix& avail,
    std::vector<size_t>& shiftsWorked,
    size_t dailyNeed,
    size_t maxShifts)
    {
        //base case, all days done
        if (day == avail.size())
            return true;

        for (Worker_T w = 0 ; w < avail[0].size() ; w++){ //iterate each worker

            if (proceed(day,w,avail, shiftsWorked, maxShifts, sched) ){
                // place
                sched[day][slot] = w;
                shiftsWorked[w]++;
                //next day/slot
                size_t nextDay  = (slot + 1 == dailyNeed) ? day + 1 : day;
                size_t nextSlot = (slot + 1) % dailyNeed;
                if(recurse(nextDay, nextSlot, sched,
                        avail, shiftsWorked,
                        dailyNeed, maxShifts))
                    return true;

                //backtrack
                sched[day][slot] = INVALID_ID;
                shiftsWorked[w]--;
            }
        }

        return false;
}




bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U || dailyNeed == 0){
        return false;
    }
    sched.clear();
    // Add your code belo

    size_t n = avail.size();
    size_t k = avail[0].size();

    sched.resize(n);
    for (size_t i = 0; i < n; ++i) {
        sched[i].resize(dailyNeed, INVALID_ID);
    }
    std::vector<size_t> shiftsWorked(k, 0);

    return recurse(0, 0, sched, avail, shiftsWorked, dailyNeed, maxShifts);


}

