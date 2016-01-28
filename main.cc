#include "main.ih"


/*

  inputs:
  1) names of files with trials to process:
  - a) trials2beIncluded_withFillers.txt
  - b) trials2beIncluded_noFillers.txt
  2) third input specifying whether to use Charlotte ROI or not
       if there is no 3rd argument normal ROI is used.

 */

int main(int argc, char **argv)
try
{
  if (argc == 1)
  {
    bool charlotte = false;
    cout << "Normal figures sizes, not Charlotte\n";
    allAtOnce(charlotte, "trials2beIncluded_noFillers.txt");
    allAtOnce(charlotte, "trials2beIncluded_withFillers.txt");

  }

  if (argc > 2)
    selectProcessing(argc, argv);
  else
  {
    cout << "Before selection\n";

    string filename = argv[1];
    cout << filename << '\n';
    if (filename.find(".txt") != string::npos)
      selectProcessing(argc, argv);
    else
    { // do all at once
      
      bool charlotte;
      string secondInput = argv[1];
      secondInput == "c" ? charlotte = true  : charlotte = false;
      cout << secondInput << '\n';
      allAtOnce(charlotte, "trials2beIncluded_noFillers.txt");
      allAtOnce(charlotte, "trials2beIncluded_withFillers.txt");
    }
  }
  
  cout << "Finished processing\n";  
  
  return 0;
  
} // end main
catch (...)
{
    return 1;
}
