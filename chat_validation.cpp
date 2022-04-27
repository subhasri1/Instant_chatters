#include <iostream>
#include <cstring>
using namespace std;


int main()
{
	    char username[20];
	        char passwrd[20];
		    int sno=0,pno=0,trial=3;
		        char j[]="John";
			    char p[]="Peter";
			        char s[]="Smith";
				    char pj[]="John4444";
				        char pp[]="Peter55555";
					    char ps[]="Smith55555";
					        cout<<"Enter username: ";
						    cin>>username;

						        if(strcmp(username,j)==0)
								    sno=1;
							    else if(strcmp(username,p)==0)
								        sno=2;
							        else if(strcmp(username,s)==0)
									    sno=3;
								    else
									        { cout<<"\nInvalid Username";
											    exit(0);
											        }


								        do
										    {
											            cout<<"\nEnter password: ";
												            cin>>passwrd;

													            switch(sno)
															            {
																	            case 1:  if(strcmp(pj,passwrd)==0)
																				                         pno=1;
																			                         break;

																						         case 2:     if(strcmp(pp,passwrd)==0)
																									                         pno=1;
																								                         break;

																											         case 3:     if(strcmp(ps,passwrd)==0)
																														                         pno=1;
																													                         break;

																																         }
														            if(pno==0)
																            {
																		                cout<<"\n\n!!!Invalid password!!!\n";
																				            trial-=1;
																					                while(trial)
																								            {cout<<"\nYou have "<<trial<<" more chance to enter correct password\n";
																										                break;
																												            }
																							        }
															            else if(pno==1)
																	            {
																			                trial=0;
																					            cout<<"\n\nSuccessful Login ";
																						                trial=3;
																								            exit(0);
																									            }

																        }while(trial>0);

									    if(trial==0)
										         cout<<"\n\n!!!Please try again after 15 mins!!!";

									   return 0;
}


	    
	      
		    
		        
			    
			        
				    
				        
					    
					        
						    

						        
								
							    
								        
							        
									    
								    
									        
											    
											        


								        
										    
											     
												            

													           

															        
																	            
																				       


