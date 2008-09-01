#ifndef __NETELEMENTS
#define __NETELEMENTS
#include <netdb.h>
#include <QNetworkInterface>
#include <QtXml/QDomElement>
#include <vector>
class NetElements
{

public:
	struct NetRecord
	{
		NetRecord(unsigned int ip,int services);
		NetRecord(const QString &host,int services);
		typedef enum 
			{
				NR_logger=1,
				NR_glomem=2,
				NR_reader=4,
				NR_hbook=8,
			} Service;
		int m_services;
		bool needLogger() const; 
		bool needGlomem() const;
		bool needReader() const; 
		bool needHbook() const; 
		unsigned int m_ipAddress;
		void host2ip(const char *name, unsigned int &ip) const; //exit on fail
	};
	
	int m_myIndex; /// contains this hosts index in the conf vector
	int m_readerIndex;
	void static (*call_back_fatal_exit)(int);
	NetElements(void (*fexit)(int));
	std::vector<NetRecord> conf;
	//
	// exit on error
	void parseNetwork(const QDomElement &elem);

};
#endif

