//  Copyright Notices

//  This software was developed for use by the U.S. Government as
//  represented by the Administrator of the National Aeronautics and
//  Space Administration. No copyright is claimed in the United States
//  under 17 U.S.C. 105.

//  This software may be used, copied, and provided to others only as
//  permitted under the terms of the contract or other agreement under
//  which it was acquired from the U.S. Government.  Neither title to nor
//  ownership of the software is hereby transferred.  This notice shall
//  remain on all copies of the software.

//!!Two doxygen comments for each #define depending on another #define is
//!!  a very bad idea.  There's no way to keep them in sync and no point
//!!  in doing so if doxygen is not using the C/C++ preprocessor.
//!!--wedgingt@email.arc.nasa.gov 2004 Oct 5

/**
   @file Error.hh
   @author Will Edgington

   @brief Numerous declarations related to error generation and handling.

   @note These macros should be used rather than direct calls to throw
   unless an existing Error is being "re-thrown" from a catch block.

   @note This is presently only the "interface for programmers";
   nothing has been tested.

   @note Think of the first section, #ifdef EUROPA_FAST, as the
   "prototypes" and documentation as well as the "production" variant.
*/

#ifndef _H_Error
#define _H_Error

/* $Id: Error.hh,v 1.6 2007-04-29 04:36:18 miatauro Exp $ */
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

#define _ARG3(_0, _1, _2, _3, ...) _3
#define NARG3(...) _ARG3(__VA_ARGS__, 3, 2, 1, 0)

/**
 * @def ALWAYS_FAIL
 * False.
 * @note Used as argument to assert() and similar functions to
 * make clear that the assertion will fail, throw an error, etc.
 */
#define ALWAYS_FAIL (false)

/**
 * @def DECLARE_ERROR
 * Declare an error as a function that returns a string naming itself.
 * @param error The error to declare.
 */
#define DECLARE_ERROR(error) \
  static const std::string& error() { \
    static const std::string sl_lblStr(#error); \
    return(sl_lblStr); \
  }

#define EUROPA_ERROR(__e__) if(Error::throwEnabled()) {throw __e__;} else {__e__.handleAssert();}

#define assert1(value, cond) {                                           \
    if((cond) == value) {                                               \
      Error __e__(#cond, __FILE__, __LINE__); EUROPA_ERROR(__e__);      \
    }                                                                   \
  }

#define assert2(value, cond, msg) {                                      \
    if((cond) == value) {                                               \
      Error __e__(#cond, msg, __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }

#define assert3(value, cond, msg, type) {                               \
    if((cond) == value) {                                               \
      Error __e__(#cond, msg, type, __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }

#define __assert_(value, N, ...) assert ## N (value, __VA_ARGS__)
#define _assert(value, N, ...) __assert_(value, N, __VA_ARGS__)

#define assertTrue(...) _assert(false, NARG3(__VA_ARGS__), __VA_ARGS__)
#define assertFalse(...) _assert(true, NARG3(__VA_ARGS__), __VA_ARGS__)


#define check_error_function __attribute__ ((unused))

#ifdef EUROPA_FAST

/**
 * @def check_error_variable
 * Declare a variable as only being used for checkError (supresses warnings when compiling optimized).
 */
#  define check_error_variable(decl) 


/**
 * @def check_error
 * Check whether an error has occurred and create an error instance if so.
 * @param cond The condition to verify; if false, an error has occurred.
 * @param optarg Other values to pass to the class Error constructor when creating the error instance.
 * @note When EUROPA_FAST is defined, these are ignored.
 */
#  define check_error1(cond)
#  define check_error2(cond, msg)
#  define check_error3(cond, msg, type)
#  define checkError1(cond)
#  define checkError2(cond, msg)
#  define checkError3(cond, msg, type)

/**
 * @def warn
 * Print a warning if such is enabled.
 * @param msg The information to print.
 * @note When EUROPA_FAST is defined, these are ignored
 */
#  ifndef warn
#    define europaWarn(msg)
#  endif

/**
 * @def condWarning
 * Print a warning if the condition is false.
 * @param cond The condition to test; if false, print the warning.
 * @param msg The information to print in the warning.
 * @note When EUROPA_FAST is defined, these are ignored
 */
#  define condWarning(cond, msg)

#else //EUROPA_FAST

#  define check_error_variable(decl) decl

/**
 * @def check_error
 * @brief If the condition is false, generate an error.
 * @param cond The condition to test.
 * @note The new error handling support is in use, this will instead
 *   throw an exception when the condition is false.
 * @note Should only be used in 'base' code and in test programs when the
 *   condition cannot be tested when the 'core' code has been compiled 'fast'.
 *   Otherwise - in test programs where the condition can always be checked,
 *   no matter how the code was compiled - use assertTrue(), assertFalse(), etc.
 * @note This macro has three flavors:
 * @item check_error(condition) - If the condition is true, do nothing.
 *  If false and all errors are being printed, print it with the message.
 *  If false, throw an exception with location information and the message.
 * @item check_error(condition, string) - If the condition is true, do nothing.
 *  If false and all errors are being printed, print it just before throwing it.
 *  If false, throw an exception with location information and the message.
 * @item check_error(condition, exception) - If the condition is true, do nothing.
 *  If false and all errors are being printed, print it just before throwing it.
 *  If false, throw the given exception after adding location information.
 *
 * @see assertTrue, assertFalse, ALWAYS_FAIL
*/
#  define check_error1(cond) {                                          \
    if(!(cond)) {                                                       \
      Error __e__(#cond, __FILE__, __LINE__); EUROPA_ERROR(__e__);      \
    }                                                                   \
  }
#  define check_error2(cond, msg) {                                     \
    if(!(cond)) {                                                       \
      Error __e__(#cond, msg, __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }
#  define check_error3(cond, msg, type) {                               \
    if(!(cond)) {                                                       \
      Error __e__(#cond, msg, type, __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }

#  define checkError1(cond) {                              \
    if(!(cond)) {                                               \
      Error __e__(#cond, __FILE__, __LINE__); EUROPA_ERROR(__e__);  \
    }                                                           \
  }

#  define checkError2(cond, msg) {                                      \
    if (!(cond)) {                                                      \
      std::stringstream sstr;                                           \
      sstr << msg;                                                      \
      Error __e__(#cond, sstr.str(), __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }

#  define checkError3(cond, msg, type) {          \
    if (!(cond)) {                                \
      std::stringstream sstr;                     \
      sstr << msg;                                                      \
      Error __e__(#cond, sstr.str(), #type,  __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }

#  ifndef warn
#    define europaWarn(msg) (Error::printWarning((msg), __FILE__, __LINE__))
#  endif

#  define condWarning(cond, msg) {                \
    if (!(cond)) {                                      \
      Error::printWarning((msg), __FILE__, __LINE__);   \
    }                                                   \
  }

#endif /* EUROPA_FAST */


/**
 * @def check_runtime_error
 * @brief If the condition is false, throw an exception
 * @param cond The condition to test.
 * @note check_error is used for testing assertions that could potentially be removed
 * from the final binary depending on the debug level that is used to build
 * check_runtime_error on the other hand is always part of the code, calling code needs to be able to 
 * handle the possible failure (even if it's at the outer-most level). 
 * TODO: we may also provide a flag to remove even check_runtime_error at some point.
 * we probably need to rethink flag/build setup, as we need to be able to build in release mode, while keeping
 * check_runtime_error. 
 * removing check_error has to do with debug levels.
 * removing check_runtime_error doesn't have to do with debugging, but with leaving behavior out for speed sake.
 */

#  define check_runtime_error1(cond) {                                  \
    if(!(cond)) {                                                       \
      Error __e__(#cond, __FILE__, __LINE__); EUROPA_ERROR(__e__);      \
    }                                                                   \
  }
#  define check_runtime_error2(cond, msg) {                             \
    if(!(cond)) {                                                       \
      Error __e__(#cond, msg, __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }
#  define check_runtime_error3(cond, msg, type) {                       \
    if(!(cond)) {                                                       \
      Error __e__(#cond, msg, type, __FILE__, __LINE__); EUROPA_ERROR(__e__); \
    }                                                                   \
  }
#define __check_error(N, ...) check_error ## N(__VA_ARGS__)
#define _check_error(N, ...) __check_error(N, __VA_ARGS__)
#define check_error(...) _check_error(NARG3(__VA_ARGS__), __VA_ARGS__)

#define __checkError(N, ...) checkError ## N(__VA_ARGS__)
#define _checkError(N, ...) __checkError(N, __VA_ARGS__)
#define checkError(...) _checkError(NARG3(__VA_ARGS__), __VA_ARGS__)

#define __check_runtime_error(N, ...) check_runtime_error ## N(__VA_ARGS__)
#define _check_runtime_error(N, ...) __check_runtime_error(N, __VA_ARGS__)
#define check_runtime_error(...) _check_runtime_error(NARG3(__VA_ARGS__), __VA_ARGS__)


#define checkRuntimeError(cond, msg) { \
  if (!(cond)) { \
    std::stringstream sstr; \
    sstr << msg; \
    Error __e__(#cond, sstr.str(), __FILE__, __LINE__); if(Error::throwEnabled()){throw __e__;}else{__e__.handleAssert();} \
  } \
}

/**
   @class Error
   @brief Used whenever a C++ exception is thrown.
*/
class Error {
public:

  /**
     @brief Build an Error object from the usual information: the false/failed
     condition and the source file and line where it was tested/detected.
  */
  Error(const std::string& condition, const std::string& file, const int& line);

  /**
     @brief Build an Error object from the information given, including an extra message.
  */
  Error(const std::string& condition, const std::string& msg, const std::string& file, const int& line);

  /**
     @brief Build an Error object from the information given, including another Error object's info.
  */
  Error(const std::string& condition, const Error& exception, const std::string& file, const int& line);

  /**
     @brief Build an Error object from the information given, including an extra message and type.
  */
  Error(const std::string& condition, const std::string& msg, const std::string& type,
        const std::string &file, const int& line);

  /**
     @brief Build an Error object from only a message.

     @note Should only be used when setCause() will be called before
     the Error is thrown.

     @note Never prints anything, unlike the other constructors.
     @see Error::setCause()
  */
  inline Error(const std::string& msg)
    : m_condition(), m_msg(msg), m_file(), m_line(0), m_type("Error") {
  }

  /**
     @brief Copy constructor.
  */
  inline Error(const Error& err)
    : m_condition(err.m_condition), m_msg(err.m_msg), m_file(err.m_file), 
      m_line(err.m_line), m_type("Error") {
  }

  /**
     @brief Assignment operator.
  */
  Error& operator=(const Error& err) {
    m_condition = err.m_condition;
    m_msg = err.m_msg;
    m_file = err.m_file;
    m_line = err.m_line;
    return(*this);
  }

  /**
     @brief Modify the Error's information as requested.

     @note Appears to not work reliably with g++ v2.95.2 or v3.2.2 just
     before throwing the same Error.
  */
  void setCause(const std::string& condition, const std::string& file, const int& line);

  /**
     @brief Modify the Error's additional message as requested.
  */
  inline void setMsg(const std::string& msg) {
    m_msg = msg;
  }

  /**
     @brief Get the Error's message.
  */
  inline const std::string& getMsg() const {
    return(m_msg);
  }

  /**
     @brief Set the Error's type.
  */
  inline void setType(const std::string& type) {
    m_type = type;
  }

  /**
     @brief Get the Error's type.
  */
  inline const std::string& getType() const {
    return(m_type);
  }

  inline const std::string& getCondition() const {
    return(m_condition);
  }

  inline int getLine() const {
    return(m_line);
  }

  /**
     @brief Set the Error's file.
  */
  inline void setFile(const std::string& file) {
    m_file = file;
  }

  /**
     @brief Get the Error's file.
  */
  inline const std::string& getFile() const {
    return(m_file);
  }

  /**
     @brief Return whether all error information should be printed when detected.
  */
  inline static bool& printingErrors() {
    return(s_printErrors);
  }

  /**
     @brief Indicate that error information should be printed at detection.
  */
  inline static void doDisplayErrors() {
    s_printErrors = true;
  }

  /**
     @brief Indicate that nothing should be printed when an error is detected.
  */
  inline static void doNotDisplayErrors() {
    s_printErrors = false;
  }

  /**
     @brief Return the output stream to which error information should be sent.
  */
  inline static std::ostream& getStream() {
    if (s_os == 0)
      s_os = &(std::cerr);
    return(*s_os);
  }

  /**
     @brief Indicate where output related to errors should be directed.
  */
  inline static void setStream(std::ostream& os) {
    s_os = &os;
  }

  /**
   * Display in "error format" (for Emacs, e.g.) on the stream (getStream()).
   */
  void display();

  /**
   * Print as if calling an Error constructor.
   */
  void print(std::ostream& os = std::cerr) const;

  /**
     @brief Compare two Errors.
  */
  inline bool operator==(const Error& err) const {
    return(m_condition == err.m_condition &&
           m_msg == err.m_msg &&
           m_file == err.m_file &&
           m_line == err.m_line);
  }

  /**
     @brief Return true iff (if and only if) the two Errors
     "match": are the same except for possibly the line numbers.
  */
  inline bool matches(const Error& err) const {
    return(m_condition == err.m_condition &&
           m_msg == err.m_msg &&
           m_file == err.m_file);
  }

  /**
   * The Error destructor.
   * @note Should only be used implicitly.
   */
  virtual ~Error();

  /**
     @brief Print a warning in the same format used by Error::display()
     unless printing warnings has been disabled.
  */
  static void printWarning(const std::string& msg,
                           const std::string& file,
                           const int& line);

  /**
     @brief Return true if printing warnings and false if not.
  */
  inline static bool& displayWarnings() {
    return(s_printWarnings);
  }

  /**
   * Indicate that warnings should be printed when detected.
  */
  inline static void doDisplayWarnings() {
    s_printWarnings = true;
  }

  /**
   * Indicate that warnings should not be printed.
  */
  inline static void doNotDisplayWarnings() {
    s_printWarnings = false;
  }

  /**
   * Indicate that errors should throw exceptions rather than
   * complaining and aborting.
   */
  inline static void doThrowExceptions() {
    s_throw = true;
  }

  /**
   * Indicate that errors should complain and abort rather than throw
   * exceptions.
   */
  inline static void doNotThrowExceptions() {
    s_throw = false;
  }

  /**
   * Are errors set to throw exceptions?
   * @return true if so; false if errors will complain and abort.
   */
  inline static bool throwEnabled() {
    return(s_throw);
  }

  DECLARE_ERROR(GeneralMemoryError);
  DECLARE_ERROR(GeneralUnknownError);

  /**
   * Actually throw the exception or complain and abort.
   * @note Which is done depends on throwEnabled().
   * @see throwEnabled
   */
  void handleAssert() __attribute__((noreturn));

private:

  std::string m_condition; /**<The condition that, being false, implies the error has occurred. */
  std::string m_msg; /**<Additional info about the error. */
  std::string m_file; /**<The source file in which the error was detected (__FILE__). */
  int m_line; /**<The source line on which the error detected (__LINE__). */
  std::string m_type; /**<The type of the error as. */
  static bool s_throw; /**<Set to throw exception. */
  static bool s_printErrors; /**<Set to print errors when detetected */
  static bool s_printWarnings; /**<Set to print warnings */
  static std::ostream *s_os; /**<The stream to write all error data to. */
  Error(); /**<The zero argument constructor should not be used. */
};

inline std::ostream& operator<<(std::ostream& os, const Error& err) {
  err.print(os);
  return(os);
}

#endif /* _H_Error */
