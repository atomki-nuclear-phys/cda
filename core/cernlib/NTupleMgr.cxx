
// System include(s):
#include <cstdio>

// CERNLIB include(s):
#ifdef HAVE_CERNLIB
extern "C" {
#include <cfortran/cfortran.h>
#include <cfortran/hbook.h>
}
#endif  // HAVE_CERNLIB

// Local include(s):
#include "NTupleMgr.h"

//
// Some CERNLIB definitions. Stolen from ROOT's THbookFile.cxx, and adjusted
// to work correctly on different flavours of Ubuntu.
//
#define PAWC_SIZE 4000000
#ifndef WIN32
#define pawc pawc_
#define quest quest_
#define hcbits hcbits_
#define hcbook hcbook_
#define rzcl rzcl_
int pawc[PAWC_SIZE] __attribute__((aligned(64)));
int quest[100] __attribute__((aligned(64)));
int hcbits[37] __attribute__((aligned(64)));
int hcbook[51] __attribute__((aligned(64)));
int rzcl[11] __attribute__((aligned(64)));
#else
#define pawc PAWC
#define quest QUEST
#define hcbits HCBITS
#define hcbook HCBOOK
#define rzcl RZCL
extern "C" int pawc[PAWC_SIZE];
extern "C" int quest[100];
extern "C" int hcbits[37];
extern "C" int hcbook[51];
extern "C" int rzcl[11];
#endif

namespace cernlib {

//
// Set the values of the constants:
//
const int NTupleMgr::HFILE_ID = 1;
const int NTupleMgr::NTUPLE_ID = 1;

/**
 * The constructor just sets all the member variables to reasonable
 * values. After creating the object, you have to add variables to the
 * ntuple and once done, call the NTupleMgr::openFile function.
 */
NTupleMgr::NTupleMgr()
    : m_fileOpen(false),
      m_varNames(),
      m_variables(0),
      m_events(0),
      m_logger("cernlib::NTupleMgr") {

   REPORT_VERBOSE(tr("Object constructed"));
}

/**
 * The destructor makes sure that the output file is properly closed
 * if it was left open.
 */
NTupleMgr::~NTupleMgr() {

   closeFile();
   REPORT_VERBOSE(tr("Object destructed"));
}

/**
 * One can use this function to declare new variables in the output ntuple.
 * Note that the names of the variables should not be longer than 8
 * characters. (CERNLIB restriction...)
 *
 * The code using this interface should save the value of the returned
 * index. That index can later be used to set the value of the variable
 * added here, by the NTupleMgr::setVar function.
 *
 * @param name Name of the ntuple variable (max. 8 characters)
 * @returns The index of the variable in the ntuple event buffer
 */
int NTupleMgr::addVar(const QString& name) {

// Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
   REPORT_VERBOSE(tr("Adding variable with name: %1").arg(name));
   return -1;
#endif  // HAVE_CERNLIB

   // Check that the file is not yet open:
   if (m_fileOpen) {
      REPORT_ERROR(
          tr("You can't add new variables when the output file\n"
             "is already open!"));
      return -1;
   }

   // Add this name to the variable list:
   int index = static_cast<int>(m_varNames.size());
   m_varNames.push_back(name);

   return index;
}

/**
 * This is the most involved function of this interface. It initializes
 * CERNLIB, opens an output file with the specified file name, and
 * creates an ntuple with the variables defined previously.
 *
 * @param fileName Name of the HBOOK file to create
 * @returns <code>true</code> if the operation was successful,
 *          <code>false</code> otherwise
 */
bool NTupleMgr::openFile(const QString& fileName) {

   // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
   REPORT_VERBOSE(tr("Opening output file: %1").arg(fileName));
   return false;
#endif  // HAVE_CERNLIB

   // Only one HBOOK file can be open at a time. (It's not a restriction
   // of CERNLIB, we just don't need to be able to open multiple output
   // files...)
   if (m_fileOpen) {
      m_logger << msg::WARNING << tr("The output file is already open")
               << msg::endmsg;
      return true;
   }

#ifdef HAVE_CERNLIB
   // Translate the variable names into a format that CERNLIB can
   // understand:
   char varNames[m_varNames.size()][10];
   unsigned int i = 0;
   for (std::vector<QString>::const_iterator name = m_varNames.begin();
        name != m_varNames.end(); ++name, ++i) {
      if (snprintf(varNames[i], sizeof(varNames[i]), "%s",
                   name->toLatin1().constData()) > (int)sizeof(varNames[i])) {
         m_logger << msg::WARNING
                  << tr("Variable name \"%1\" too long. It was shortened "
                        "to \"%2\"")
                         .arg(*name)
                         .arg(varNames[i])
                  << msg::endmsg;
      }
   }
#endif  // HAVE_CERNLIB

   // Prepare some other character variables for the CERNLIB
   // functions:
   char aq[15], data[10], n[2], hname[50];
   snprintf(aq, sizeof(aq), "acquisition");
   snprintf(data, sizeof(data), "Data");
   snprintf(n, sizeof(n), "N");
   if (snprintf(hname, sizeof(hname), "%s", fileName.toLatin1().constData()) >
       (int)sizeof(hname)) {
      REPORT_ERROR(tr("Output file name too long. File not opened!"));
      return false;
   }

#ifdef HAVE_CERNLIB
   // Create the HBOOK common block(s):
   HLIMIT(PAWC_SIZE);

   // Some additionally needed variables for the CERNLIB functions:
   int istat = 0, record_size = 1024;

   // Try to open the output file:
   HROPEN(HFILE_ID, aq, hname, n, record_size, istat);
   if (istat) {
      REPORT_ERROR(tr("There was a problem opening: %1").arg(hname));
      return false;
   } else {
      m_logger << msg::INFO << tr("\"%1\" opened").arg(hname) << msg::endmsg;
   }

   // Create the ntuple for the data:
   HBOOKN(NTUPLE_ID, data, m_varNames.size(), aq, 5000, varNames);
#endif  // HAVE_CERNLIB

   m_logger << msg::DEBUG << tr("NTuple created") << msg::endmsg;
   m_fileOpen = true;
   m_events = 0;

   // Create the buffer for the event variables:
   m_variables = new float[m_varNames.size()];
   for (unsigned int i = 0; i < m_varNames.size(); ++i) {
      m_variables[i] = 0.;
   }

   return true;
}

/**
 * If a HBOOK file is currently open, this function makes sure that all the
 * events are written out to the file, and then it closes the file.
 */
void NTupleMgr::closeFile() {

   // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
   return;
#endif  // HAVE_CERNLIB

   // Don't do anything if there is no output file open:
   if (!m_fileOpen) {
      m_logger << msg::DEBUG << tr("No output file is open") << msg::endmsg;
      return;
   }

   // Create the character variables needed by the CERNLIB functions:
   char aq[15], t[2];
   snprintf(aq, sizeof(aq), "acquisition");
   snprintf(t, sizeof(t), "T");

#ifdef HAVE_CERNLIB
   // Additional variable(s) needed by CERNLIB:
   int icycle;

   // Flush the memory buffers:
   HROUT(0, icycle, t);

   m_logger << msg::DEBUG
            << tr("A total of %1 events written to the ntuple").arg(m_events)
            << msg::endmsg;

   // Close the file:
   HREND(aq);
#endif  // HAVE_CERNLIB

   m_logger << msg::INFO << tr("Output file closed") << msg::endmsg;
   m_fileOpen = false;

   // Delete the event buffer:
   delete[] m_variables;
   m_variables = 0;

   return;
}

/**
 * This function can be used to set the values of the variables in the
 * output ntuple. The idea is that for each event that was collected by
 * the DAQ system, each output variable is set separately by the CAMAC
 * device objects, then the NTupleMgr::saveEvent function is called to
 * write the full event to the ntuple.
 *
 * @param index The index that was returned by the NTupleMgr::addVar function
 * @param value Value of the variable in the current event
 * @returns <code>true</code> if the operation was successful,
 *          <code>false</code> otherwise
 */
bool NTupleMgr::setVar(int index, float value) {

   // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
   REPORT_VERBOSE(tr("Setting variable %1 to: %2").arg(index).arg(value));
   return false;
#endif  // HAVE_CERNLIB

   // Check that the file is already open:
   if (!m_fileOpen) {
      REPORT_ERROR(
          tr("You should not be setting variables before opening\n"
             "an output file!"));
      return false;
   }

   // Check that this is a valid index:
   if ((!(index < (int)m_varNames.size())) || (index < 0)) {
      REPORT_ERROR(tr("Index %1 is not defined in the ntuple!").arg(index));
      return false;
   }

   // Set the variable value in the internal buffer:
   m_variables[index] = value;

   return true;
}

/**
 * After the event variables have been set using the NTupleMgr::setVar
 * function, this function has to be called to save the event to the
 * output ntuple.
 */
void NTupleMgr::saveEvent() {

   // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
   return;
#endif  // HAVE_CERNLIB

   // Check that the output file is open:
   if (!m_fileOpen) {
      REPORT_ERROR(
          tr("saveEvent() should only be called when an output\n"
             "file is already open!"));
      return;
   }

   // Fill the ntuple with the event:
#ifdef HAVE_CERNLIB
   HFN(NTUPLE_ID, m_variables);
#endif  // HAVE_CERNLIB

   // Clear the event buffer:
   for (unsigned int i = 0; i < m_varNames.size(); ++i) {
      m_variables[i] = 0.;
   }

   ++m_events;

   return;
}

/**
 * This function clears all settings of the object. In retrospective,
 * I'm not sure where this would be used in the CDA code... :-/
 */
void NTupleMgr::clear() {

   if (m_fileOpen)
      closeFile();
   m_varNames.clear();
   m_events = 0;

   return;
}

}  // namespace cernlib
