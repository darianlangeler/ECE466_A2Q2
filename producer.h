#include "systemc.h"
#include "fifo_if.h"
#include <iostream>
#include <iomanip>

using namespace std;

template <class T> class producer : public sc_module 
{
public:
  sc_port < fifo_out_if<T> > out;
  sc_in_clk clk;

  void do_writes()
  {
    T data = 0;
    while (true) 
    {
      
      if (out->write(data)) 
        cout << "W " << data << " at " << sc_time_stamp() << endl;
  
      // Cycle data from 0-9
      data = (data + 1) % 10;
      wait();
    }
  }

  SC_CTOR(producer)
  {
    // Use thread due to wait
    SC_THREAD(do_writes); sensitive << clk.pos();
  }
};