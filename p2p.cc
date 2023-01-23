#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DevFirstExample");

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  
  NodeContainer nodes;
  nodes.Create (3);

  InternetStackHelper stack;
  stack.Install (nodes);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes.Get (0), nodes.Get (1));
  
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  devices = pointToPoint.Install (nodes.Get (1), nodes.Get (2));
  address.SetBase ("10.1.2.0", "255.255.255.0");
  interfaces = address.Assign (devices);
  
    Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();

    //Introduce error model to drop packets
    em->SetAttribute ("ErrorRate", DoubleValue (0.002));
    devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em)); 
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  
  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream (" example 12.tr"));
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
