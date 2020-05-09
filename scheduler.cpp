#include "scheduler.h"

void Scheduler::GetInput()
{
  char selection;
  
  srand(time(NULL));
  
  cout << "To randomly generate a set of jobs, please enter the following data\n"
       <<"\nNumber of Jobs: ";
  cin  >> numJobs;
  cout <<"\nPercentage of Periodic Jobs (0-100): ";
  cin  >> percentPeriodic;  
  
  cout << "To use custom values, enter 'y'. To use default values, press any other key." << endl;
  cin >> selection;
  
  if (selection == 'y')
  {
    if (percentPeriodic > 0)
    {
      cout <<"\nMinimum length of Period: ";
      cin  >> minPeriod;
      cout <<"\nMaximum length of Period: ";
      cin  >> maxPeriod;
    }
    cout <<"\nMinimum Execution time: ";
    cin  >> minExecTime;
    cout <<"\nMaximum Execution time: ";
    cin  >> maxExecTime;
    cout <<"\nMinimum Deadline: ";
    cin  >> minDeadline;
    cout <<"\nMaximum Deadline: ";
    cin  >> maxDeadline;
    cout <<"\nMinimum ReleaseTime: ";
    cin  >> minReleaseTime;
    cout <<"\nMaximum ReleaseTime: ";
    cin  >> maxReleaseTime;
  }
  
  
  return;
}

void Scheduler::getTestInput(int numTest)
{
  percentPeriodic = perPeriodic[perPeriodicLength];
  
  minPeriod = minPeriods[PeriodLength];
  maxPeriod = maxPeriods[PeriodLength];
  
  minExecTime = minExecTimes[ExecTimeLength];
  maxExecTime = maxExecTimes[ExecTimeLength];
  
  minDeadline = minDeadlines[DeadlineLength];
  maxDeadline = maxDeadlines[DeadlineLength];
  
  maxReleaseTime = maxReleaseTimes[ReleaseTimeLength];
  
  
  if (numTest%81 == 0) //vary length of release time
  {
    if (ReleaseTimeLength == LONG)
    {
      ReleaseTimeLength = SHORT;
    }
    else
    {
      ReleaseTimeLength++;
    }
  }
  
  if (numTest%27 == 0) //vary length of deadline
  {
    if (DeadlineLength == LONG)
    {
      DeadlineLength = SHORT;
    }
    else
    {
      DeadlineLength++;
    }
  }
  
  if (numTest%9 == 0) //vary length of exec time
  {
    if (ExecTimeLength == LONG)
    {
      ExecTimeLength = SHORT;
    }
    else
    {
      ExecTimeLength++;
    }
  }
  
  if (numTest%3 == 0) //vary length of period
  {
    if (PeriodLength == LONG)
    {
      PeriodLength = SHORT;
    }
    else
    {
      PeriodLength++;
    }
  }
  
  //vary percent periodic
  if (perPeriodicLength == LONG)
  {
    perPeriodicLength = SHORT;
  }
  else
  {
    perPeriodicLength++;
  }
  
  return;
}

bool Scheduler::getSchedulableJobSet()
{
  int numGen = 0;
  bool success = false;
  numJobs = OriginalNumJobs;
  listOfJobs.clear();
  
  while (numGen < MAX_ATTEMPTS && !success && numJobs > 1)
  {
    generateJobs();
    if (EDF_test())
    {
      success = true;
    }
    else
    {
      numGen++;
      listOfJobs.clear();
    }
    
    if (numGen% NUM_TO_REDUCE_SET == 0)
    {
      numJobs--;
    }
  }
  
  if (numGen == MAX_ATTEMPTS || numJobs == 1)
  {
    cout << "Could not generate set of schedulable jobs. Please adjust parameters and try again." << endl;
    return false;
  }
  else
  {
    cout << "took " << numGen << " tries to create a schedulable set of " << numJobs 
         << " jobs with utilization of " << Utilization << endl;
    return true;
  }
}

void Scheduler::generateJobs()
{
  numPeriodic = (percentPeriodic * numJobs)/(100.0);
  int tempnumPeriodic = numPeriodic;
  
  for (int i = 0; i < numJobs; i++)
  {
    int Period;
    int ExecTime;
    int Deadline;
    int releaseTime;
    
    ExecTime = rand() % maxExecTime + minExecTime;
    Deadline = rand() % maxDeadline + minDeadline;
    
    if (tempnumPeriodic > 0) //generate periodic job
    {
      Period = rand() % maxPeriod + minPeriod;    
      tempnumPeriodic--;  
      
      //cout << "generate" << (Deadline - Period) << endl;
      
      if (Deadline < ExecTime)
      {
        listOfJobs.push_back(new PeriodicJob(i, Period, ExecTime, ExecTime));
      }      
      else if (Deadline > Period)
      {
        listOfJobs.push_back(new PeriodicJob(i, Period, ExecTime, Period));
      }  
      else
      {
        listOfJobs.push_back(new PeriodicJob(i, Period, ExecTime, Deadline));
      }
    }
    else //generate aperiodic job
    {
      releaseTime = rand() % maxReleaseTime + minReleaseTime;        
      if (Deadline < (ExecTime + releaseTime))
      {
        Deadline = ExecTime + releaseTime;
      }
      listOfJobs.push_back(new AperiodicJob(i, releaseTime, ExecTime, Deadline));  
    }
  }  
  return;
}

void Scheduler::ListJobs()
{
  if (finished) //lists extra information
  {
    if (numPeriodic > 0 ) //if there are periodic jobs
    {
      cout << "Periodic Jobs:\n" <<
              "TaskNum\t\tPeriod\t\tExec Time\tDeadline\tResponseT\tSlackT\t\tStartT\t\tFinishT\t\tMet Deadline?\n";
    
      for (int i = 0; i < numPeriodic; i++)
      {
        cout << listOfJobs[i]->jobNum << "\t\t" 
             << listOfJobs[i]->period << "\t\t" 
             << listOfJobs[i]->execTime << "\t\t" 
             << listOfJobs[i]->relDeadline << "\t\t" 
             << listOfJobs[i]->respTime << "\t\t"
             << listOfJobs[i]->slackTime  << "\t\t" 
             << listOfJobs[i]->StartTime  << "\t\t" 
             <<listOfJobs[i]->FinishTime << "\t\t" 
             <<listOfJobs[i]->metDeadline() << endl;
        
      }
      cout << endl;
    }
    if (numPeriodic < numJobs) //if there are aperiodic jobs
    {
      cout << "Aperiodic Jobs:\n" <<
              "TaskNum\t\tRel Time\tExec Time\tDeadline\tResponseT\tSlackT\t\tStartT\t\tFinishT\t\tMet Deadline?\n";
      
      for (int i = numPeriodic; i < numJobs; i++)
      {
        cout << listOfJobs[i]->jobNum << "\t\t" 
             << listOfJobs[i]->releaseTime << "\t\t" 
             << listOfJobs[i]->execTime << "\t\t" 
             << listOfJobs[i]->deadline << "\t\t" 
             << listOfJobs[i]->respTime << "\t\t"
             << listOfJobs[i]->slackTime  << "\t\t" 
             << listOfJobs[i]->StartTime  << "\t\t" 
             <<listOfJobs[i]->FinishTime << "\t\t" 
             <<listOfJobs[i]->metDeadline() << endl;
      }      
    }
    
    cout << "Average Normalized Response Time: " << avgNormResponseTime() << endl;
  }
  else
  {
    if (numPeriodic > 0 ) //if there are periodic jobs
    {
      cout << "Periodic Jobs:\n"
           << "TaskNum\t\tPeriod\t\tExec Time\tDeadline\n";
    
      for (int i = 0; i < numPeriodic; i++)
      {
        cout << listOfJobs[i]->jobNum << "\t\t" << listOfJobs[i]->period << "\t\t" << listOfJobs[i]->execTime 
        << "\t\t" << listOfJobs[i]->relDeadline << endl;
      }      
      cout << endl;
    }
    if (numPeriodic < numJobs) //if there are aperiodic jobs
    {
      cout << "Aperiodic Jobs:\n"
           << "TaskNum\t\tRel Time\tExec Time\tDeadline\n";
      
      for (int i = numPeriodic; i < numJobs; i++)
      {
        cout << listOfJobs[i]->jobNum << "\t\t" << listOfJobs[i]->releaseTime << "\t\t" << listOfJobs[i]->execTime 
        << "\t\t" << listOfJobs[i]->deadline << endl;
      }      
    }
  }
  return;
}

bool Scheduler::EDF_test()
{
  Utilization = 0;
  for (int i = 0; i < numJobs; i++)
  {
    Utilization += ((float)(listOfJobs[i]->execTime) / (float)(listOfJobs[i]->deadline));
    /*cout << ((float)(listOfJobs[i].execTime) / (float)(listOfJobs[i].deadline));
    if (i != (numJobs-1))
      {
        cout << " + ";
      } 
    cout << i << ": " << Utilization << endl;*/
  }
  
  //cout << " = " << Utilization << endl;
  
  if (Utilization > 1)
  {
    return false;
  }
  else 
  {
    return true;
  }
}

void Scheduler::printScheduleHoriz()
{
  cout << " ";
  for (int i = 0; i <= FinishTime; i++)
  {
    if (i%10 == 0)
    {
       cout << "|";
    }
    else if (i%2 == 0)
    {
      cout << ".";
    }
    else
    {
      cout << " ";
    }
  }
  
  cout << endl;
  for (int i = 0; i < numJobs; i++)
  {
    cout << i << listOfJobs[i]->output << endl;
  }
  return;
}

void Scheduler::printScheduleVert()
{
  for (int i = 0; i < numJobs; i++)
  {
    cout << '\t'<< i;
  }
  cout << endl;
  for (int i = 0; i <= (FinishTime); i++)
  { 
    cout << i << '\t';
    for (int j = 0; j < numJobs; j++)
    {
      cout << listOfJobs[j]->output[i] << '\t';
    }
    cout << endl;
  }
  return;
}

//scheduling algorithms
void Scheduler::NoPreemptEDF()
{
  Job * curr_job = NULL;
  
  while (!finished)
  {
    //cout << endl << "0" << endl;
    bool jobsDone = true;
    bool JobsReady = false;
    bool findNewJob = true;
    
    Job * job_to_exec = NULL;
    
    if (curr_job != NULL)
    {
      if (curr_job->executing)
      {
        job_to_exec = curr_job;
        findNewJob = false;
        //cout << "current job" << endl;
      }
    }
    
    //cout << endl << "1" << endl;
    
    /*
    for (int i = 0; i < numJobs; i++)
    {
      if (listOfJobs[i]->periodic)
      {
        if ((CurTime % listOfJobs[i]->period) == 0)
        {
          listOfJobs[i]->newIteration();
          //cout << i << ": new it: " << listOfJobs[i]->iteration << endl;
        }
      }
    }*/
    
    //cout << endl << "2" << endl;
    if (findNewJob)
    {
      //cout << CurTime << ": \t";
      for (int i = 0; i < numJobs; i++)  //finds first job ready
      {
        //cout << listOfJobs[i]->releaseTime << '\t';
        
        if ((listOfJobs[i]->releaseTime <= CurTime) && (!listOfJobs[i]->done))
        {
          JobsReady = true;
          job_to_exec = listOfJobs[i];
          //cout << "found job ready" << '\t';
        }
        
      }
    
      if (JobsReady)
      {
        //cout << "jobs ready" << endl;
        
        for (int i = 0; i < numJobs; i++)
        {
          if ((job_to_exec->deadline > listOfJobs[i]->deadline) 
              && (!listOfJobs[i]->done) && (listOfJobs[i]->releaseTime < CurTime))
          {
            job_to_exec = listOfJobs[i];
            //cout << "new job" << endl;
          }
        }
      }
      else
      {
        //cout << "no job ready" << endl;
      }
    }
    
    //cout << "3" << endl;
    
    if (job_to_exec != NULL)
    {
      /*cout << CurTime << ": attempting to execute job " << job_to_exec->jobNum
           << " with time remaining " << job_to_exec->timeRemaining << endl;*/
      
      job_to_exec->Execute(CurTime);
      curr_job = job_to_exec;
    }
    
    CurTime++;
    
    //cout << "4" << endl;
      
    for (int i = 0; i < numJobs; i++)
    {
      if (job_to_exec != NULL)
      {
        if (listOfJobs[i]->jobNum == job_to_exec->jobNum)
        {
          if (job_to_exec->done)
          {
            listOfJobs[i]->output += "D";
          }
          else
          {
            listOfJobs[i]->output += "X";
          }
        }
        else
        {
          listOfJobs[i]->output += " ";
        }
               
      }
      else
      {
        listOfJobs[i]->output += " ";
      }
      
      if (listOfJobs[i]->periodic)
      {
        if (CurTime < listOfJobs[i]->period || listOfJobs[i]->iteration < 3)
        {
          jobsDone = false;
          //cout << "job " << i << "not done: " << CurTime << " < " << listOfJobs[i]->period << endl;
        }
      } 
      else if (listOfJobs[i]->done == false)
      {
        jobsDone = false;
        //cout << "job " << i << "not done." << CurTime << endl;
      }   
    }
    
    //cout << "5" << endl;
    
    if (jobsDone)
    {
      finished = true;
      FinishTime = CurTime;
      results << avgNormResponseTime() << ',' 
              << percentMissedDeadlines() << ',';
    }
    //cout << "6" << endl;
  }
}

void Scheduler::PreemptEDF()
{
  while (!finished)
  {
    //cout << endl << "0" << endl;
    bool jobsDone = true;
    bool JobsReady = false;
    bool findNewJob = true;
    
    Job * job_to_exec = NULL;
    
    //cout << endl << "2" << endl;
    if (findNewJob)
    {
      //cout << CurTime << ": \t";
      for (int i = 0; i < numJobs; i++)  //finds first job ready
      {
        //cout << listOfJobs[i]->releaseTime << '\t';
        
        if ((listOfJobs[i]->releaseTime <= CurTime) && (!listOfJobs[i]->done))
        {
          JobsReady = true;
          job_to_exec = listOfJobs[i];
          //cout << "found job ready" << '\t';
        }
        
      }
    
      if (JobsReady)
      {
        //cout << "jobs ready" << endl;
        
        for (int i = 0; i < numJobs; i++)
        {
          if ((job_to_exec->deadline > listOfJobs[i]->deadline) 
              && (!listOfJobs[i]->done) && (listOfJobs[i]->releaseTime < CurTime))
          {
            job_to_exec = listOfJobs[i];
            //cout << "new job" << endl;
          }
        }
      }
      else
      {
        //cout << "no job ready" << endl;
      }
    }
    
    //cout << "3" << endl;
    
    if (job_to_exec != NULL)
    {
      /*cout << CurTime << ": attempting to execute job " << job_to_exec->jobNum
           << " with time remaining " << job_to_exec->timeRemaining << endl;*/
      
      job_to_exec->Execute(CurTime);
    }
    
    CurTime++;
    
    //cout << "4" << endl;
      
    for (int i = 0; i < numJobs; i++)
    {
      if (job_to_exec != NULL)
      {
        if (listOfJobs[i]->jobNum == job_to_exec->jobNum)
        {
          if (job_to_exec->done)
          {
            listOfJobs[i]->output += "D";
          }
          else
          {
            listOfJobs[i]->output += "X";
          }
        }
        else
        {
          listOfJobs[i]->output += " ";
        }
               
      }
      else
      {
        listOfJobs[i]->output += " ";
      }
      
      if (listOfJobs[i]->periodic)
      {
        if (CurTime < listOfJobs[i]->period || listOfJobs[i]->iteration < 3)
        {
          jobsDone = false;
          //cout << "job " << i << "not done: " << CurTime << " < " << listOfJobs[i]->period << endl;
        }
      } 
      else if (listOfJobs[i]->done == false)
      {
        jobsDone = false;
        //cout << "job " << i << "not done." << CurTime << endl;
      }   
    }
    
    //cout << "5" << endl;
    
    if (jobsDone)
    {
      finished = true;
      FinishTime = CurTime;
      results << avgNormResponseTime() << ',' 
              << percentMissedDeadlines() << ',';
    }
    //cout << "6" << endl;
  }
}

void Scheduler::LST_EDF()
{
  while (!finished)
  {
    //cout << endl << "0" << endl;
    bool jobsDone = true;
    bool JobsReady = false;
    bool findNewJob = true;
    
    Job * job_to_exec = NULL;
    
    //cout << endl << "2" << endl;
    if (findNewJob)
    {
      //cout << CurTime << ": \t";
      for (int i = 0; i < numJobs; i++)  //finds first job ready
      {
        //cout << listOfJobs[i]->releaseTime << '\t';
        
        if ((listOfJobs[i]->releaseTime <= CurTime) && (!listOfJobs[i]->done))
        {
          JobsReady = true;
          job_to_exec = listOfJobs[i];
          //cout << "found job ready" << '\t';
        }       
      }
      
      if (JobsReady)
      {
        //cout << "jobs ready" << endl;
        
        for (int i = 0; i < numJobs; i++)
        {
          if ((!listOfJobs[i]->done) && (listOfJobs[i]->releaseTime < CurTime))
          {
            if (job_to_exec->deadline > listOfJobs[i]->deadline)
            {
              job_to_exec = listOfJobs[i];
            }
            else if ((job_to_exec->deadline == listOfJobs[i]->deadline) && (job_to_exec->jobNum != listOfJobs[i]->jobNum))
            {
              /*cout << job_to_exec->jobNum << " slack time of " << job_to_exec->slackTime << " vs " 
                   << listOfJobs[i]->jobNum << " slack time of " << listOfJobs[i]
                     ->slackTime << endl;*/
              if (job_to_exec->slackTime > listOfJobs[i]->slackTime)
              {
                job_to_exec = listOfJobs[i];
              }
              //cout << "executing " << job_to_exec->jobNum << endl;
            }
            //cout << "new job" << endl;
          }
        }
      }
      else
      {
        //cout << "no job ready" << endl;
      }
    }
    
    //cout << "3" << endl;
    
    if (job_to_exec != NULL)
    {
      /*cout << CurTime << ": attempting to execute job " << job_to_exec->jobNum
           << " with time remaining " << job_to_exec->timeRemaining << endl;*/
      
      job_to_exec->Execute(CurTime);
    }
    
    CurTime++;
    
    //cout << "4" << endl;
      
    for (int i = 0; i < numJobs; i++)
    {
      if (job_to_exec != NULL)
      {
        if (listOfJobs[i]->jobNum == job_to_exec->jobNum)
        {
          if (job_to_exec->done)
          {
            listOfJobs[i]->output += "D";
          }
          else
          {
            listOfJobs[i]->output += "X";
          }
        }
        else
        {
          listOfJobs[i]->output += " ";
        }
               
      }
      else
      {
        listOfJobs[i]->output += " ";
      }
      
      if (listOfJobs[i]->periodic)
      {
        if (CurTime < listOfJobs[i]->period || listOfJobs[i]->iteration < 3)
        {
          jobsDone = false;
          //cout << "job " << i << "not done: " << CurTime << " < " << listOfJobs[i]->period << endl;
        }
      } 
      else if (listOfJobs[i]->done == false)
      {
        jobsDone = false;
        //cout << "job " << i << "not done." << CurTime << endl;
      }   
    }
    
    //cout << "5" << endl;
    
    if (jobsDone)
    {
      finished = true;
      FinishTime = CurTime;
      results << avgNormResponseTime() << ',' 
              << percentMissedDeadlines() << ',';
    }
    //cout << "6" << endl;
  }
}

void Scheduler::CriticalWindowEDF()
{
  
}

void Scheduler::resetAllJobs()
{
  for (int i = 0; i < numJobs; i++)
  {
    listOfJobs[i]->resetJob();
  }
  CurTime = 0;
  FinishTime = 0;
  
  finished = false;  
}

float Scheduler::avgNormResponseTime()
{
  float avgResponseTime = 0;  
  for (int i = 0; i < numJobs; i++)
  {
    if (listOfJobs[i]->periodic)
    {
      avgResponseTime += (listOfJobs[i]->avgRespTime / (float)(listOfJobs[i]->execTime));
    }
    else
    {
      avgResponseTime += (listOfJobs[i]->respTime / (float)(listOfJobs[i]->execTime));
    }
  }  
  avgResponseTime = (avgResponseTime /  (float)(numJobs));  
  return avgResponseTime;
}

float Scheduler::percentMissedDeadlines()
{
  float percentMissed = 0;  
  for (int i = 0; i < numJobs; i++)
  {
    if (listOfJobs[i]->deadline < listOfJobs[i]->FinishTime)
    {
      percentMissed++;
    }
  } 
  return (percentMissed / (float)(numJobs));
}

float Scheduler::avgExecTime()
{
  float avgExecTime = 0;  
  for (int i = 0; i < numJobs; i++)
  {
    avgExecTime += (listOfJobs[i]->execTime);
  }  
  avgExecTime = (avgExecTime / (float)(numJobs));  
  return avgExecTime;
}

int Scheduler::execRange()
{
  int smallestExec;
  int largestExec;
  
  smallestExec = largestExec = listOfJobs[0]->execTime;
  
  for (int i = 0; i < numJobs; i++)
  {
    if (listOfJobs[i]->execTime > largestExec)
    {
      largestExec = listOfJobs[i]->execTime;
    }
    else if (listOfJobs[i]->execTime < smallestExec)
    {
      smallestExec = listOfJobs[i]->execTime;
    }
  }
  
  return (largestExec - smallestExec);
}

float Scheduler::avgPeriod()
{
  float avgPeriod = 0;  
  for (int i = 0; i < numJobs; i++)
  {
    avgPeriod += (listOfJobs[i]->period);
  }  
  avgPeriod = (avgPeriod / (float)(numJobs));  
  return avgPeriod;
}

int Scheduler::PeriodRange()
{
  int smallestPeriod;
  int largestPeriod;
  
  smallestPeriod = largestPeriod = listOfJobs[0]->period;
  
  for (int i = 0; i < numJobs; i++)
  {
    if (listOfJobs[i]->period > largestPeriod)
    {
      largestPeriod = listOfJobs[i]->period;
    }
    else if (listOfJobs[i]->period < smallestPeriod)
    {
      smallestPeriod = listOfJobs[i]->period;
    }
  }
  
  return (largestPeriod - smallestPeriod);
}

float Scheduler::avgDeadline()
{
  float avgDeadline = 0;  
  for (int i = 0; i < numJobs; i++)
  {
    if (listOfJobs[i]->periodic)
    {
      avgDeadline += (listOfJobs[i]->relDeadline);
    }
    else
    {
      avgDeadline += (listOfJobs[i]->deadline);
    }
  }  
  avgDeadline = (avgDeadline / (float)(numJobs));  
  return avgDeadline;
}

int Scheduler::deadlineRange()
{
  int smallestDeadline;
  int largestDeadline;
  
  smallestDeadline = largestDeadline = listOfJobs[0]->deadline;
  
  for (int i = 0; i < numJobs; i++)
  {
    if (listOfJobs[i]->deadline > largestDeadline)
    {
      largestDeadline = listOfJobs[i]->deadline;
    }
    else if (listOfJobs[i]->deadline < smallestDeadline)
    {
      smallestDeadline = listOfJobs[i]->deadline;
    }
  }
  
  return (largestDeadline - smallestDeadline);
}

void Scheduler::getResults()
{
  results << numJobs << ','
          << numPeriodic << ','
          << minPeriod << ','
          << maxPeriod << ','
          << minExecTime << ','
          << maxExecTime << ','
          << minDeadline << ','
          << maxDeadline << ','
          << minReleaseTime << ','
          << maxReleaseTime << ','
          << FinishTime << ','
          << Utilization << ',';

  results << avgExecTime() << ','
          << avgPeriod() << ','
          << avgDeadline() << '\n';
}

void Scheduler::initResultsCSV()
{
  results << "algorithm,norm resp time,num missed deadlines,num jobs,"
          << "num periodic jobs,min period,max period,min exec time,max exec time," 
          << "min deadline,max deadline,min rel time,max rel time,finish time,utilization,"
          << "avgExecTime,avgPeriod,avgDeadline\n";
  return;
}