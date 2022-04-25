#include<iostream>
#include<istream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
using namespace std;
void registr();
void login();
int main()
{
        int choice;
        cout<<" Welcome to login page \n";
	cout<<endl;
        cout<<" MENU \n";
	cout<<endl;
        cout<<"1.REGISTER"<<endl;
	cout<<"2.Login"<<endl;
        cout<<"3.Exit"<<endl;
        cout<<"\nEnter your choice :";
        cin>>choice;
        cout<<endl;
        switch(choice)
        {
                case 1:
                        registr();
                        break;
		case 2:
			login();
			break;
                case 3:
                        cout<<"You have exited successfully\n\n";
                        break;
                default:
			system("clear");
                        cout<<"Invalid choice! Try again\n"<<endl;
                        main();
        }
       
}
void login()
{
        int a;
        string user,pass,u,p;
        cout<<"please enter the following details"<<endl;
        cout<<"USERNAME :";
        cin>>user;
        cout<<"PASSWORD :";
        cin>>pass;
        
        ifstream input("userdetails.txt");
        while(input>>u>>p)
        {
                if(u==user && p==pass)
        
                {
                        a=1;
                }
        }
        input.close();
        if(a==1)
        {
                cout<<"\nHello "<<user<<"\nYou are logged in successfully\n";
		cout<<endl;
                cin.get();
		cin.get();
                main();
        }
        else
        {	
		system("clear");
                cout<<"\nLOGIN ERROR\nPlease check your username and password\n";
		cout<<endl;
                main();
        }
}

void registr()
{
       
        string reguser,regpass,confirmpass,regmobile,regemail,rpass,cpass;
        cout<<"Enter the username :";
        cin>>reguser;
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
	system("clear");
        cout<<"\nRegistration Sucessful\n";
	cout<<endl;
        main();       
}
