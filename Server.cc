//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Server.h"
#include "Job.h"
#include "SelectionStrategies.h"
#include "IPassiveQueue.h"
#include <string.h>

using namespace std;
using std::to_string;



namespace queueing {

Define_Module(Server);

Server::~Server()
{
    delete selectionStrategy;
    delete jobServiced;
    cancelAndDelete(endServiceMsg);
}


void Server::initialize()
{
    busySignal = registerSignal("busy");
    emit(busySignal, false);
    endServiceMsg = new cMessage("end-service");
    jobServiced = nullptr;
    allocated = false;
    selectionStrategy = SelectionStrategy::create(par("fetchingAlgorithm"), this, true);
    if (!selectionStrategy)
        throw cRuntimeError("invalid selection strategy");



}


void Server::handleMessage(cMessage *msg)
{
    cModule *parent = getParentModule();
    int numServers = parent->par("numServers").intValue();

    if (msg == endServiceMsg) {
        if (jobServiced == nullptr) {
            error("endServiceMsg received while no job is being serviced");
            return;
        }

        int kind = jobServiced->getKind();

        simtime_t d = simTime() - endServiceMsg->getSendingTime();
        jobServiced->setTotalServiceTime(jobServiced->getTotalServiceTime() + d);

        cGate *out = gate("out", 0);
        //auto index = getIndex();

        //if (index == 0) {
        if (kind == 2) {
            sendJobIn(jobServiced);
        } else {
            sendJobEl(jobServiced);
        }
        /*} else if (index == numServers-1) {
            send(jobServiced, out);
        } else {
            send(jobServiced, out);
            check_and_cast<IServer *>(out->getPathEndGate()->getOwnerModule())->allocate();
        }*/

        jobServiced = nullptr;
        allocated = false;
        emit(busySignal, false);


        //if (index == 0) {
        int k = selectionStrategy->select();
        if (k >= 0) {
            EV << "requesting job from queue " << k << endl;
            cGate *gate = selectionStrategy->selectableGate(k);

            check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
        }
        //}
    } else {
        if (jobServiced != nullptr) {
            EV << "job arrived while already serving one, dropping it\n";
            delete msg;
            return;
        }

        if (!allocated) {
            error("job arrived, but the sender did not call allocate() previously");
            return;
        }

        jobServiced = check_and_cast<Job *>(msg);
        simtime_t serviceTime;

        int numServers = getParentModule()->par("numServers").intValue();
        if (jobServiced->getKind() == 1) { //Elastic Job ServiceTime
           serviceTime = par("serviceTimeEl").doubleValue() / numServers;
        }else{
           serviceTime = par("serviceTimeIn");
        }

        scheduleAt(simTime() + serviceTime, endServiceMsg);
        emit(busySignal, true);


    }
}


void Server::refreshDisplay() const
{
    getDisplayString().setTagArg("i2", 0, jobServiced ? "status/execute" : "");
}

void Server::finish()
{

}

bool Server::isIdle()
{
    return !allocated;  // we are idle if nobody has allocated us for processing
}

void Server::allocate()
{
    allocated = true;
}

void Server::sendJob(Job *job, int gateIndex){
    cGate *out = gate("out",gateIndex);
    send(job,out);
    check_and_cast<IServer *>(out->getPathEndGate()->getOwnerModule())->allocate();

}

void Server::sendJobEl(Job* job){
    cGate *out = gate("out", 1);
    send(job, out);
    //check_and_cast<IServer *>(out->getPathEndGate()->getOwnerModule())->allocate();
}

void Server::sendJobIn(Job* job){ // aggiunto da me
    cGate *out = gate("out", 0);
    send(job, out);
    //check_and_cast<IServer *>(out->getPathEndGate()->getOwnerModule())->allocate();
}


}; //namespace

