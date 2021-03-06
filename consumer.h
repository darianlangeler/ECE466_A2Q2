#include "systemc.h"
#include "fifo_if.h"
#include <iostream>
#include <iomanip>

using namespace std;

template <class T> class consumer : public sc_module 
{
public:
  sc_port < fifo_in_if<T> > in;
  sc_in_clk clk;
  
  void do_reads()
  {
    T data;
    while (true) 
    {
      
      if (in->read(data)) 
        cout << "R " << data << " at " << sc_time_stamp() << endl;
  
      wait();
    }
  }

  SC_CTOR(consumer)
  {
    // Use thread due to wait
    SC_THREAD(do_reads); sensitive << clk.pos();
  }
}; 