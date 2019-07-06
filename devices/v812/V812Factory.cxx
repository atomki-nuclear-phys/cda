
// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "V812Factory.h"
#include "Configurable.h"

namespace v812 {

   const QString& V812Factory::shortName() const {

      static const QString name( "V812" );
      return name;
   }

   const QString& V812Factory::longName() const {

      static const QString name( tr( "CAEN V812 CFD" ) );
      return name;
   }

   void* V812Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::ICaenVmeConfigurable ) ) {
         return static_cast< dev::ICaenVmeConfigurable* >( new Configurable() );
      }

      return nullptr;
   }

} // namespace v812

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( v812, v812::V812Factory )
#endif
