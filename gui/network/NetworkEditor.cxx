#include "NetworkEditor.h"
#include <QTableWidgetItem>
namespace net
{

	NetworkEditor::NetworkEditor( QWidget* parent, Qt::WindowFlags flags )
	: QWidget( parent, flags ), m_logger( "net::NetworkEditor" )
      {
      		 m_AddButton= new QPushButton(tr("Add"),this);
		 connect(m_AddButton,SIGNAL(clicked(bool)),this,SLOT(addClickedSlot(bool)));
      		 m_DelButton= new QPushButton(tr("Del"),this);
		 m_DelButton->setGeometry(QRect(200,0,100,25));
		 connect(m_DelButton,SIGNAL(clicked(bool)),this,SLOT(delClickedSlot(bool)));
      		 m_hosts= new QTableWidget(this);
		 m_hosts->setGeometry(QRect(0,25,600,400));
		 m_hosts->setColumnCount(5);
		 m_hosts->setHorizontalHeaderItem(0,  new QTableWidgetItem(tr("Hostname")));
		 m_hosts->setColumnWidth(0,250);
		 m_hosts->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("log")));
		 m_hosts->setColumnWidth(1,60);
		 m_hosts->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("reader")));
		 m_hosts->setColumnWidth(2,60);
		 m_hosts->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("hbook")));
		 m_hosts->setColumnWidth(3,60);
		 m_hosts->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("glomem")));
		 m_hosts->setColumnWidth(4,60);
		 m_hosts->setSelectionBehavior(QAbstractItemView::SelectRows);
	
      }
	void NetworkEditor::addClickedSlot(bool)
	{
		addRow();
	}
	void NetworkEditor::delClickedSlot(bool)
	{
		int row=m_hosts->currentRow();
		if (row>=0)
			m_hosts->removeRow(m_hosts->currentRow());
	}
	void NetworkEditor::addRow(QString hostname, bool logger,bool reader, bool hbook,bool glomem)
	{
		int row;
		QTableWidgetItem *workItem;
		row=m_hosts->rowCount();
		m_hosts->setRowCount(row+1);
		workItem=new QTableWidgetItem(hostname);
		m_hosts->setItem(row,0, workItem );

		workItem=new QTableWidgetItem();
		workItem->setData(Qt::EditRole,logger);
		m_hosts->setItem(row,1, workItem );

		workItem=new QTableWidgetItem();
		workItem->setData(Qt::EditRole,reader);
		m_hosts->setItem(row,2, workItem );

		workItem=new QTableWidgetItem();
		workItem->setData(Qt::EditRole,hbook);
		m_hosts->setItem(row,3, workItem );

		workItem=new QTableWidgetItem(glomem);
		workItem->setData(Qt::EditRole,glomem);
		m_hosts->setItem(row,4, workItem );
	}
	bool NetworkEditor::writeConfig (QDomElement &elem) const
	{
		QDomDocument doc=elem.ownerDocument(); 
		QDomElement work;
		for (int i=m_hosts->rowCount()-1;i>=0;i--)
		{
			work=doc.createElement ( "Node" );
			work.setAttribute("hname",m_hosts->item(i,0)->data(Qt::EditRole).toString());
			work.setAttribute("logger",m_hosts->item(i,1)->data(Qt::EditRole).toBool());
			work.setAttribute("reader",m_hosts->item(i,2)->data(Qt::EditRole).toBool());
			work.setAttribute("hbook",m_hosts->item(i,3)->data(Qt::EditRole).toBool());
			work.setAttribute("glomem",m_hosts->item(i,4)->data(Qt::EditRole).toBool());
			elem.appendChild(work);
		}
		return true;
	}
	bool NetworkEditor::readConfig (const QDomElement &elem)
	{
		m_hosts->setRowCount(0)	; //clear,no rollback
		QDomNodeList nl= elem.childNodes ();
		QDomElement work;
		for (int i=nl.count()-1;i>=0;i--)
		{
			work=nl.item(i).toElement();
			if (!work.isElement())
			{
				//add log msg, if you want
				continue;
			}
			// add tag name check, if you want
			addRow(
				work.attribute("hname"),
				work.attribute("logger","0")!="0",
				work.attribute("reader","0")!="0",
				work.attribute("hbook","0")!="0",
				work.attribute("glomem","0")!="0"
			);
		}
		return true;
	}
	bool NetworkEditor::writeConfig (QIODevice *dev) const
	{

		return false;
	}
	bool NetworkEditor::readConfig (QIODevice *dev)
	{

		return false;
	}
	void NetworkEditor::clear()
	{
		m_hosts->setRowCount(0)	; 
		addRow("localhost",true,true,true,true);
	}

}

