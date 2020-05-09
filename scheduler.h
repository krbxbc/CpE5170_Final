#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"
#include <fstream>

const int SHORT = 0;
const int MED = 1;
const int LONG = 2;

const int MAX_NUM_TEST = 243;
const int MAX_REPEAT = 5;
const int NUM_TO_REDUCE_SET = 100;
const long MAX_ATTEMPTS = 100000;

class Scheduler
{
  private: 
    vector<Job*> listOfJobs;
    //parameters for generation
    int percentPeriodic = 0;
    int minPeriod = 1;
    int maxPeriod = 50;
    int minExecTime = 1;
    int maxExecTime = 10;
    int minDeadline = 1;
    int maxDeadline = 100;
    int minReleaseTime = 0;
    int maxReleaseTime = 10;
    
    int perPeriodic[3] = {0,50,100}; //vary every 1
    int perPeriodicLength = SHORT;
    
    int minPeriods[3] = {1,5,10};  //vary every 3
    int maxPeriods[3] = {5,10,20};
    int PeriodLength = SHORT;
    
    int minExecTimes[3] = {1,5,10};  //vary every 9
    int maxExecTimes[3] = {5,10,20};
    int ExecTimeLength = SHORT;
    
    int minDeadlines[3] = {1, 10, 20};  //vary every 27
    int maxDeadlines[3] = {20, 50, 100};
    int DeadlineLength = SHORT;
    
    int maxReleaseTimes[3] = {5,20,50}; //vary every 81
    int ReleaseTimeLength = SHORT;
    
    int numJobs = 10;
    int OriginalNumJobs = 10;
    int CurTime = 0;
    int FinishTime = 0;
    float Utilization = 0;
    
    bool finished = false;
    
    int numPeriodic = 0;
    
  public:
    ofstream results;
    
    void GetInput();
    void getTestInput(int numTest);
    
    void generateJobs();
    bool getSchedulableJobSet();
    bool EDF_test();
    void ListJobs();
    
    void printScheduleHoriz();
    void printScheduleVert();
    
    void NoPreemptEDF();
    void PreemptEDF();
    void LST_EDF();
    void CriticalWindowEDF();
    
    void resetAllJobs();
    
    void getResults();
    float avgNormResponseTime();
    float percentMissedDeadlines();
    float avgExecTime();
    int execRange();
    float avgPeriod();
    int PeriodRange();
    float avgDeadline();
    int deadlineRange();
    void initResultsCSV();
};

#endif