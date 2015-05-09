#ifndef _TESTUTILS_H_
#define _TESTUTILS_H_

namespace EUROPA {
  void setTestLoadLibraryPath(std::string path);

  std::string getTestLoadLibraryPath();
}

#define EUROPA_runTest(test) { \
     try { \
       unsigned long id_count = EUROPA::IdTable::size(); \
       bool result = test(); \
       EUROPA::IdTable::checkResult(result,id_count); \
     } \
       catch ( Error err ){ \
       err.print( std::cout ); \
     } \
   }


#endif /* _TESTUTILS_H_ */
