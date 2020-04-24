#include <math.h>

double julianDat(int year, int month, int day) 
{
  const int timeZone = 2; /// ??????????

  double  zone = -(timeZone * 60 / 1440.0);
  
  if (month <= 2)
  {
    year -= 1;
    month += 12;
  }
  double day2 = day + zone + 0.5;
  double A = floor(year / 100.0);
  double B = 2 - A + floor(A / 4.0);
  double JD = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day2 + B - 1524.5;
  return JD;
}

double proper_ang(double big)
{
  double tmp = 0;
  if (big > 0)
  {
    tmp = big / 360.0;
    tmp = (tmp - floor(tmp)) * 360.0;
  }
  else
  {
    tmp = ceil(fabs(big / 360.0));
    tmp = big + tmp * 360.0;
  }

  return tmp;
}

/// Given the year/month/day, calculate the lunar day (0-27)
/// year: Standard CE year
/// month: Calendar Month (1-12?)
/// day: day of month (1-31?)
int moonPhases(int year, int month, int day)
{
  double jd = julianDat(year, month, day);  // calculate Julian Date
  double dr = M_PI / 180.0;
  double rd = 1 / dr;
  unsigned long  meeDT = pow((jd - 2382148L), 2) / (41048480LL * 86400LL);

  double meeT = (jd + meeDT - 2451545.0) / 36525;
  unsigned long meeT2 = pow(meeT, 2);
  unsigned long meeT3 = pow(meeT, 3);
  double meeD = 297.85 + (445267.1115 * meeT) - (0.0016300 * meeT2) + (meeT3 / 545868);
  meeD = proper_ang(meeD) * dr;
  double meeM1 = 134.96 + (477198.8676 * meeT) + (0.0089970 * meeT2) + (meeT3 / 69699);
  meeM1 = proper_ang(meeM1) * dr;
  double meeM = 357.53 + (35999.0503 * meeT);
  meeM = proper_ang(meeM) * dr;

  double elong = meeD * rd + 6.29 * sin(meeM1);
  elong = elong - 2.10 * sin(meeM);
  elong = elong + 1.27 * sin(2 * meeD - meeM1);
  elong = elong + 0.66 * sin(2 * meeD);
  elong = proper_ang(elong);
  elong = round(elong);
  double moonNum = ((elong + 6.43) / 360) * 28;
  moonNum = floor(moonNum);
  
  if (moonNum == 28)
  {
    moonNum = 0;
  }

  return moonNum;
}
