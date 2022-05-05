

#include <cppunit/TestCase.h> 

using namespace CppUnit; 

class chat_client_functionTestCase : public TestCase 

{ 
public:

void testnewUserDataInOneString(); 
void testexistingUserCredInOneString();

static Test* suite(); 

}; 
