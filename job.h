#ifndef JOB_H
#define JOB_H

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>  
#include <stdlib.h>     
#include <time.h>  
     
using namespace std;

const bool PERIODIC = 1;
const bool APERIODIC = 0;

class Job
{ 
  public:
    int jobNum;
    bool periodic; //true = periodic, false = aperiodic
    bool done = false;
    bool started = false;
    bool executing = false;
    int priority = 0;
    
    int execTime = 0;
    int deadline = 0;
    int releaseTime = 0;
  
    float respTime = 0;
    float slackTime = 0;
    
    int timeRemaining = 0;
    int StartTime = 0;
    int FinishTime = 0;
    
    int period = 0;
    int iteration = 0; 
    int numDeadlinesMissed = 0;
    int avgRespTime = 0;
    
    int relDeadline = 0;
  
    string output = "";
    
    virtual bool Execute(int CurrTime)
    {
      cout << "calling virtual" << endl;
      return done;
    }
    
    virtual void resetJob()
    {
      cout << "calling virtual" << endl;
    }
    
    virtual void newIteration()
    {
      cout << "calling virtual" << endl;
    }
    
    void CalcRespTime()
    {
      respTime = FinishTime - releaseTime;
      return;
    }
    void CalcSlackTime(int CurrTime)
    {
      slackTime = deadline - CurrTime - timeRemaining;
      return;
    }   
    string metDeadline()
    {
      if (deadline >= FinishTime)
      {
        return "yes";
      }
      else
      {
        numDeadlinesMissed++;
        return "no";
      }
    }
};

class AperiodicJob: public Job
{
  public:   
    AperiodicJob(int n, int r, int e, int d)
    {
      jobNum = n;
      periodic = 0;
      releaseTime = r;
      execTime = e;
      deadline = d;
      timeRemaining = e;
    }
    
    bool Execute(int CurrTime)
    {
      //cout << "Executing job " << jobNum << " with time remaining " << timeRemaining << endl;
      
      if (!started)
      {
        started = true;
        StartTime = CurrTime;
      }
      timeRemaining--;    
      CalcSlackTime(CurrTime);

      if (timeRemaining <= 0)
      {
        done = true;        
        FinishTime = CurrTime;
        CalcRespTime();
      }  
      
      if (started && !done)
      {
        executing = true;
        if (CurrTime >= deadline)
        {
          numDeadlinesMissed++;
        }
      }
      else
      {
        executing = false;
      }
      return done;
    }
    
    void resetJob()
    {
      done = false;
      started = false;
  
      respTime = 0;
      slackTime = 0;
    
      timeRemaining = execTime;
      StartTime = 0;
      FinishTime = 0;
  
      output = "";
    }
};

class PeriodicJob: public Job
{
  public:
    
    PeriodicJob(int n, int p, int e, int d)
    {
      jobNum = n;
      periodic = 1;
      period = p;
      execTime = e;
      deadline = relDeadline = d;
      timeRemaining = e;
      
      CalcSlackTime(0);
    }
    
    bool Execute(int CurrTime)
    {
      if (!started)
      {
        started = true;
        StartTime = CurrTime;
      }
      timeRemaining--;    
      CalcSlackTime(CurrTime);
      
      if (timeRemaining <= 0)
      {     
        FinishTime = CurrTime;
        CalcRespTime();
        done = true;
        newIteration();
      }  
      
      if (started && !done)
      {
        executing = true;
      }
      else
      {
        executing = false;
      }
      
      return done;
    }
    
    void CalcRespTime()
    {
      respTime = FinishTime - releaseTime;
      avgRespTime += respTime;
      
      return;
    }
    void CalcSlackTime(int CurrTime)
    {
      slackTime = deadline - CurrTime - timeRemaining;
      return;
    }  
    
    string metDeadline()
    {
      if (numDeadlinesMissed <= 0)
      {
        return "yes";
      }
      else
      {
        return "no";
      }
    } 
    
    void resetJob()
    {
      done = false;
      started = false;
  
      respTime = 0;
      slackTime = 0;
    
      timeRemaining = execTime;
      StartTime = 0;
      FinishTime = 0;
      
      deadline = relDeadline;
      
      iteration = 0; 
      releaseTime = 0;
      numDeadlinesMissed = 0;
      avgRespTime = 0;
  
      output = "";
      
      return;
    }
    
    void newIteration()
    {
      done = false;
      started = false;
      
      iteration++;
  
      releaseTime = period * iteration;
      deadline = relDeadline + releaseTime;
      slackTime = 0;
    
      timeRemaining = execTime;
    }
};


#endif