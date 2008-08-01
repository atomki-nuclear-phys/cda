#include <QPushButton>
#include <QTableWidget>
#include <QDomElement>
// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif
namespace net
{
	class   NetworkEditor : public  QWidget
	{
		Q_OBJECT
		public:
			void addRow(QString hostname="", bool logger=false,bool reader=false, bool hbook=false,bool glomem=false);
			mutable msg::Logger m_logger;
			NetworkEditor( QWidget* parent, Qt::WindowFlags flags =0);
			bool readConfig (QDomElement &elem);
			bool writeConfig (QDomElement &elem);
			bool readConfig (QIODevice* dev);
			bool writeConfig (QIODevice* dev);
		public slots:
			void addClickedSlot(bool checked);
			void delClickedSlot(bool checked);

		protected:
			QPushButton *m_AddButton;
			QPushButton *m_DelButton;
			QTableWidget *m_hosts;
	};
}
