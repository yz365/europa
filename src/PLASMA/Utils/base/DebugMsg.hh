#ifndef _PLASMA_DEBUG_MSG
#define _PLASMA_DEBUG_MSG
#include <iostream>
#include <string>
#include <list>

/**
   @file DebugDefs.hh
   @author Europa project

   @brief  This file contains the original Europa logging class.  Numerous declarations related to debugging.
*/

#include "Error.hh"

/**
   @brief Returns the current level of the given marker.  If no level is provided
   in a configuration file for this marker, this method attempts to emulate
   the tree-based structure of log4cxx.  If the level cannot be determined
   by inheritance, then this method returns the lowest level.
*/
#define debugGetLevel( marker )



/** @brief Use the debugMsg() macro to create a debug message that
    will be printed when the code is executed if and only if this
    particular debug message has been 'enabled'.  @param marker A string
    that "marks" the message to enable it by.  @param data The data to
    be printed when the message is enabled.  @brief The data argument
    can be any "chain" of C++ expressions to output to the debug stream
    returned by DebugMessage::getStream() when this debug message is
    enabled (via, e.g. DebugMessage::enable() or
    DebugMessage::enableAll()).  @see condDebugMsg @see debugStmt @see
    condDebugStmt @see DebugMessage
*/
#define debugMsg(marker, data) condDebugMsg(true, marker, data)

/**
   @brief Level-specific version of debugMsg
   @note  Levels are ignored in this version of logging
*/
#define debugMsgLvl(marker, level, data) condDebugMsg(true, marker, level, data)

/**
   @brief Create a conditional debug message, which will
   only be created or used when the given condition is true at run time.
   @param cond An additional condition to be checked before printing the message,
   which can be any C/C++ expression that could be used in an if statement.
   @param marker A string that "marks" the message to enable it by.
   @param data The data to be printed when the message is enabled.
   @see debugMsg
   @see condDebugMsg
   @see debugStmt
   @see condDebugStmt
   @see DebugMessage
*/
#define condDebugMsg(cond, marker, data) {                              \
    static DebugMessage *dmPtr = DebugMessage::addMsg(__FILE__, __LINE__, marker); \
    if (dmPtr->isEnabled() && (cond)) {                                 \
      try {                                                             \
        DebugMessage::getStream().exceptions(std::ios_base::badbit);    \
        DebugMessage::getStream() << /*dmPtr[0] << */ "[" << marker << "] " << data << std::endl; \
      }                                                                 \
      catch(std::ios_base::failure& exc) {                              \
        checkError(ALWAYS_FAIL, exc.what());                            \
        throw;                                                          \
      }                                                                 \
    }                                                                   \
  }

/**
   @brief Level-specific version of condDebugMsg
   @note  Levels are ignored in this version of logging
*/
#define condDebugMsgLvl(cond, marker, level, data) condDebugMsg( cond, marker, data )

/**
   @brief Add code to be executed only if the DebugMessage is enabled.
   @param marker A string that "marks" the message to enable it by.
   @param stmt The code to be executed when the message is enabled.
   @see debugMsg
   @see condDebugMsg
   @see condDebugStmt
   @see DebugMessage
*/
#define debugStmt(marker, stmt) condDebugStmt(true, marker, stmt)

/**
   @brief Level-specific version of debuStmt
   @note  Levels are ignored in this version of logging
   @see condDebugStmtLvl
*/
#define debugStmtLvl(marker, level, stmt) condDebugStmt(true, marker, stmt)

/**
   @brief Add code to be executed only if the DebugMessage is enabled and
   the condition is true.
   @param cond An additional condition to be checked before printing the message,
   which can be any C/C++ expression that could be used in an if statement.
   @param marker A string that "marks" the message to enable it by.
   @param stmt The code to be executed when the message is enabled and the condition
   is true.
   @see debugMsg
   @see condDebugMsg
   @see debugStmt
   @see DebugMessage
*/
#define condDebugStmt(cond, marker, stmt) {                             \
    static DebugMessage *dmPtr = DebugMessage::addMsg(__FILE__, __LINE__, marker); \
    if (dmPtr->isEnabled() && (cond)) {                                 \
      stmt ;                                                            \
    }                                                                   \
  }

/**
   @brief Level-specific version of condDebugStmt
   @note  Levels are ignored in this version of logging
   @see condDebugStmt
*/
#define condDebugStmtLvl(cond, marker, level, stmt) {   \
    condDebugStmt( cond, marker, stmt )                 \
  }

#define CHECK_DEBUG_STREAM check_error(DebugMessage::isGood());

class DebugErr {
 public:
  DECLARE_ERROR(DebugStreamError);
  DECLARE_ERROR(DebugMessageError);
  DECLARE_ERROR(DebugMemoryError);
  DECLARE_ERROR(DebugConfigError);
};

/**
   @class DebugMessage Debug.hh
   @brief Implements support for debugMsg() macro, which should be used
   to create all instances.
*/
class DebugPattern;
class DebugMessage {

  typedef std::ostream* oSptr;

 private:

  /**
   * @brief Are all debug messages enabled?
   * @note Individual ones could be even when this is false.
   */
  static bool& allEnabled() {
    static bool s_allEnabled = false;
    return(s_allEnabled);
  }

  /**
   * @brief Construct a DebugMessage.
   * @param file File containing the debug message instance.
   * @param line Line on which it is declared/created.
   * @param marker Name for the particular instance (not required to be unique within the process).
   * @note Only constructor that should be used.
   * @note Should only be called from static member functions.
   */
  DebugMessage(const std::string& file, const int& line,
               const std::string& marker);


  /**
   * @brief Destroy a DebugMessage.
   * @note Should only be called implicitly (e.g., by std::list<DebugMessage>).
   */
  inline virtual ~DebugMessage() {
  }

 public:
  class DebugInternals;

  /**
     @brief Create a new DebugMessage.  Should only be called from the
     debugMsg() macro and readConfigFile().
     @param file The originating file
     @param line The line number
     @param marker The message marker for matching
     @par Errors thrown:
     @li If no debug stream has been assigned.
     @see DebugMessage::enable
     @see DebugMessage::setStream
  */
  static DebugMessage *addMsg(const std::string& file,
                              const int& line,
                              const std::string& marker);

  /**
     @brief Create a new DebugMessage.  Should only be called from the
     debugMsg() macro and readConfigFile().
     @param file The originating file
     @param line The line number
     @param marker The message marker for matching
     @param level The message level
     @par Errors thrown:
     @li If no debug stream has been assigned.
     @see DebugMessage::enable
     @see DebugMessage::setStream
  */
  static DebugMessage *addMsg(const std::string& file,
                              const int& line,
                              const std::string& marker,
                              const int level );

  /**
     @brief Find any matching DebugMessage.
     @param file The originating file
     @param pattern The pattern to match markers against
  */
  static DebugMessage *findMsg(const std::string& file,
                               const std::string& pattern);

  /**
     @brief Find all matching DebugMessages and appends them to matches parameter
     without emptying it first.
     @param file The originating file
     @param pattern The pattern to match markers against
     @param matches Destination for matches
  */
  static void findMatchingMsgs(const std::string& file,
                               const std::string& pattern,
                               std::list<DebugMessage*>& matches);

  /**
     @brief Get list of all debug known messages.
  */
  static std::list<DebugMessage*> getAllMsgs();

  /**
     @brief Enable all debug messages, including ones not yet created.
     @par Errors thrown:
     @li If no debug stream has been assigned.
     @see DebugMessage::setStream
  */
  static void enableAll();

  /**
   * @brief Disable all messages
   */
  static void disableAll();

  /**
     @brief Assign a stream to which all debug messages will be sent.
     @param os The output stream to send messages to
  */
  inline static void setStream(std::ostream& os) {
    streamPtr() = &os;
  }

  /**
     @brief Return the stream being used for debug messages.
  */
  inline static std::ostream& getStream() {
    return(*(streamPtr()));
  }

  /**
     @brief Read a list of debug message enablements from the
     stream argument.
     @param is Input stream to read
     @par Errors thrown:
     @li If the stream is not good.
     @li If setStream() has not been called
     and some existing debug messages should be enabled.
  */
  static bool readConfigFile(std::istream& is);

  /**
     @brief Return the file used to create the debug message.
  */
  inline const std::string& getFile() const {
    return( m_file );
  }

  /**
     @brief Return the line used to create the debug message.
  */
  inline int getLine() const {
    return( m_line );
  }

  /**
     @brief Return the marker used to create the debug message.
  */
  inline const std::string& getMarker() const {
    return( m_marker );
  }

  /**
     @brief Return whether the debug message is currently enabled.
  */
  inline bool isEnabled() const {
    return( m_enabled );
  }

  /**
     @brief Enable the debug message.
     @par Errors thrown:
     @li If the stream has not been set.
  */
  inline void enable() {
    checkError(streamPtr()->good(),
	       "cannot enable debug message(s) without a good debug stream: " <<
	       (streamPtr()->rdstate() & std::ostream::badbit ? " bad " : "") <<
	       (streamPtr()->rdstate() & std::ostream::eofbit ? " eof " : "") <<
	       (streamPtr()->rdstate() & std::ostream::failbit ? " fail " : "") <<
	       (streamPtr()->rdstate() & std::ostream::goodbit ? " good???" : ""));
    /*
      check_error(streamPtr()->good(),
      "cannot enable debug message(s) without a good debug stream:",DebugErr::DebugStreamError());
    */
    m_enabled = true;
  }

  /**
     @brief Disable the debug message.
  */
  inline void disable() {
    m_enabled = false;
  }

  /**
     @brief Print the data members of the debug message in a format
     that Emacs can use to display the corresponding source code.
     @param os The output stream to write to
  */
  inline void print(std::ostream *os = streamPtr()) const {
    try {
      os[0].exceptions(std::ostream::badbit);
      os[0] << m_file << ':' << m_line << ": " << m_marker << ' ';
    }
    catch(std::ios_base::failure& exc) {
      checkError(ALWAYS_FAIL, exc.what());
      throw;
    }
  }

  /**
     @brief Enable matching debug messages, including those created later.
     @param file The originating file
     @param marker Marker to match messages against
     @par Errors thrown:
     @li If a message would be enabled but no debug stream has been set.
     @see DebugMessage::setStream
  */
  static void enableMatchingMsgs(const std::string& file,
                                 const std::string& marker);

  /**
     @brief Disable matching debug messages, including those created later.
     @param file The originating file
     @param marker Marker to match messages against
  */

  static void disableMatchingMsgs(const std::string& file,
                                  const std::string& marker);

  /**
     @brief Whether the message is matched by the pattern.
  */
  bool matches(const DebugPattern& pattern) const;

  static bool isGood() {
    return streamPtr()->good();
  }

 private:
  /**
     @brief The pointer to the stream being used.
     @note Has to be a pointer because some C++ compiler
     implementations have a private operator=().
  */
  inline static oSptr& streamPtr() {
    static oSptr s_debugStream = &(std::cerr);
    return(s_debugStream);
  }

  /**
     @brief File given when this instance was created.
  */
  std::string m_file;

  /**
     @brief Line given when this instance was created.
  */
  int m_line;

  /**
     @brief Marker given when this instance was created.
  */
  std::string m_marker;

  /**
     @brief Whether this instance is 'enabled' or not.
  */
  bool m_enabled;

  /**
     @brief Whether the given marker matches the "pattern".
     Exists solely to ensure the same method is always used to check
     for a match.
  */
  inline static bool markerMatches(const std::string& marker,
                                   const std::string& pattern) {
    if (pattern.length() < 1)
      return(true);
    if (marker.length() < pattern.length())
      return(false);
    return(marker.find(pattern) < marker.length());
  }

  /**
     @brief Should not be used.
  */
  DebugMessage();

  /**
     @brief Should not be used.
  */
  DebugMessage(const DebugMessage&);

  /**
     @brief Should not be used.
  */
  DebugMessage& operator=(const DebugMessage&);

  /**
     @brief Should not be used.
  */
  bool operator==(const DebugMessage&) const;

}; //end of class DebugMessage

inline std::ostream& operator<<(std::ostream& os, const DebugMessage& dm) {
  dm.print(&os);
  return(os);
}
#endif // _PLASMA_DEBUG_MSG
