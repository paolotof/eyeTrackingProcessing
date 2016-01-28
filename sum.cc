#include "main.ih"

double sum(double v[], size_t n)
{
  size_t i;
  double s;
  
  for(i = 0, s = 0; i < n; s += v[i++]);
//     cout << s << ' ';
  
//   cout << '\n';
  return s;
}