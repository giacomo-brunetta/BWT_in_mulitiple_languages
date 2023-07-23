
#if defined(unix) || defined(__unix__) || defined(__unix)

#include <time.h>

clock_t timer1;
clock_t timer2;

void set_frequency(){
  return;
}

void start_timer(){
  timer1=clock();
}

void end_timer(){
  timer2=clock();
}

double get_time_elapsed(){
  return (((double)timer2)-((double)timer1))/CLOCKS_PER_SEC;
}

#elif WIN32

#include <windows.h>

LARGE_INTEGER timer1;
LARGE_INTEGER timer2;
LARGE_INTEGER frequency;

void set_frequency(){
  QueryPerformanceFrequency(&frequency);
}

void start_timer(){
  QueryPerformanceCounter(&timer1);
}

void end_timer(){
  QueryPerformanceCounter(&timer2);
}

double get_time_elapsed(){
  return ((double)(timer2.QuadPart - timer1.QuadPart))/frequency.QuadPart;
}

#endif
