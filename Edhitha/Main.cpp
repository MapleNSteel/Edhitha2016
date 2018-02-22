/*


 * Main.cpp
 *
 *  Created on: 7 Mar 2016
 *      Author: arjun
 */
#include <csignal>
#include <atomic>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/atomic.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <tbb/concurrent_queue.h>

#include "Backend/Baelando.h"
#include "Backend/Engine/Inotifier/Watch.h"

using namespace std;
using namespace Edhitha2016;

tbb::atomic<bool> stop (false);
tbb::atomic<bool> running (false);

tbb::concurrent_queue< boost::filesystem::path > fileQueue;

Baelando* baelando;

fs::path rootDirectory;

void signalHandler( int signum )
{
	cout << "Interrupt signal (" << signum << ") received.\n";

	// cleanup and close up stuff here
	// terminate program
	fstream dummy;
	stop = true;

	remove((rootDirectory.string()+"/"+"dummy").c_str());
	dummy.open((rootDirectory.string()+"/"+"dummy").c_str(),ios::out);
	dummy<<flush;

	dummy.close();

}
void getTarget(){
	sleep(1);
	while(!fileQueue.empty()&&!stop){
		baelando->bae.setFrame();
		baelando->bae.anomalyDetect.detectTarget();

		queue<Target> target;
		target=baelando->bae.anomalyDetect.getTargets();
		baelando->writeTargetImages(target);
	}
	running = false;
}
int main(int args , char* argc[]){

	signal(SIGINT, signalHandler);

	vector<fs::path> files;
	rootDirectory = Baelando::getImageRootDirectory(argc[0]);
	cout<<"parent directory:\t"<<rootDirectory.c_str()<<endl;

	//Baelando::getAllImages(rootDirectory,&fileQueue);
	baelando=new Baelando(rootDirectory,&fileQueue,Size(4496,3000),CV_8UC3,false,0.00006,0.0012,50);

	fileMonitor(rootDirectory.parent_path().c_str(),stop,running,getTarget,&fileQueue);
	//Alphabet detail in file.
	destroyAllWindows();
	delete baelando;

	remove((rootDirectory.string()+"/"+"dummy").c_str());

	return 0;
}
