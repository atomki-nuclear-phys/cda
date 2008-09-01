#include "net/NetElements.h"

NetElements::NetRecord::NetRecord(unsigned int ip,int services)
{
	m_services=services;
	m_ipAddress=ip;
}

NetElements::NetRecord::NetRecord(const QString &host,int services)
{			
	m_services=services;
	host2ip(host.toAscii(),m_ipAddress);

}
bool NetElements::NetRecord::needLogger() const {return m_services&NR_logger;	}
bool NetElements::NetRecord::needGlomem() const {return m_services&NR_glomem;	}
bool NetElements::NetRecord::needReader() const {return m_services&NR_reader;	}
bool NetElements::NetRecord::needHbook()	const {return m_services&NR_hbook;	}

void NetElements::NetRecord::host2ip(const char *name, unsigned int &ip) const //exit on fail
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
		call_back_fatal_exit(42);
	} else
	{
		//assuming only one IPv4 address returned, else just the first one will be processed
		//sanity check
		if (ret.h_addrtype!=AF_INET||ret.h_length!=4||ret.h_addr_list[0]==NULL)
		{ //unlikely
			qWarning("Unexpected gethostbyname2_r results %d,%d  FATAL\n",ret.h_addrtype,ret.h_length);
			call_back_fatal_exit(42);					
		}
		ip=*((int*)ret.h_addr_list[0]);
	}					
	delete buf;
}

void  (*NetElements::call_back_fatal_exit)(int);
NetElements::NetElements(void (*fexit)(int))
{
	call_back_fatal_exit=fexit; //warning this is static member
	m_myIndex=-1;
	m_readerIndex=-1;
}
//
// exit on error
void NetElements::parseNetwork(const QDomElement &elem)
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
				call_back_fatal_exit(42);
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
						call_back_fatal_exit(42);
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
				call_back_fatal_exit(42);						
			}
		}
		// is me ?

		id++;
	}
	if (m_readerIndex==-1)
	{ 	//unlikely
		qWarning("No reader Host specifyed, we cant do anything without reader. FATAL \n");
		call_back_fatal_exit(42);
	}
	if (m_myIndex==-1)
	{
		qWarning("This host isn't in the config file, it dont`t have to do anything FATAL \n");
		call_back_fatal_exit(42);
	}

}



