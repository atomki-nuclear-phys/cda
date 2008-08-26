// $Id: main.cxx 40 2008-07-15 16:46:45Z krasznaa $
/**
 *   @file apps/cd-init/main.cxx
 *  @short Main file for the init application
 *
 * @author Attila Fazekas
 *
 * $Revision: 48 $
 * $Date: 2008-08-24 18:46:45 +0200 (Sun, 24 Aug 2008) $
 */

// System include(s):
#include <cstdlib>
#include <csignal>
#include <errno.h>
#include <signal.h>

// STL include(s):
#include <iostream>
#include <map>
#include <queue>
// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QNetworkInterface>
#include <QString>
#include <QtDebug> //qWarning <<
// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/cmdl/cmdargs.h"
#   include "cdacore/device/Loader.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/EventServer.h"
#else
#   include "cmdl/cmdargs.h"
#   include "device/Loader.h"
#   include "event/Event.h"
#   include "event/EventServer.h"
#endif

// Local include(s):


// Function forward declaration(s):
void shutDown( int );
void sigChildAction(int i, siginfo_t *info, void *data);

//Process
class Process
{
	private:
	static std::map<pid_t,Process*> s_pidMap;
	static const char * str_verbosity;
	static const char * str_pipefd;
	static const char * str_config;
	static const char * str_output;
	typedef std::list<void*> pList ;
	pList  m_deleteOnDestroyList;
	typedef std::priority_queue< int > FDList;
	FDList m_doNotClose; // will be empty after execute (fork)
	bool m_running;
	const char *m_file;
	char * const *m_argv;
	int m_argc;
	int m_verbosity; //change to static ? all process uses the same verbosity
	const char *m_config; //change to static ? all process uses the same config
	int m_pipefd; // must be uniq
	pid_t m_pid;
	friend void sigChildAction(int i, siginfo_t *info, void *data);
	int start()
	{
		sigset_t allSignal;
		sigset_t saveSignal;
		sigfillset(&allSignal);
		sigprocmask(SIG_SETMASK,&allSignal,&saveSignal);
		m_pid=fork();
		switch (m_pid)
		{
			case -1: //error
				 qWarning("Failed to start: %s ,fork problem\n",m_file);
				 break;
			case 0: //child
//
//	Closes unnecesry fd -s
//
				int i=getdtablesize()-1;

				while (!m_doNotClose.empty() && i>0)
				{
					if (m_doNotClose.top()==i)
						m_doNotClose.pop();
					else
						close(i);
					i--;
				} 

				while (i>=0)			
				{
					close(i--);
				}

				 if (-1==execvp(m_file,m_argv))
				{
					qWarning("Error occured while executing : %s  errno:%d :%s",m_file, errno, strerror(errno));
				}
				 exit(42);
				 
			default://parent
			 m_running=true;
			 s_pidMap[m_pid]=this;
		}		
		sigprocmask(SIG_SETMASK,&saveSignal,NULL);
	}
	public:
	Process()
	{
		m_running=false;
		m_argv=NULL;
		m_file=NULL;
		m_pid=-1;
		m_doNotClose.push(2); //fileno stderr
	}
	~Process()
	{
		for (pList::iterator i=m_deleteOnDestroyList.begin();i!=m_deleteOnDestroyList.end();i++)
			delete (char*)*i;		
	}

	int addKeepopenfd(int i)
	{
		m_doNotClose.push(i);
	}
	void setVerbosity(int verbosity) { m_verbosity=verbosity; }
	void setConfigFile(char *config ){ m_config=config; }  //not copy
	void setPipefd(int pipefd) {m_pipefd=pipefd;}
	int cdaProcStart(const char *file)
	{
		
	}
	int start(const char *file, char * const *argv)
	{
		m_argv=argv;
		m_file=file;
		start();
	}

	int sigINT()
	{		
		if (m_running)
		{
			return kill(m_pid,SIGINT);
		}
		return 1;
	}
	int sigKILL()
	{
		if (m_running)
		{
			return kill(m_pid,SIGKILL);
		}
		return 1;
	}
};

std::map<pid_t,Process*> Process::s_pidMap;

const char * Process::str_verbosity="--verbosity";
const char * Process::str_pipefd="--pipefd";
const char * Process::str_config="--config";
const char * Process::str_output="--output";

static const char* description =
   "Program starting all necesery application on this host.\n";

int main( int argc, char* argv[] ) 
{

	//
	// Read the command line options:
	//
	CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
	CmdArgStr config( 'c', "config", "filename", "Name of an XML config file",
			CmdArg::isREQ );
	CmdArgStr output( 'o', "output", "filename", "Name of HBOOK file",
			CmdArg::isREQ );

	CmdLine cmd( *argv, &verbosity, &config, &output, NULL );
	cmd.description( description );

	CmdArgvIter arg_iter( --argc, ++argv );
	verbosity = 3;
	cmd.parse( arg_iter );

	//
	// Translate the verbosity option:
	//

	//
	// Open the configuration file:
	//
	QFile config_file( ( const char* ) config );
	if( ! config_file.open( QFile::ReadOnly | QFile::Text ) ) {
		qWarning("The specified configuration file (\"%s\""
				" could not be opened!\n",
				( ( const char* ) config ? ( const char* ) config : "" ));
		return 1;
	}

	//
	// Read the file's contents into XML format:
	//
	QDomDocument doc;
	QString errorMsg;
	int errorLine, errorColumn;
	if( ! doc.setContent( &config_file, false, &errorMsg, &errorLine,
				&errorColumn ) ) {
		qWarning( "Error in parsing \"%s\"\n"
				"  Error message: %s\n" 
				"  Error line   : %d\n" 
				"  Error column : %d\n" 
				,( const char* ) config,errorMsg.data() ,errorLine,errorColumn);
		return 1;
	} else {
		qWarning( "Successfully parsed: %s",( const char* ) config);
	}

	//
	// Connect the interrupt signal to the shutDown function:
	//
	signal( SIGINT, shutDown ); //deprecated
	struct sigaction sigChildStruct;
	sigChildStruct.sa_handler=NULL;
	sigChildStruct.sa_sigaction=sigChildAction;
	sigset_t sigChildMask; //uint
	sigfillset(&sigChildMask);
	// block all signel while handling
	sigChildStruct.sa_mask=sigChildMask;
	/* Restatart syscalls
	 * use 3 arg
	 * Only at exit
	 */
	sigChildStruct.sa_flags=SA_RESTART|SA_SIGINFO|SA_NOCLDSTOP;
	sigChildStruct.sa_restorer=NULL; //deprecated

	//
	// Set handler, without saving the old one
	sigaction( SIGCHLD,&sigChildStruct,NULL);
//	Process ls;
//	ls.m_argv= (char* const []) {NULL,"/sehol",NULL};
//	ls.m_file="ls";
//	ls.start();
	sleep(10);
	return 0;

}

/**
 * This function makes kills the running children applications,
 * when the application terminates.
 */
void shutDown( int ) {


	qWarning("Terminating application...\n");
	exit( 0 );

	return;

}
QString siginfo_t2Qstring(const siginfo_t& info)
{
	//slow
	 QString ret;
		ret=QString("Signal number (si_signo): %1 \n").arg(info.si_signo);
		ret+=QString("An errno value (si_errno): %1 \n").arg(info.si_errno);
		ret+=QString("Signal code (si_code): %1 \n").arg(info.si_code);
		ret+=QString("Sending process ID (si_pid): %1 \n").arg(info.si_pid);
		ret+=QString("Real user ID of sending process (si_uid): %1 \n").arg(info.si_uid);
		ret+=QString("Exit value or signal (si_status): %1 \n").arg(info.si_status);
		ret+=QString("User time consumed (si_utime): %1 \n").arg(info.si_utime);
		ret+=QString("System time consumed (si_stime): %1 \n").arg(info.si_stime);
		ret+=QString("Signal value (si_value): %1 \n").arg((info.si_value.sival_int));
		ret+=QString("POSIX.1b signal (si_int): %1 \n").arg(info.si_int);
		ret+=QString("POSIX.1b signal (si_ptr): 0x%1 \n").arg((long)info.si_ptr,0,16);
		ret+=QString("Memory location which caused fault (si_addr): 0x%1 \n").arg((long)info.si_addr,0,16);
		ret+=QString("Band event (si_band): %1 \n").arg(info.si_band);
		ret+=QString("File descriptor (si_fd): %1 \n").arg(info.si_fd);
		
//               int      si_code;     /* Signal code */
//               pid_t    si_pid;      /* Sending process ID */
//               uid_t    si_uid;      /* Real user ID of sending process */
//               int      si_status;   /* Exit value or signal */
//               clock_t  si_utime;    /* User time consumed */
//               clock_t  si_stime;    /* System time consumed */
//               sigval_t si_value;    /* Signal value */
//               int      si_int;      /* POSIX.1b signal */
//               void    *si_ptr;      /* POSIX.1b signal */
//               void    *si_addr;     /* Memory location which caused fault */
//               int      si_band;     /* Band event */
//               int      si_fd;       /* File descriptor */
           
	return ret;	
}
void     sigChildAction(int i, siginfo_t *info, void *data)
{
	qWarning()<<siginfo_t2Qstring(*info);
	Process &proc=*Process::s_pidMap[info->si_pid];
	proc.m_running=false;

	if (WIFSIGNALED(info->si_status))
	{
		qWarning("%s terminated by signal(%d) \n",proc.m_file,WTERMSIG(info->si_status));
#ifdef WCOREDUMP
		if (WCOREDUMP(info->si_status))
		{
			qWarning("Core Dumped \n",proc->m_file,WTERMSIG(info->si_status));
		}
#endif
	} else
	if (WIFEXITED(info->si_status))
	{
		qWarning("%s exited \"normali\" status:%d \n",proc.m_file,WEXITSTATUS(info->si_status));
	} else
	{
		
		proc.m_running=true; 
		qWarning("%s Unexpected status received, pid: %d\n",proc.m_file,info->si_pid);
		if (WIFSTOPPED(info->si_status))
		{
			qWarning("Stopped\n"); 
		}else
		if (WIFCONTINUED(info->si_status))
		{
			qWarning("Continue\n");
		}else
		qWarning("Unknown status, pid: %d  name:%s status %d\n",info->si_pid,proc.m_file,info->si_status);
	}
	

}
