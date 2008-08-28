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
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
//
// Contains network element
//
class NetElements
{

public:
	struct NetRecord
	{
		NetRecord(unsigned int ip,int services)
		{
			m_services=services;
			m_ipAddress=ip;
		}
		NetRecord(const QString &host,int services)
		{			
			m_services=services;
			host2ip(host.toAscii(),m_ipAddress);

		}
		typedef enum 
			{
				NR_logger=1,
				NR_glomem=2,
				NR_reader=4,
				NR_hbook=8,
			} Service;
		int m_services;
		inline bool needLogger() const {return m_services&NR_logger;	}
		inline bool needGlomem() const {return m_services&NR_glomem;	}
		inline bool needReader() const {return m_services&NR_reader;	}
		inline bool needHbook()	const {return m_services&NR_hbook;	}
		unsigned int m_ipAddress;
		void host2ip(const char *name, unsigned int &ip) const //exit on fail
		{
			static const int buflen=4096; //must be enought, if not, we will not, do anything to increase
			char *buf=new char[buflen];
			struct hostent ret,*result;
			int lh_errno;

			// blocks, ipv4 only
			if (gethostbyname2_r(name, AF_INET,
						&ret, buf, buflen,
						&result, &lh_errno) || result==NULL)
			{//unlikely
				const char* emsg;
				switch (lh_errno)
				{
					case HOST_NOT_FOUND:
						emsg="The specified host is unknown.";
						break;						
					case NO_ADDRESS:
#if NO_ADDRESS!=NO_DATA 					
					case NO_DATA:
#endif					
						emsg="The requested name is valid but does not have an IP address.";
						break;
					case NO_RECOVERY:
						emsg="A non-recoverable name server error occurred";
						break;
					case TRY_AGAIN:
						emsg="A temporary error occurred on an authoritative name server.  Try again later.";
						break;
					default:
						emsg="Unknown error occured";
				};
				qWarning("Failed to resolve \'%s\', error string:%s  exiting..\n",name,emsg);
				shutDown(42);
			} else
			{
				//assuming only one IPv4 address returned, else just the first one will be processed
				//sanity check
				if (ret.h_addrtype!=AF_INET||ret.h_length!=4||ret.h_addr_list[0]==NULL)
				{ //unlikely
					qWarning("Unexpected gethostbyname2_r results %d,%d,%p  FATAL\n",ret.h_addrtype,ret.h_length);
					shutDown(42);					
				}
				ip=*((int*)ret.h_addr_list[0]);
			}					
			delete buf;
		}
	};
	
	int m_myIndex; /// contains this hosts index in the conf vector
	int m_readerIndex;
	NetElements()
	{
		m_myIndex=-1;
		m_readerIndex=-1;
	}
	std::vector<NetRecord> conf;
	//
	//returns true or exit
	bool parseNetwork(const QDomElement &elem)
	{
		QDomNodeList nl= elem.childNodes ();
		QDomElement work;
		bool hasNonLoopBack=false;
		QList<QHostAddress> addr=QNetworkInterface::allAddresses ();
		int id=0;
		for (int i=nl.count()-1;i>=0;i--)
		{
			int serv=0;
			work=nl.item(i).toElement();
			if (!work.isElement())
			{
				qWarning("Network node, has a non element child\n");
				continue;
			}
				
				if (work.attribute("logger","0")!="0") 
					serv|=NetRecord::NR_logger;
				if (work.attribute("reader","0")!="0")
				{
					serv|=NetRecord::NR_reader;
					if (m_readerIndex!=-1)
					{ 	//unlikely
						qWarning("Only one reader allowed in config. It is FATAL\n");
						shutDown(42);
					}
					m_readerIndex=id;
					
				}
				if (work.attribute("hbook","0")!="0")
					serv|=NetRecord::NR_hbook;
				if (work.attribute("glomem","0")!="0")
					serv|=NetRecord::NR_glomem;
				conf.push_back(NetRecord(work.attribute("hname"),serv));
				
				NetRecord &nr=conf.back();
				for (QList<QHostAddress>::iterator ii=addr.begin();ii!=addr.end();++ii)
				{
					if (QAbstractSocket::IPv4Protocol==(*ii).protocol()) 
					{
						if (nr.m_ipAddress==htonl((*ii).toIPv4Address()))
						{
							if (m_myIndex==-1)
								m_myIndex=id;
							else
							 {
							 	qWarning("Describes 2 record this host in config is FATAL, only one allowed\n");
								shutDown(42);
							 }
						}
					}
				}
				if (((char)nr.m_ipAddress)!=127) //big endian ip addr, first char is the first digit
				{
					hasNonLoopBack=true;
				} else
				{
					if (hasNonLoopBack)
					{
						qWarning("Mixing loopback addresses with network addreses in config is FATAL\n");
						shutDown(42);						
					}
				}
				// is me ?

			id++;
		}
		if (m_readerIndex==-1)
		{ 	//unlikely
			qWarning("No reader Host specifyed, we cant do anything without reader. FATAL \n");
			shutDown(42);
		}
		if (m_myIndex==-1)
		{
			qWarning("This host isn't in the config file, it dont`t have to do anything FATAL \n");
			shutDown(42);
		}

		return true;
	
	}

} g_NetElements;
//Process
class Process
{
	private:
	typedef std::map<pid_t,Process*> PidMap;
	static PidMap s_pidMap;
	static const char * str_verbosity;
	static const char * str_pipefd;
	static const char * str_config;
	static const char * str_output;
	typedef std::list<void*> pList ;
	pList  m_deleteOnDestroyList;
	typedef std::priority_queue< int > FDList;
	FDList m_doNotClose; // will be empty after execute (fork)
	volatile bool m_running;
	const char *m_file;
	char * const *m_argv;
	int m_argc;
	int m_verbosity; //change to static ? all process uses the same verbosity
	const char *m_config; //change to static ? all process uses the same config
	const char *m_output; //h_book only , we should create an inherited class, and remove it from base
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
				 {
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
				 }
				 
			default:	//parent
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
		m_config=NULL;
		m_output=NULL;
		m_verbosity=3;
		m_pid=-1;
		m_pipefd=-1;
		addKeepOpenfd(fileno(stderr)); //2
		addKeepOpenfd(fileno(stdout)); //1
	}
	~Process()
	{
		for (pList::iterator i=m_deleteOnDestroyList.begin();i!=m_deleteOnDestroyList.end();++i)
			delete (char*)*i;

		if (m_pid!=-1)
		{
			sigset_t allSignal;
			sigset_t saveSignal;
			sigfillset(&allSignal);
			sigprocmask(SIG_SETMASK,&allSignal,&saveSignal);
			PidMap::iterator i=s_pidMap.find(m_pid);
			if (i!=s_pidMap.end())
			{
				s_pidMap.erase(i);
			}
			sigprocmask(SIG_SETMASK,&saveSignal,NULL);
		}

	}

	inline void addKeepOpenfd(int i)
	{
		m_doNotClose.push(i);
	}
	inline void setVerbosity(int verbosity) { m_verbosity=verbosity; }
	inline void setConfigFile(const char *config ){ m_config=config; }  //not a copy
	inline void setOutputFile(const char *output ){ m_output=output; }  //not a copy
	inline void setPipefd(int pipefd) {m_pipefd=pipefd; addKeepOpenfd(pipefd);}
	int cdaProcStart(const char *file)
	{
		char const ** argv;
		int argc;
		if (!file)
		{ //unlikely
			qWarning("Config file name is invalid (NULL)\n");
			shutDown(42);
		}
		m_file=file;
//
//	construct args
//
		int needArg=4; // command,verbosity,verb_arg,NULL
		if (m_config)	needArg+=2;
		if (m_output)	needArg+=2;
		if (m_pipefd!=-1)	needArg+=2;
		
		argv=new const char* [needArg];
		argv[needArg-1]=NULL;
		m_deleteOnDestroyList.push_front(argv);		
		argv[0]=file;
		argv[1]=str_verbosity;
		char *str_verbosity_value = new char[16];
		m_deleteOnDestroyList.push_front(str_verbosity_value);
		sprintf(str_verbosity_value,"%d",m_verbosity);
		argv[2]=str_verbosity_value;
		argc=3;
		if (m_config)
		{
			argv[argc++]=str_config;
			argv[argc++]=m_config;
		}
		if (m_output)
		{
			argv[argc++]=str_output;
			argv[argc++]=m_output;
		}
		if (m_pipefd!=-1)
		{			
			argv[argc++]=str_pipefd;
			char *str_pipefd_value = new char[16];
			m_deleteOnDestroyList.push_front(str_pipefd_value);
			sprintf(str_pipefd_value,"%d",m_pipefd);		
			argv[argc]=str_pipefd_value; //  ++ missing
		}
		m_argv=(char* const*)argv;
		start();
	}
	int start(const char *file, char * const *argv)
	{
		m_argv=argv;
		m_file=file;
		start();
	}
	
	inline int sigSend(int sig)
	{
		if (m_running)
		{
			return kill(m_pid,sig);
		}
		return 1;
		
	}
	static inline  void sigSendAll(int sig)
	{
		for (PidMap::iterator i=s_pidMap.begin();i!=s_pidMap.end();i++)
		{
			(*i).second->sigSend(sig);
		}
	}
	inline int sigINT() const
	{		
		if (m_running)
		{
			return kill(m_pid,SIGINT);
		}
		return 1;
	}
	static inline void sigINTAll()
	{
		sigSendAll(SIGINT);
	}

	inline int sigKILL() const
	{
		if (m_running)
		{
			return kill(m_pid,SIGKILL);
		}
		return 1;
	}

	static inline void sigKILLAll() 
	{

		sigSendAll(SIGKILL);
	}
};

Process::PidMap Process::s_pidMap;

const char * Process::str_verbosity="--verbosity";
const char * Process::str_pipefd="--fd";
const char * Process::str_config="--config";
const char * Process::str_output="--output";

static const char* description =
   "Program starting all necesery application on this host.\n";

void pipeFail() //inline not needed
{
	qWarning("Failed to create pipes, reason:%s\n",strerror(errno));
	shutDown(42);
}
int g_verbosity=3;

int main( int argc, char* argv[] ) 
{

//
// Read the command line options:
//
	CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
	CmdArgStr config( 'c', "config", "filename", "Name of an XML config file",
			CmdArg::isREQ );
	CmdArgStr output( 'o', "output", "filename", "Name of HBOOK file");

	CmdLine cmd( *argv, &verbosity, &config, &output, NULL );
	cmd.description( description );

	CmdArgvIter arg_iter( --argc, ++argv );
	verbosity = 3;
	cmd.parse( arg_iter );

	g_verbosity=verbosity;
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
// Connect the interrupt signal to the shutDown and sigChildAction function:
//
	signal( SIGINT, shutDown ); //deprecated
	signal( SIGTERM, shutDown ); //deprecated
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
//
//	Read Network config
//
	QDomElement work,root=doc.documentElement();
	QDomNodeList	nl=root.elementsByTagName("Network");
	work=nl.item(0).toElement();
	if (nl.count()!=1)
	{
		qWarning("Config file must contins exactly one Network element node.\n");
		shutDown(42);
	}
	if (work.isElement())
	{
		g_NetElements.parseNetwork(work);
	} else
	{
		qWarning("not elemnt Network node \n");
		shutDown(42);
	}
//
//	Starting necessery processies
//
	
//
//	logger
//
	int numProc=0;
	if (g_NetElements.conf[g_NetElements.m_myIndex].needLogger())
	{
		Process *proc = new Process();
		proc->setVerbosity(verbosity);
		proc->setConfigFile(config);
		proc->cdaProcStart("cda-msgserver");
		numProc++;
	}
//
// Initcilize pipes if necessery
//
// When the reader, glomem-writer^hbook-writer is on the same host  
// They should use pipe(s), it is faster than tcp.
// Now when both in same, host reader have to use only one pipe, and the init will distribute the data via pipes 
// if the one hbook-write glomme-writer thi init will connect to the reader, and distriute
// the data to writers
// when one writer running on disferent host than reader's, host use TCP
//

int readerfd=-1;
int glomemfd=-1;
int hbookfd=-1;
int targets[2]={-1,-1};
int source=-1;
int pfd[2];
if	(g_NetElements.conf[g_NetElements.m_myIndex].needReader())
{
	// we have reader
	if (g_NetElements.conf[g_NetElements.m_myIndex].needHbook())
	{ //reader + hbook + ?
		if (g_NetElements.conf[g_NetElements.m_myIndex].needGlomem())
		{ //reader + hbook + glomem
			if (pipe(pfd)) pipeFail();
			source=pfd[0];
			readerfd=pfd[1];
			if (pipe(pfd)) pipeFail();
			targets[0]=pfd[1];
			hbookfd=pfd[0];
			if (pipe(pfd)) pipeFail();
			targets[1]=pfd[1];
			glomemfd=pfd[0];

			Process *proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->setOutputFile(output);
			proc->setPipefd(hbookfd);
			proc->cdaProcStart("cda-hbook-writer");
			numProc++;

			proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->setPipefd(glomemfd);
			proc->cdaProcStart("cda-glomem-writer");
			numProc++;


		} else
		{ //reader + hbook
			if (pipe(pfd)) pipeFail();
			hbookfd=pfd[0];
			readerfd=pfd[1];
			Process *proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->setOutputFile(output);
			proc->setPipefd(hbookfd);
			proc->cdaProcStart("cda-hbook-writer");
			numProc++;
		
		}

	}else
		if (g_NetElements.conf[g_NetElements.m_myIndex].needGlomem())
		{  //reader+glomem
			if (pipe(pfd)) pipeFail();
			glomemfd=pfd[0];
			readerfd=pfd[1];
			Process *proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->setPipefd(glomemfd);
			proc->cdaProcStart("cda-glomem-writer");
			numProc++;
		}
//
//	reader
//
	if (g_NetElements.conf[g_NetElements.m_myIndex].needReader())
	{
		Process *proc = new Process();
		proc->setVerbosity(verbosity);
		proc->setConfigFile(config);
		if (readerfd!=-1)
		proc->setPipefd(readerfd);
		proc->cdaProcStart("cda-camac-reader");
		numProc++;
	}
} else
{
	if (g_NetElements.conf[g_NetElements.m_myIndex].needHbook()&&g_NetElements.conf[g_NetElements.m_myIndex].needGlomem())
	{ //no reader hbook+glomem
			if (pipe(pfd)) pipeFail();
			targets[0]=pfd[1];
			glomemfd=pfd[0];
			if (pipe(pfd)) pipeFail();
			targets[1]=pfd[1];
			hbookfd=pfd[0];

/*		socket();
 *		listen here
 */
			Process *proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->setOutputFile(output);
			proc->setPipefd(hbookfd);
			proc->cdaProcStart("cda-hbook-writer");
			numProc++;

			proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->setPipefd(glomemfd);
			proc->cdaProcStart("cda-glomem-writer");
			numProc++;

	}
	else
	{ // no reader hbook^glomem, normal TCP
//
//	hbook
//

		if (g_NetElements.conf[g_NetElements.m_myIndex].needHbook())
		{
			Process *proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->setOutputFile(output);
			proc->cdaProcStart("cda-hbook-writer");
			numProc++;
		}
//
//	glomem
//
		if (g_NetElements.conf[g_NetElements.m_myIndex].needGlomem())
		{
			Process *proc = new Process();
			proc->setVerbosity(verbosity);
			proc->setConfigFile(config);
			proc->cdaProcStart("cda-glomem-writer");
			numProc++;
		}	
	}
}


/*
 * Place here the distrubuters code
 *
 *
 *
 */



	while (numProc) 
	{
		int kuka;
		wait(&kuka);
		numProc--;
	}
	return 0;

}

/**
 * This function makes kills the running children applications,
 * when the application terminates.
 */
void shutDown( int ) {


	qWarning("Terminating application...\n");
	Process::sigINTAll(); //logger will survive
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
		
           
	return ret;	
}
void     sigChildAction(int i, siginfo_t *info, void *data)
{
	if (g_verbosity<=0)
		qWarning()<<siginfo_t2Qstring(*info); //debug
	Process &proc=*Process::s_pidMap[info->si_pid];
	proc.m_running=false;

	if (WIFSIGNALED(info->si_status))
	{
		qWarning("%s terminated by signal(%d) \n",proc.m_file,WTERMSIG(info->si_status));
#ifdef WCOREDUMP
		if (WCOREDUMP(info->si_status))
		{
			qWarning("Core Dumped \n",proc.m_file,WTERMSIG(info->si_status));
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
