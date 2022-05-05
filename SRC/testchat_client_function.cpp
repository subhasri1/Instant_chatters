
#include <cppunit/TestCase.h> 

#include <cppunit/TestCaller.h> 

#include <cppunit/TestSuite.h> 

#include <cppunit/ui/text/TestRunner.h> 
#include "../INC/chat_client_header.h"

using namespace CppUnit; 

#include <iostream> 

using namespace std; 

#include "../INC/testchat_client_function.h"

 

void chat_client_functionTestCase::testnewUserDataInOneString()

{	

newUser obj("124536","Aman","aman123","aman123","aman1234","aman@gmail.com","aman1234");
string s="2aman123,aman1234,Aman,124536,aman@gmail.com";
string s1="2aman123,aman123,Aman,124536,aman@gmail.com";
CPPUNIT_ASSERT(s==obj.newUserDataInOneString());
CPPUNIT_ASSERT(s1==obj.newUserDataInOneString());   
} 


void chat_client_functionTestCase::testexistingUserCredInOneString()

{	

newUser obj("124536","Aman","aman123","aman123","aman1234","aman@gmail.com","aman1234");

string s="1aman123,aman1234";
CPPUNIT_ASSERT(s==obj.existingUserCredInOneString());  
} 
 
//creating suite of tests for various functionalities 

Test* chat_client_functionTestCase::suite() 

{ 

TestSuite *testsuite = new TestSuite("chat_client_functionTestCase");	 

testsuite->addTest(new TestCaller< chat_client_functionTestCase>("testnewUserDataInOneString",& chat_client_functionTestCase ::testnewUserDataInOneString)); 

testsuite->addTest(new TestCaller< chat_client_functionTestCase>("testexistingUserCredInOneString",& chat_client_functionTestCase ::testexistingUserCredInOneString)); 

return testsuite; 

} 

int main(int argc, char *argv[]) 

{ 

if (argc!=2) 

{ 

cout << "usage error " << endl; 

cout << "tester <name of the class to be tested> " << endl; 

exit(1); 

} 

 

CppUnit::TextUi::TestRunner testrunner; 

testrunner.addTest(chat_client_functionTestCase::suite()); 

testrunner.run(); 

} 
