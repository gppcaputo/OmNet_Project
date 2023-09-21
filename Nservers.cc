//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Nservers.h"
#include "Sink.h"
#include "cstring"
#include "Job.h"

using std::to_string;
using namespace std;


Define_Module(Nservers);





void Nservers::initialize()
{
        cModule *parent = getParentModule();
        numServers = parent->par("numServers");

        //parent->setSubmoduleVectorSize("server", numServers+1);
        cModuleType *serverModuleType = cModuleType::get("org.omnetpp.queueing.Server");
                //cModule *serverModule = moduleType->create(name.c_str(), getParentModule());


        cout << parent->getName() << endl;
        cout << serverModuleType->getName() << endl;

           cModule *servers[numServers];
           memset(servers, 0, numServers * sizeof(*servers));

//         cModule *nser = getModuleByPath("nservers");
//         nser->setGateSize("outs", 1);

           cModule *sink = getModuleByPath("sink");
           sink->setGateSize("in", 2);

           cModule *queueE = getModuleByPath("ElasticQueue");
           queueE->setGateSize("out", 1);

           cModule *queueI = getModuleByPath("InelasticQueue");
           queueI->setGateSize("out", 2);

           // cModule *Server = getModuleByPath("server");
           //Server->setGateSize("out",2);

          // parent->setSubmoduleVectorSize("servers", numServers +1);

           for (int i = 0; i < numServers; i++)
           {
               std::string serverName = "server_" + to_string(i);
               cModule *serverModule = serverModuleType->create(serverName.c_str(), parent);

               servTime = parent->par("ServiceTime").doubleValue();

               serverModule->setGateSize("in",numServers+5);
               serverModule->setGateSize("out",numServers+5);

               serverModule->par("serviceTime") = servTime;
               serverModule->par("fetchingAlgorithm");
               serverModule->finalizeParameters();
               serverModule->buildInside();
               serverModule->scheduleStart(simTime());

               servers[i] = serverModule;

           }

               queueE->gate("out",0)->connectTo(servers[0]->gate("in",0));
               queueI->gate("out",1)->connectTo(servers[0]->gate("in",1));
               //Server->gate("out",0)->connectTo(servers[0]->gate("in",0));
               //Server->gate("out",1)->connectTo(servers[0]->gate("in",1));
               //Server->gate("out",1)->connectTo(sink->gate("in",1));

               servers[0]->gate("out", 1)->connectTo(sink->gate("in",1));


              for(int i = 0; i < numServers; i++){
                  if(i == numServers -1){
                      servers[i]->gate("out", i)->connectTo(sink->gate("in",0));
                  }else{
                      servers[i]->gate("out", i)->connectTo(servers[i+1]->gate("in", i+1));

                  }
              }

}

void Nservers::handleMessage(cMessage *msg)
{

}

void Nservers::finish(){
       // TODO Auto-generated destructor stub
    delete[] servers;
    servers = NULL;
}



