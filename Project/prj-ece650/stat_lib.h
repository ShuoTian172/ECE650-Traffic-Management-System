//
// Created by Morgan Wu on 2018/12/9.
//

#ifndef PRJ_ECE650_STAT_LIB_H
#define PRJ_ECE650_STAT_LIB_H

#include <vector>
#include <math.h>

double Variance(std::vector<double> samples)
{
  int size = samples.size();
  if(size <= 1) {
    return 0;
  }

  double variance = 0;
  double t = samples[0];
  for (int i = 1; i < size; i++)
  {
    t += samples[i];
    double diff = ((i + 1) * samples[i]) - t;
    variance += (diff * diff) / ((i + 1.0) *i);
  }

  return variance / (size);
}

double StandardDeviation(std::vector<double> samples)
{
  return sqrt(Variance(samples));
}

double Average(std::vector<double> samples) {
  int size = samples.size();
  if(size <= 0) {
    return 0;
  }
  double sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += samples[i];
  }
  return sum / size;
}

bool doubleEqual(double a, double b){
  return (a-b) < 0.0000001;
}



#endif //PRJ_ECE650_STAT_LIB_H
