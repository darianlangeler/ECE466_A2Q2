#include "systemc.h"
#include "fifo_if.h"
#include <iostream>
#include <iomanip>
using namespace std;

template <class T, unsigned N> class fifo : public sc_module, public fifo_out_if <T>, public fifo_in_if <T>
{

private:
  T data[N];
  int free, size, ri, wi;
  bool read_flag, write_flag, read_status, write_status;
  sc_event read_req, write_req, done;
  T* read_c;
  T write_c;

public:
  SC_HAS_PROCESS(fifo);

  fifo(sc_module_name nm): sc_module(nm)
  {
    //Starting Values
    free = N;
    ri = 0;
    wi = 0;
    read_flag     = false;
    write_flag    = false;
    read_status   = false;
    write_status  = false;
    SC_THREAD(arbitrator);
    sensitive << read_req << write_req;
  }

  // Refer to deterministic stack.h in lecture notes
  void arbitrator() {
    while(1) {
      //Handle read flag first
      if(read_flag) {
        read_flag = false;
        if (free < N) {
          *read_c = data[ri];
          ri = (ri + 1) % N;
          free++;
          read_status = true;
        }
        else read_status = false;
      }

      //Write flag handled after any reads
      if(write_flag) {
        write_flag = false;
        if (free > 0) {
          data[wi] = write_c;
          wi = (wi + 1) % N;
          free--;
          write_status = true;
        }
        else write_status = false;
      }
      
      //All event flags handled, state finished
      done.notify();
      wait();
    } 
  }

  bool write(T data)
  {
    write_flag = true;
    write_c = data;
    write_req.notify();
    wait(done);
    return write_status;
  }

  bool read(T& data) 
  {
    read_flag = true;
    read_c = &data;
    read_req.notify();
    wait(done);
    return read_status;
  }

};