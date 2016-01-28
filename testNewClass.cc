#include "main.ih"

void testNewClass()
{
  ifstream shortBlinksReport("blinksPerSubjectSmaller300.txt");
  
  if (not shortBlinksReport.is_open())
    cout << "blinksPerSubjectSmaller300.txt does not exists\n";
  else
  {
    string line1;
    string line2;
//     Interpdata data.extractData(line);
    Interpdata data;
    while (getline(shortBlinksReport, line1))
    {
      getline(shortBlinksReport, line2);
      data = data.extractInterpData(line1, line2);
      cout << data.sub() << '\n';
    } 
  }
}