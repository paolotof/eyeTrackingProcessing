//                     usage.cc

#include "main.ih"

void usage(std::string const &progname)
{
    cout << "\n" <<
    progname << " by " << Icmbuild::author << "\n" <<
    progname << " V" << Icmbuild::version << " " << Icmbuild::years << "\n"
    "\n"
    "Usage: " << progname << " [options] args\n"
    "Where:\n"
    "   [options] - optional arguments (short options between parentheses):\n"
    "      --help (-h)      - provide this help\n"
    "      --version (-v)   - show version information and terminate\n"
    "   args   - explain additional arguments.\n"
    " \n"  
    " [empty] argument defaults to generation of 2 files for each participant \n"
    " file 1 is the 'delay file', file 2 is the file with all data after the\n"
    " onset of the target word\n"
    "\n"
    " [tbt] trial-by-trial generates a single file with the eyetracking of each\n"
    " participant \n"
    " [bin] generates file with observations binned together (i.e. averaged \n"
    " accross data points). Bin must be a number reflecting how many \n"
    " observations should be binned together.\n"
    "\n";
}
