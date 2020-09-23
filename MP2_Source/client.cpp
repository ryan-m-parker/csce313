/* 
    File: client.cpp

    Author: R. Parkekr
            Department of Computer Science
            Texas A&M University
    Date  : 2020/09/22

    Client main program for MP2 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <errno.h>
#include <unistd.h>

#include "reqchannel.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string int2string(int number) {
  std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

void print_time_diff(const std::string _label,
                     const struct timeval & _tp1,
                     const struct timeval & _tp2) {
    // Prints to stdout the difference, in seconds and museconds, between
    // two timevals.
    
    std::cout << _label;
    long sec = _tp2.tv_sec - _tp1.tv_sec;
    long musec = _tp2.tv_usec - _tp1.tv_usec;
    if (musec < 0) {
        musec += 1000000;
        sec--;
    }
    std::cout << " [sec = " << sec << ", musec = " << musec << "]" << std::endl;
    
}

std::string generate_data() {
  // Generate the data to be returned to the client.
  return int2string(rand() % 100);
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  if(fork() == 0) {
    char *args[]={"./dataserver", nullptr};
    execvp(args[0], args);
    std::cout << "test" << std::endl;
  }







  std::cout << "CLIENT STARTED:" << std::endl;

  std::cout << "Establishing control channel... " << std::flush;
  RequestChannel chan("control", RequestChannel::Side::CLIENT);
  std::cout << "done." << std::endl;

  /* -- Start sending a sequence of requests */

  std::string reply1 = chan.send_request("hello");
  std::cout << "Reply to request 'hello' is '" << reply1 << "'" << std::endl;

  std::string reply2 = chan.send_request("data Joe Smith");
  std::cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << std::endl;

  struct timeval timeStart_1;
  struct timeval timeEnd_1;

  assert(gettimeofday(&timeStart_1, 0) == 0);

  std::string reply3 = chan.send_request("data Jane Smith");
  std::cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << std::endl;

  assert(gettimeofday(&timeEnd_1, 0) == 0);

  for(int i = 0; i < 100; i++) {
    std::string request_string("data TestPerson" + int2string(i));
    std::string reply_string = chan.send_request(request_string);
    std::cout << "reply to request " << i << ":" << reply_string << std::endl;;
  }

  struct timeval timeStart_2;
  struct timeval timeEnd_2;

  assert(gettimeofday(&timeStart_2, 0) == 0);

  generate_data();

  assert(gettimeofday(&timeEnd_2, 0) == 0);

 
  std::string reply4 = chan.send_request("quit");
  std::cout << "Reply to request 'quit' is '" << reply4 << std::endl;

  print_time_diff("Time taken for computation remotely: ", timeStart_1, timeEnd_1);
  print_time_diff("Time taken for computation localy: ", timeStart_2, timeEnd_2);

  usleep(1000000);
}
