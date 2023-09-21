import ned.IBidirectionalChannel;
import org.omnetpp.queueing.Delay;
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
import org.omnetpp.queueing.PassiveQueue;
import org.omnetpp.queueing.Queue;
import org.omnetpp.queueing.Server;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;
import org.omnetpp.queueing.SourceOnce;



//
// TODO documentation
//


network Rete
{
    parameters:
        int numServers @prompt("Number of servers") = default(4);
        //string moduleName = "Nserver";
        //volatile double ServiceTime @unit(s);
        string fetchingAlgorithm @enum("priority","random","roundRobin","longestQueue") = default("priority");


        @display("bgb=837.936,494.086");
    types:

    submodules:
        ElasticQueue: PassiveQueue {
            @display("p=196,90");
        }
        ElasticSource: Source {
            @display("p=60,91");
        }
        InelasticSource: Source {
            @display("p=58,200");
        }
        ElasticSink: Sink {
            @display("p=717.324,241.224");
        }
        InelasticQueue: PassiveQueue {
            @display("p=194.672,199.962");
        }

        InelasticSink: Sink {
            @display("p=703.28625,343.75876");
        }
        server: Server {
            @display("p=419.44873,201.84");
        }
        delay: Delay {
            @display("p=554.0087,131.40625");
        }
    connections:
        ElasticSource.out --> ElasticQueue.in++;
        InelasticSource.out --> InelasticQueue.in++;
        ElasticQueue.out++ --> server.in++;
        InelasticQueue.out++ --> server.in++;
        server.out++ --> InelasticSink.in++;
        server.out++ --> delay.in++;
        delay.out --> ElasticSink.in++;
}

