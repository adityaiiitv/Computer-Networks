#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <iostream>
using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  
  int num;
  cout<<"\n Enter the number of nodes: ";
  cin>>num;
  
  NodeContainer term[num];
  for(int i=0;i<num;i++)
  {
	  term[i].Create(1);
  } 
  /*NodeContainer term_0;
  term_0.Create (1);
  NodeContainer term_1;
  term_1.Create (1);
  NodeContainer term_2;
  term_2.Create (1);
  */
  
  cout<<"\n Enter the connection matrix. The matrix should be symmetric";
  int conn[num][num],count=0;
  for(int i=0;i<num;i++)
  {
	  for(int j=0;j<num;j++)
	  {
		  cout<<"\n Enter element"<<i<<" "<<j<<": ";
		  cin>>conn[i][j];
		  if(conn[i][j]==1)
		  {
			  count++;
		  }
	  }
  }
  count  = count / 2;
  
  PointToPointHelper p2p_p2p[count];
  for(int i=0;i<count;i++)
  {
	  p2p_p2p[i].SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	  p2p_p2p[i].SetChannelAttribute ("Delay", StringValue ("2ms"));
  }
  /*
  p2p_p2p_0.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p_p2p_0.SetChannelAttribute ("Delay", StringValue ("2ms"));
  PointToPointHelper p2p_p2p_1;
  p2p_p2p_1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p_p2p_1.SetChannelAttribute ("Delay", StringValue ("2ms"));
*/
///////

  int done[num][num];
  for(int i=0;i<num;i++)
  {
	  for(int j=0;j<num;j++)
	  {
		  done[i][j]=0;
	  }
  }
  int nc=0;
  InternetStackHelper internetStackH;
  for(int i=0;i<num;i++)
  {
	internetStackH.Install (term[i]);
  }
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  NodeContainer all_p2p[count];
  NetDeviceContainer ndc_p2p[count];
  for(int i=0;i<num;i++)
  {
	  for(int j=0;j<num;j++)
	  {
		  if(conn[i][j]==1 && done[i][j]==0)
		  {
			  all_p2p[nc].Add (term[i]);
			  all_p2p[nc].Add (term[j]);
			  done[i][j]=1;
			  done[j][i]=1;
			  ndc_p2p[nc] = p2p_p2p[nc].Install (all_p2p[nc]);
			  ipv4.Assign (ndc_p2p[nc]);
              ipv4.NewNetwork ();
			  nc++;
		  }
	  }
  }
  
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
  
  
  Ipv4InterfaceContainer iface_ndc_p2p[count];
  for(int i=0;i<count;i++)
  {
	  iface_ndc_p2p[i] = ipv4.Assign (ndc_p2p[i]);
  }
  /*
  Ipv4InterfaceContainer iface_ndc_p2p_0 = ipv4.Assign (ndc_p2p_0);
  ipv4.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer iface_ndc_p2p_1 = ipv4.Assign (ndc_p2p_1);
  */
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

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
// INSTALL APPLICATION

// ALL NODES LOGIC

char m[4]={'.','x','m','l'};
int c=0;
	for(int j=0;j<num;j++)
	{
		for(int k=0;k<num;k++)
		{
			if(j!=k)
			{
				UdpEchoServerHelper echoServer (9);
				ApplicationContainer serverApps = echoServer.Install (term[j].Get(0));
				serverApps.Start (Seconds (1.0));
				serverApps.Stop (Seconds (10.0));
				
				UdpEchoClientHelper echoClient (iface_ndc_p2p[j+1].GetAddress (0), 9);
				echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
				echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
				echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

				ApplicationContainer clientApps = echoClient.Install (term[k].Get (0));
				clientApps.Start (Seconds (2.0));
				clientApps.Stop (Seconds (10.0));

				char animat[31][6];
				/*for(int i=0;i<8;i++)
				{
					anim[c][i]=m[i];
				}
*/
				int dig=c,dig1=c,dig2=0;
				while(dig!=0)
				{
					dig=dig/10;
					dig2=dig;
				}
				animat[c][0]=dig2;
				animat[c][1]=dig1;
				for(int l=0;l<4;l++)
				{
					animat[c][l+2]=m[l];
				}
				cout << animat[c]<<endl;
				/*AnimationInterface anim(animat[c]);
				double x=5.0,y=1.0;
				anim.SetConstantPosition(term[0].Get(0),25.0,25.0);
				for(int i=1;i<num;i++)
				{
					anim.SetConstantPosition(term[i].Get(0),0.0 + x*y,0.0 + y*x);
					y = y + 5.0;
					x = x + 4.0;
				}*/
				c++;
				
				uint32_t stopTime = 11;
				Simulator::Stop (Seconds (stopTime));
				Simulator::Run ();  
				Simulator::Destroy ();
			}
		}
	}
/*
  ApplicationContainer serverApps = echoServer.Install (term[num-1].Get(0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (iface_ndc_p2p[count-1].GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (term[0].Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

*/


  /* Simulation. */
  /* Pcap output. */
  /* Stop the simulation after x seconds. */

  /*
  anim.SetConstantPosition(term_1.Get(0),30.0,30.0);
  anim.SetConstantPosition(term_0.Get(0),15.0,25.0);
  anim.SetConstantPosition(term_2.Get(0),5.0,15.0);  
  */
  /* Start and clean simulation. */


}
