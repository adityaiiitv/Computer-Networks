#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  
  NodeContainer term_0;
  term_0.Create (1);
  NodeContainer term_1;
  term_1.Create (1);
  NodeContainer term_2;
  term_2.Create (1);
  
  PointToPointHelper p2p_p2p_0;
  p2p_p2p_0.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p_p2p_0.SetChannelAttribute ("Delay", StringValue ("2ms"));
  PointToPointHelper p2p_p2p_1;
  p2p_p2p_1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p_p2p_1.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NodeContainer all_p2p_0;
  all_p2p_0.Add (term_0);
  all_p2p_0.Add (term_1);
  NetDeviceContainer ndc_p2p_0 = p2p_p2p_0.Install (all_p2p_0);
  NodeContainer all_p2p_1;
  all_p2p_1.Add (term_0);
  all_p2p_1.Add (term_2);
  NetDeviceContainer ndc_p2p_1 = p2p_p2p_1.Install (all_p2p_1);
  
  InternetStackHelper internetStackH;
  internetStackH.Install (term_0);
  internetStackH.Install (term_1);
  internetStackH.Install (term_2);
  
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer iface_ndc_p2p_0 = ipv4.Assign (ndc_p2p_0);
  ipv4.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer iface_ndc_p2p_1 = ipv4.Assign (ndc_p2p_1);
  
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
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (term_2.Get(0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (iface_ndc_p2p_1.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (term_1.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));


  /* Simulation. */
  /* Pcap output. */
  /* Stop the simulation after x seconds. */
  uint32_t stopTime = 11;
  Simulator::Stop (Seconds (stopTime));
  
  AnimationInterface anim("anim2.xml");
  anim.SetConstantPosition(term_0.Get(0),10.0,20.0);
  anim.SetConstantPosition(term_1.Get(0),15.0,25.0);
  anim.SetConstantPosition(term_2.Get(0),5.0,15.0);  
  /* Start and clean simulation. */
  Simulator::Run ();
  Simulator::Destroy ();
}
