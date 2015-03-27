#include "dataline.ih"

void Dataline::detValidity(double (*Dataline::g_psize)())
{
  Dataline::setValidity(true);
  if (psize == 0.0)
    Dataline::setValidity(false);

}