// $Id$

// Local include(s):
#include "Application.h"

namespace daq {

   Application::Application()
      : m_name( "" ), m_msgServers() {

   }

   void Application::clear() {

      m_name = "";
      m_msgServers.clear();
      return;

   }

   const QString& Application::getName() const {

      return m_name;

   }

   void Application::setName( const QString& name ) {

      m_name = name;
      return;

   }

   const std::vector< Address >& Application::getMsgServers() const {

      return m_msgServers;

   }

   void Application::addMsgServer( const Address& address ) {

      m_msgServers.push_back( address );
      return;

   }

} // namespace daq
