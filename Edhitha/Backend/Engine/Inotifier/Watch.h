#ifndef I_NOTIFICATION
#define I_NOTIFICATION

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <limits.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <tbb/concurrent_queue.h>

using std::map;
using std::string;
using std::cout;
using std::endl;

#define EVENT_SIZE          ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN       ( 1024 * ( EVENT_SIZE + NAME_MAX + 1) )
#define WATCH_FLAGS         ( IN_CREATE | IN_DELETE )

struct wd_elem {
       int pd;
       string name;
       bool operator() (const wd_elem &l, const wd_elem &r) const
           { return l.pd < r.pd ? true : l.pd == r.pd && l.name < r.name ? true : false; }
   };

class Watch
{
     public:
	 int insert( int pd, const string &name, int wd );
	 string erase( int pd, const string &name, int *wd );
	 string get( int wd );
	 int get(int pd, string name );
	 void cleanup( int fd );
	 void stats();
};
void sig_callback(int sig);
void fileMonitor(const char* root, tbb::atomic<bool>& stop, tbb::atomic<bool>& running, void(*getTarget)(void), tbb::concurrent_queue< boost::filesystem::path >* fileQueue);

#endif
