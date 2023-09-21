//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package org.omnetpp.queueing;

//
// Queue server. It serves multiple input queues (PassiveQueue), using a preset
// algorithm. Inputs must be connected to Passive Queues (PassiveQueue)
//
simple Server
{
    parameters:
        @group(Queueing);
        @display("i=block/server");
        @signal[busy](type="bool");
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
               
        string fetchingAlgorithm @enum("priority","random","roundRobin","longestQueue") = default("priority");
             // how the next job will be choosen from the attached queues
        string fetchingAlgorithm2 @enum("priority","random","roundRobin","longestQueue") = default("priority");
             // how the next job will be choosen from the attached queues
        volatile double serviceTime @unit(s); // service time of a job
        volatile double serviceTimeEl @unit(s); // service time for Elastic Jobs
        volatile double serviceTimeIn @unit(s); // service time for Inelatic Jobs
        
        //volatile double serviceTimeIn2 @unit(s); // inelastic service time with 2 servers
        //volatile double serviceTimeIn4 @unit(s); // inelastic service time with 4 servers
      	//volatile double serviceTimeIn6 @unit(s); // inealstic service time with 6 servers
      	
      	//volatile double serviceTimeEl2 @unit(s); // elastic service time with 2 servers
        //volatile double serviceTimeEl4 @unit(s); // elastic service time with 4 servers
      	//volatile double serviceTimeEl6 @unit(s); // ealstic service time with 6 servers
                
        
        
       	
    gates:
       	input in[];
       	output out[];
        
}


