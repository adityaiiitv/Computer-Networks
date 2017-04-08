//	Group	:	3
//	Aditya Prakash		:	201351010
//	Manu Sharma			:	201351021
//	Anjul Kumar Tyagi	:	201352029

//	Header files inclusion
#include "ns3/netanim-module.h"			//	header file to use NetAnim
#include "ns3/core-module.h"			//	core module for ns3
#include "ns3/network-module.h"			//	for declaring NodeContainers
#include "ns3/internet-module.h"		//	for declaring internet stack helpers
#include "ns3/point-to-point-module.h"	//	for declaring point to point helper
#include "ns3/applications-module.h"	//	for installing client and server applications on nodes
#include <iostream>

using namespace ns3;					//	use namespace ns3 instead of using 'ns3::' everytime
using namespace std;					//	using standard library
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int main ()	//	main function	int argc, char *argv[]
{
  //CommandLine cmd;
  //cmd.Parse (argc, argv);
  Time::SetResolution (Time::NS);		//	time reso set to 1 Nano sec. discrete interval. minimum realisable difference between 2 instances
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);	//	enables recording of all the functions the client and server use
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	//	and the packets they receive. currently logged at level INFO
  
  int num;								//	number of nodes
  cout<<"\n Enter the number of nodes: ";
  cin>>num;								//	entering the number of nodes
  
  NodeContainer term[num];				//	an object array of class node container
  for(int i=0;i<num;i++)
  {
	  term[i].Create(1);				//	creating nodes are term[i]
  } 
  /*NodeContainer term_0;
  term_0.Create (1);
  NodeContainer term_1;
  term_1.Create (1);
  NodeContainer term_2;
  term_2.Create (1);
  */
  
  cout<<"\n Enter the connection matrix. The matrix should be symmetric";
  int conn[num][num],count=0;			//	a symmetric matrix conn is accepted to understand the connections
  for(int i=0;i<num;i++)
  {
	  for(int j=0;j<num;j++)
	  {
		  cout<<"\n Enter element"<<i<<" "<<j<<": ";
		  cin>>conn[i][j];
		  if(conn[i][j]==1)
		  {
			  count++;					//	counting the number of links
		  }
	  }
  }
  count  = count / 2;					//	as matrix is symmetric
  //	ESTABLISHING CHANNEL
  PointToPointHelper p2p_p2p[count];	//	object array of point to point helper used as channel with count number of elements
  for(int i=0;i<count;i++)
  {
	  p2p_p2p[i].SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));	//	setting the data rate of channel
	  p2p_p2p[i].SetChannelAttribute ("Delay", StringValue ("2ms"));		//	setting the delay rate of channel
  }
  /*
  p2p_p2p_0.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p_p2p_0.SetChannelAttribute ("Delay", StringValue ("2ms"));
  PointToPointHelper p2p_p2p_1;
  p2p_p2p_1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p_p2p_1.SetChannelAttribute ("Delay", StringValue ("2ms"));
*/
///////

  int done[num][num];					//	a zero matrix used later and converted to conn matrix
  for(int i=0;i<num;i++)
  {
	  for(int j=0;j<num;j++)
	  {
		  done[i][j]=0;
	  }
  }
  int nc=0;										//	used as a counter but comes out as 'count'
  InternetStackHelper internetStackH;			//	used to install internet protocol on nodes. nodes can use TCP,IP or UDP
  for(int i=0;i<num;i++)
  {
	internetStackH.Install (term[i]);			//	installing the stack helper on each node or term[i]
  }
  Ipv4AddressHelper ipv4;						//	class Ipv4AddressHelper to assign IP address onto the nodes
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");	//	setting the base and the subnet mask
  NodeContainer all_p2p[count];					//	to make a pair of nodes as another node
  //	to connect nodes and channel
  NetDeviceContainer ndc_p2p[count];			//	net device are the hardware installed on nodes to support specific channels
  int lib[count][3];							//	2D integer array to keep the track of nodes to install interface later
  for(int i=0;i<count;i++)
  {
	  for(int j=0;j<3;j++)
	  {
		 lib[i][j]=0; 							//	initialising to a zero matrix
	  }
  }
  for(int i=0;i<num;i++)
  {
	  for(int j=0;j<num;j++)
	  {
		  if(conn[i][j]==1 && done[i][j]==0)
		  {
			  all_p2p[nc].Add (term[i]);		//	adding the terms
			  all_p2p[nc].Add (term[j]);
			  lib[nc][0]=nc;
			  lib[nc][1]=i;
			  lib[nc][2]=j;
			  done[i][j]=1;						//	changing the done matrix
			  done[j][i]=1;
			  ndc_p2p[nc] = p2p_p2p[nc].Install (all_p2p[nc]);	//	this connects the nodes and p2p_p2p
			  ipv4.Assign (ndc_p2p[nc]);
              ipv4.NewNetwork ();				//	creating a new network everytime
			  nc++;
		  }
	  }
  }
  cout<<" \n the value of nc is: "<<nc<<"\n";
  /*
  NodeContainer all_p2p_0;
  all_p2p_0.Add (term_1);
  all_p2p_0.Add (term_0);
  NetDeviceContainer ndc_p2p_0 = p2p_p2p_0.Install (all_p2p_0);
  NodeContainer all_p2p_1;
  all_p2p_1.Add (term_1);
  all_p2p_1.Add (term_2);
  NetDeviceContainer ndc_p2p_1 = p2p_p2p_1.Install (all_p2p_1);
  */
  
  
  Ipv4InterfaceContainer iface_ndc_p2p[count];					//	declaring an array of interfaces to establish between the nodes
  for(int i=0;i<nc;i++)
  {
	  iface_ndc_p2p[i] = ipv4.Assign (ndc_p2p[i]);				//	assign IP address to the devices
  }
  /*
  Ipv4InterfaceContainer iface_ndc_p2p_0 = ipv4.Assign (ndc_p2p_0);
  ipv4.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer iface_ndc_p2p_1 = ipv4.Assign (ndc_p2p_1);
  */
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();			//	makes the intermediary nodes to act as routers , each node gets its own routing table

  /* Generate Application.
  uint16_t port_udpEcho_0 = 9;
  UdpEchoServerHelper server_udpEcho_0 (port_udpEcho_0);
  ApplicationContainer apps_udpEcho_0 = server_udpEcho_0.Install (term_2.Get(0));
  apps_udpEcho_0.Start (Seconds (0.0));
  apps_udpEcho_0.Stop (Seconds (10.0));
  Time interPacketInterval_udpEcho_0 = Seconds (1.0);
  UdpEchoClientHelper client_udpEcho_0 (iface_ndc_p2p_1.GetAddress(1), 9);
  client_udpEcho_0.SetAttribute ("MaxPackets", UintegerValue (1));
  client_udpEcho_0.SetAttribute ("Interval", TimeValue (interPacketInterval_udpEcho_0));
  client_udpEcho_0.SetAttribute ("PacketSize", UintegerValue (1024));
  apps_udpEcho_0 = client_udpEcho_0.Install (term_1.Get (0));
  apps_udpEcho_0.Start (Seconds (0.1));
  apps_udpEcho_0.Stop (Seconds (10.0));
*/

//	INSTALL APPLICATION
//	application used to generate data in the network
//char m[4]={'.','x','m','l'};
int c=0;													//	used to keep track of .xml files generated
//int counter=0;
for(int j=0;j<num;j++)										//	for every node
	{
		
		for(int k=0;k<num;k++)								//	to every node
		{
			if(j!=k)
			{
				// GENERATING APPLICATION
				UdpEchoServerHelper echoServer (9);			//	creating a UDP server at port number 9 , the port at which server waits for incoming packets
				ApplicationContainer serverApps = echoServer.Install (term[j].Get(0));					//	install server application on node 1
				serverApps.Start (Seconds (1.0));			//	app start time
				serverApps.Stop (Seconds (400.0));			//	app closing time
// logic
				//int check=0;
				UdpEchoClientHelper *echoClient;
				for(int i=0;i<nc;i++)						//	revisiting lib
				{
					for(int l=1;l<3;l++)
					{
						if(lib[i][l]==j) // && check == 0
						{
							echoClient = new UdpEchoClientHelper(iface_ndc_p2p[i].GetAddress (l-1), 9);	//	give the corresponding IP of the server
							//check++;
	
						}
					}
				}
							echoClient->SetAttribute ("MaxPackets", UintegerValue (1));				//	setting attributes for the client
							echoClient->SetAttribute ("Interval", TimeValue (Seconds (1.0)));
							echoClient->SetAttribute ("PacketSize", UintegerValue (1024));
			
							ApplicationContainer clientApps = echoClient->Install (term[k].Get (0));//	installing client
  
							clientApps.Start (Seconds (2.0));										//	app start time
							clientApps.Stop (Seconds (400.0));										//	app closing time
				//
				//cout<<"\n you reached here "<<counter<<"\n";
				//counter++;			
				char animat[100][11];						//	generating name of the file created
				for(int p=0;p<100;p++)
				{
					for(int q=0;q<11;q++)
					{
						animat[p][q]='\0';					//	initialising to null
					}
				}
				/*
				for(int i=0;i<8;i++)
				{
					animat[c][i]=m[i];
				}
*/
				/*int dig=c,dig1,dig2=0;
				dig1=(c%10);

					dig=dig/10;
					dig2=dig;
				*/
				animat[c][0]='A';							//	assigning the name
				animat[c][1]='n';
				animat[c][2]='i';
				animat[c][3]='m';
				animat[c][4]='e';
				animat[c][5]=j+48;							//	number to a character
				animat[c][6]=k+48;
				animat[c][7]='.';
				animat[c][8]='x';
				animat[c][9]='m';
				animat[c][10]='l';
				cout << animat[c]<<endl;
				
				
				AnimationInterface anim(animat[c]);									//	creating interface for animation
				double x=5.0,y=1.0;													//	the argument gives the name of the xml file created
				//anim.SetConstantPosition(term[0].Get(0),25.0,25.0);					//	used to set position of the nodes
																					//	setting the position of constants
				for(int i=0;i<num;i++)
				{
					anim.SetConstantPosition(term[i].Get(0),0.0 + (2.0)*y,0.0 + x*x);	//	used to set position of the nodes ,  the ith term
					y = y + 1.0;
					x = x + 1.0;
				}
				c++;
				//
				uint32_t stopTime = 11;
				Simulator::Stop (Seconds (stopTime));
				Simulator::Run ();  						//	starting simulator
				
				delete echoClient;
			}
		}
	}
  
  /*
  anim.SetConstantPosition(term_1.Get(0),30.0,30.0);
  anim.SetConstantPosition(term_0.Get(0),15.0,25.0);
  anim.SetConstantPosition(term_2.Get(0),5.0,15.0);  
  */
  				Simulator::Destroy ();						//	destroying simulator
  /* Start and clean simulation. */
}

/*
 * 0110
 * 1001
 * 1001
 * 0110
 */
