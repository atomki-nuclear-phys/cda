// Dear emacs, this is -*- c++ -*-
/**
 *   @file core/common/QStringCompat.h
 *  @short Compatibility definitions for using QtString in different Qt versions
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
#ifndef CDA_CORE_COMMON_QTSTRINGCOMPAT_H
#define CDA_CORE_COMMON_QTSTRINGCOMPAT_H

// Qt include(s).
#include <QString>

#ifndef CDA_HAVE_QT_SPLITBEHAVIOR
namespace Qt {
typedef QString::SplitBehavior SplitBehavior;
static const SplitBehavior SkipEmptyParts = QString::SkipEmptyParts;
static const SplitBehavior KeepEmptyParts = QString::KeepEmptyParts;
}  // namespace Qt
#endif  // CDA_HAVE_QT_SPLITBEHAVIOR

#endif // CDA_CORE_COMMON_QTSTRINGCOMPAT_H
