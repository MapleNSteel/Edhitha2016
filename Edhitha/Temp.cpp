/*
 * Temp.cpp
 *
 *  Created on: 21 Mar 2016
 *      Author: arjun
 */
/*
#include "Backend/Engine/Inotifier/Watch.h"
#include <csignal>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/atomic.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <tbb/concurrent_queue.h>

using namespace std;
tbb::atomic<bool> stop (false);
tbb::atomic<bool> running (false);

tbb::concurrent_queue< boost::filesystem::path > fileQueue;

const char* root= "/home/arjun/Workspace/OpenCV/Images/2015 Aerial Images/Originals/Outputs/Targets";

void signalHandler( int signum )
{
    cout << "Interrupt signal (" << signum << ") received.\n";
    // cleanup and close up stuff here
    // terminate program
    fstream dummy;
    stop = true;
    string directory(root);
    remove((directory+"/"+"dummy").c_str());
    dummy.open((directory+"/"+"dummy").c_str(),ios::out);
    dummy<<flush;

    dummy.close();
}

void consumer(tbb::concurrent_queue< boost::filesystem::path > fileQueue){
	while(!fileQueue.empty()){
		boost::filesystem::path filePath;
		fileQueue.try_pop(filePath);
		cout<<filePath.c_str()<<endl;
	}
	running = false;
}

void producer(){
	fileMonitor(root,stop,running,consumer,fileQueue);
}
int main(int argc , char* argv[]){
	signal(SIGINT, signalHandler);

	boost::thread producerThread(producer);

	producerThread.join();

	cout<<"Exiting main\nMay the OCR be with you.\n";

	return 0;
}*/
