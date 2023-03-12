#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp
  void update(long newIdleTime, long newActiveTime,long newTotalTime);

  // TODO: Declare any necessary private members
 private:
  
  long m_PreIdleTime;
  long m_PreActiveTime;
  long m_PreTotalTime;
};

#endif