#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-apps-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DevFirstExample");

static void PingRtt (std::string context, Time rtt)
{
  std::cout << context << " " << rtt << std::endl;
}

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  
  NodeContainer nodes;
  nodes.Create (6);
 
  InternetStackHelper stack;
  stack.Install (nodes);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (10000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (0.2)));
  
  
  NetDeviceContainer devices;
  devices = csma.Install(nodes);
  
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  V4PingHelper ping = V4PingHelper (interfaces.GetAddress (2));
  
  NodeContainer pingers;
  pingers.Add (nodes.Get (0));

  ApplicationContainer apps;
  apps = ping.Install (pingers);

  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));
 
// finally, print the ping rtts.
  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::V4Ping/Rtt",
                   MakeCallback (&PingRtt));
 
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  OnOffHelper onoff("ns3::UdpSocketFactory",Address(InetSocketAddress (interfaces.GetAddress(2), 9)));
  onoff.SetConstantRate (DataRate ("500Mb/s"));
  
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
  serverApps.Start (Seconds (11.0));
  serverApps.Stop (Seconds (20.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (2), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (10));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = (echoClient.Install (nodes.Get (0)));
  clientApps=onoff.Install (nodes.Get (0));
  
  clientApps.Start (Seconds (12.0));
  clientApps.Stop (Seconds (20.0));

  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream (" csma6nodesB.tr"));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

