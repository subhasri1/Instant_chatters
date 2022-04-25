#include <iostream>
#include<fstream>
using namespace std;
int main(){
                 int ch;
                 system("cls");
                 cout<<"1.Search your password by username"<<endl;
                 cout<<"2.Search your username by password"<<endl;
                 cout<<"3.Main menu"<<endl;
                 cout<<"Enter your choice :";
                 cin>>ch;
                switch(ch)
               {
                case 1:
                {
                        int ex=0;
                        string searchuser,su,sp;
                        cout<<"\nEnter your username :";
                        cin>>searchuser;

                        ifstream searchu("data.txt");
                        while(searchu>>su>>sp)
                        {
                                if(su==searchuser)
                                {
                                        ex=1;
                                }
                        }
                        searchu.close();
                        if(ex==1)
                        {
                                cout<<"\n\nHurray, account found\n";
                                cout<<"\nYour password is "<<sp;
                                cin.get();
                                cin.get();
                                system("cls");

                        }
                        else
                        {
                                cout<<"\nSorry, Your userID is not found in our database\n";
                                cout<<"\nPlease kindly contact your system administrator for more details \n";
                                cin.get();
                                cin.get();

                        }
                        break;
                }
                case 2:
                {
                        int exi=0;
                        string searchpass,su2,sp2;
                        cout<<"\nEnter the remembered password :";
                        cin>>searchpass;

                        ifstream searchp("database.txt");
                        while(searchp>>su2>>sp2)
                        {
                                if(sp2==searchpass)
                                {
                                        exi=1;
                                }
                        }
                        searchp.close();
                        if(exi==1)
                        {
                                cout<<"\nYour password is found in the database \n";
                                cout<<"\nYour username is : "<<su2;
                                cin.get();
                                cin.get();
                                system("cls");

                        }
                        else
                        {
                                cout<<"Sorry, We cannot found your password in our database \n";
                                cout<<"\nkindly contact your administrator for more information\n";
                                cin.get();
                                cin.get();

                        }

                        break;
                }

                case 3:
                {
                        cin.get();

                }
                default:
                        cout<<"Sorry, You entered wrong choice. Kindly try again"<<endl;

            }
            }


