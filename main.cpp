#include "scheduler.h"

int main()
{
  srand(time(NULL));
  Scheduler EDF_test;
  EDF_test.results.open("results.csv");
  EDF_test.initResultsCSV();
  
  for (int i = 0; i < MAX_NUM_TEST; i++)
  {
    EDF_test.getTestInput(i);
    for (int j = 0; j < MAX_REPEAT; j++)
    {
      if (EDF_test.getSchedulableJobSet())
      {
        EDF_test.ListJobs();
      
        cout << endl << "\nEarliest Deadline First (With preemptions)\n";
        EDF_test.results << "EDF Preempt,";
        EDF_test.PreemptEDF();
        EDF_test.printScheduleHoriz();
        EDF_test.ListJobs();
        EDF_test.getResults();
      
        EDF_test.resetAllJobs();
  
        cout << "\nEarliest Deadline First (No preemptions)\n";
        EDF_test.results << "EDF No Preempt,";
        EDF_test.NoPreemptEDF();
        EDF_test.printScheduleHoriz();
        EDF_test.ListJobs();
        EDF_test.getResults();
  
        EDF_test.resetAllJobs();
  
        cout << endl << "\nEarliest Deadline First (With Least Slack Time)\n";
        EDF_test.results << "EDF/LST,";
        EDF_test.LST_EDF();
        EDF_test.printScheduleHoriz();
        EDF_test.ListJobs();
        EDF_test.getResults();
    
        EDF_test.resetAllJobs();
      }      
    }
  }  
  EDF_test.results.close(); 
  return 0;
}


/*
int main()
{
  srand(time(NULL));
  char input = 'x';
  
  Scheduler EDF_test;
  EDF_test.results.open("results.csv");
  EDF_test.initResultsCSV();
  
  while (input == 'y' || input == 'x')
  {
    if (input == 'x')
    {
      EDF_test.GetInput();
    }
    
    if (EDF_test.getSchedulableJobSet())
    {
      EDF_test.ListJobs();
      
      cout << endl << "\nEarliest Deadline First (With preemptions)\n";
      EDF_test.results << "EDF Preempt,";
      EDF_test.PreemptEDF();
      EDF_test.printScheduleHoriz();
      EDF_test.ListJobs();
      EDF_test.getResults();
      
      EDF_test.resetAllJobs();
  
      cout << "\nEarliest Deadline First (No preemptions)\n";
      EDF_test.results << "EDF No Preempt,";
      EDF_test.NoPreemptEDF();
      EDF_test.printScheduleHoriz();
      EDF_test.ListJobs();
      EDF_test.getResults();
  
      EDF_test.resetAllJobs();
  
      cout << endl << "\nEarliest Deadline First (With Least Slack Time)\n";
      EDF_test.results << "EDF/LST,";
      EDF_test.LST_EDF();
      EDF_test.printScheduleHoriz();
      EDF_test.ListJobs();
      EDF_test.getResults();
    
      EDF_test.resetAllJobs();
    }
  
    cout << "To repeat simulation with same parameters, enter y. To change parameters, enter x. To end program, enter any other key" << endl;
    cin >> input;
  } 
  EDF_test.results.close(); 
  
  return 0;
}
*/
