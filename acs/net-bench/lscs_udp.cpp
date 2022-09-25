/**
 *****************************************************************************
 *
 * @file lscs_udp.cpp
 *      LSCS Test Server For Network Benchmarking.
 *
 * @par Project
 *      TMT Primary Mirror Control System (M1CS) \n
 *      Jet Propulsion Laboratory, Pasadena, CA
 *
 * @author	Brad Hines
 * @date	23-Sept-2022 -- Initial delivery.
 *
 * Copyright (c) 2022, California Institute of Technology
 *
 *****************************************************************************/



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>

#include <list>
#include <iostream>
#include <fstream>

#include "GlcMsg.h"
#include "GlcLscsIf.h"
#include "Client.h"
#include "UdpPorts.h"

#define MAXMSGLEN	1024

using namespace std;

bool bDebug = false;
bool b_fFlagIsPresent = false;
bool b_pFlagIsPresent = false;
bool b_nFlagIsPresent = false;
bool b_hFlagIsPresent = false;
string sFilename;
tClientList ClientList;


// Values when the info is not provided from a file
string sHostIpAddressString = "10.0.0.1";
int iNumClients  = 0;
int iNextPortNum = M1CS_DEFAULT_FIRST_UDP_PORT;
int iLastPortNum = (M1CS_DEFAULT_FIRST_UDP_PORT + M1CS_DEFAULT_NUM_UDP_PORTS - 1);


/*****************************
* PopulateFromFile
*
* If a portnum is not specified in the file, just autoincrement from 
*/

int PopulateFromFile(string sFilename)
{
  std::string sHostIpAddressString;
  int         iPortNum = -1;
  std::ifstream HostListFile(sFilename);

  while (HostListFile >> sHostIpAddressString >> iPortNum) {
    if (iPortNum == -1) { // Port num not supplied, will autoincrement from defaults
      iPortNum = iNextPortNum++;
    }
    ClientList.AddClient(sHostIpAddressString, iPortNum);
    iPortNum = -1;
  }

  return 0;
}


/*****************************
* PopulateFromValues
*
* 
*/

int PopulateFromValues()
{
  for ( ; iNextPortNum <= iLastPortNum; iNextPortNum++) {
    ClientList.AddClient(sHostIpAddressString, iNextPortNum);
    cout << "Added client targeting " << sHostIpAddressString << "::" << iNextPortNum << endl;
  }

  return 0;
}


/*****************************
* TraverseArgList
*
*/

int TraverseArgList(const char *sArgList[])
{
  const char *sProgramName = sArgList[0];
  const char *sArg = *sArgList++;

  while (sArg != NULL) {
    if (!strcmp(sArg, "-help")) {
      cout << "Usage: " << sProgramName << " [-d] [-f client_ip_list_filename] [-h host_ip] [-p first_server_port] [-n num_clients] " << endl;
      cout << "  You must either provide either -f or -h, not both" << endl;
      cout << "  The -p/-n are optional.  If you do not provide them, defaults will be used." << endl;
      cout << "  If you provide -f, you can include port numbers in the file, or use the -p argument" << endl;
      cout << "  * -f should provide the filename of a list of IP addresses to masquerade as, with optional server target ports" << endl;
      cout << "  * -p first_server_port numports" << endl;
      cout << "  * If the -t option is provided the program will launch its server threads at that priority" << endl;
      cout << "    realtime priority thread_priority, from 1-99, with 99 being highest.   " << endl;
      cout << "  * -p: One server thread will be created for each port in the range" << endl;
      cout << "  * -d is the debug flag.  Doesn't do anything at present." << endl << endl;

      exit(0);
    }
    else if (!strcmp(sArg, "-f"))  {
      b_fFlagIsPresent = true;
      sFilename = *sArgList++;
    }

    else if (!strcmp(sArg, "-h"))  {
      sHostIpAddressString = *sArgList++;
      b_hFlagIsPresent = true;
    }

    else if (!strcmp(sArg, "-p"))  {
      iNextPortNum = atoi(*sArgList++);
      if (iNextPortNum <=0) {
        throw std::runtime_error("Invalid value for -p argument");
      }
      b_pFlagIsPresent = true;
    }

    else if (!strcmp(sArg, "-n"))  {
      iNumClients   = atoi(*sArgList++);
      if (iNumClients < 1) {
        throw std::runtime_error("Invalid value for -p argument");
      }
      b_nFlagIsPresent = true;
    }

    else if (!strcmp(sArg, "-d")) {
      bDebug = true;
    }

    sArg = *sArgList++;
  }

  // Check for valid combinations
  if (b_hFlagIsPresent && b_fFlagIsPresent) return -1;

  if (b_nFlagIsPresent)  iLastPortNum = iNextPortNum + iNumClients - 1;

  return 0;
}

#if 0
void timer_handler(int sig, siginfo_t *si, void *uc)
{
    UNUSED(sig);
    UNUSED(uc);
    struct t_eventData *data = (struct t_eventData *) si->_sifields._rt.si_sigval.sival_ptr;
    printf("Timer fired %d - thread-id: %d\n", ++data->myData, gettid());
}
#endif


/*****************************
* main
*/

int main (int argc, const char **argv)
{
  timer_t ticker;
  struct sigevent event;

  if (TraverseArgList(argv) < 0) {
    cerr << "Error: Invalid switch combination supplied, try " << argv[0] << " -help" << endl;
  }

  if (b_fFlagIsPresent)  PopulateFromFile(sFilename);
  else                   PopulateFromValues();

  #if 0 // Prepare the periodic timer
    event. 

    if (timer_create(CLOCK_MONOTONIC, struct sigevent *sevp, &ticker) < 0) {
      throw std::runtime_error("Error creating timer");
    }
  #endif

  while (1) { 
    ClientList.EmitMessagesFromAll();
    sleep(1);
  }

}
