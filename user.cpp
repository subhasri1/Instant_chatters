#include<iostream>
#include<istream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
using namespace std;
void login();
void registr();

int main()
{
        int choice;
        cout<<"                      Welcome to login page                               \n\n";
        cout<<"*******        MENU        ***********\n\n";
        cout<<"1.REGISTER"<<endl;
        cout<<"2.Exit"<<endl;
        cout<<"\nEnter your choice :";
        cin>>choice;
        cout<<endl;
        switch(choice)
        {
                case 1:
                        registr();
                        break;
                case 2:
                        cout<<"You have exited successfully\n\n";
                        break;
                default:
                        system("cls");
                        cout<<"You've made a mistake , give it a try again\n"<<endl;
                        main();
        }
       
}

void registr()
{
       
        string reguser,regpass,confirmpass,regmobile,regemail,rpass,cpass;
        system("cls");
        cout<<"Enter the username :";
        cin>>reguser;
        cout<<"\nEnter the password :";
        cin>>regpass;
        cout<<"\nConfirm password :";
        cin>>confirmpass;
	again:
        	cout<<"\nEnter the password :";
        	cin>>regpass;
        	cout<<"\nConfirm password :";
        	cin>>confirmpass;
		
	

        if(regpass==confirmpass)
        {
            cout<<"\n Same password\n";
        }
        else
        {
            cout<<"\n password mismatch\n";
		cout<<"\nEnter the password :";
        	cin>>rpass;
        	cout<<"\nConfirm password :";
        	cin>>cpass;
		rpass=regpass;
		cpass=confirmpass;

        }
	
	

        cout<<"\nEnter the mobile no :";
        cin>>regmobile;
        cout<<"\nEnter the email :";
        cin>>regemail;
       
        ofstream data("userdetails.txt",ios::app);
        data<<reguser<<' '<<regpass<<' '<<regmobile<<' '<<regemail<<' '<<endl;
        system("cls");
        cout<<"\nRegistration Sucessful\n";
        main();
       
       
}
