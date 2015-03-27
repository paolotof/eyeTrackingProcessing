#include "main.ih"

double mean(double v[], size_t n)
{
  return sum(v, n) / n;
}