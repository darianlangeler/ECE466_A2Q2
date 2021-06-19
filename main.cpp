#include "systemc.h"
#include "fifo.h"
#include "producer.h"
#include "consumer.h"
#include <iostream>
#include <iomanip>
using namespace std;

int sc_main(int argc, char* argv[]){
	//Producer Clock
    sc_clock pClk("pClk", 10, SC_NS, 0.5, 1, SC_NS);

    //Consumer Clock
    sc_clock cClk("cClk", 20, SC_NS, 0.5, 1, SC_NS);

    // FIFO of size 4, type char
    fifo<int, 4> FF("fifo");

    producer<int> PRODUCER("producer");
    PRODUCER.clk(pClk); 
    PRODUCER.out(FF);

    consumer<int> CONSUMER("consumer");
    CONSUMER.clk(cClk); 
    CONSUMER.in(FF);

    sc_start(250, SC_NS);
    return 0;
}