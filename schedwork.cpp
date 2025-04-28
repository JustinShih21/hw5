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

// Helper for the schedule function 
bool scheduleHelper(const AvailabilityMatrix& avail, 
const size_t dailyNeed, 
const size_t maxShifts, 
DailySchedule& sched, 
vector<size_t>& workerShifts, 
size_t day, 
size_t worsker_slot);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    size_t n = avail.size();
    size_t k = avail[0].size();

    // Allocate memory for the schedule, filling it with empty slots
    sched.resize(n, vector<Worker_T>(dailyNeed, INVALID_ID));

    // Keep track of the amount of shifts the workers have done
    // Make sure no one has to many
    vector<size_t> workerShifts(k, 0);

    // Start the recursion
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, 0, 0);


}

bool scheduleHelper(const AvailabilityMatrix& avail, 
const size_t dailyNeed, 
const size_t maxShifts, 
DailySchedule& sched, 
vector<size_t>& workerShifts, 
size_t day, 
size_t worker_slot) {

    // Set variables for the xy grid on the availability matrix
    size_t n = avail.size();
    size_t k = avail[0].size();

    // If the entire schedule is full :: Base Case
    if (day==n) {
        return true;
    }

    // If the worker slots are filled, then go to the next day 
    if (worker_slot == dailyNeed) {

        // Call the next day
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, day + 1, 0);
    }

    // See if can assign the workers to the slot that its in 
    for (Worker_T worker = 0; worker < k; worker ++) {

        // If the worker has not worked their max shifts and they are available
        if (avail[day][worker] && workerShifts[worker] < maxShifts) {
            bool working = false;

            // Loops through all the worker slots that need to be filled 
            for (size_t i = 0; i < worker_slot; i++) {
                
                // If the scheduled block is already taken by a worker set working to true
                if (sched[day][i] == worker) {
                    working = true;
                    break;
                }
            }


            // If there needs to be more workers on that day
            if (!working) {

                sched[day][worker_slot] = worker;
                workerShifts[worker]++;

                // Use recursion to fill up the rest of the schedule
                if (scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, day, worker_slot + 1)) {
                    return true;
                }

                // If the assignment returns false, then reset the worker slot to empty
                // and de-increment the amount of shiffts the worker has 
                sched[day][worker_slot] = INVALID_ID;
                workerShifts[worker]--;

            }
        }

    }

    // If all the workers have been tried;

    return false;

} 

