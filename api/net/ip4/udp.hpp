#ifndef NET_IP4_UDP_HPP
#define NET_IP4_UDP_HPP

#include <map>
#include "../ip4.hpp"
#include "../inet.hpp"

namespace net
{
  class PacketUDP;
  class SocketUDP;
  
  /** Basic UDP support. @todo Implement UDP sockets.  */
  class UDP
  {
  public:
    typedef IP4::addr addr_t;
    /** UDP port number */
    typedef uint16_t port;
  
    /** A protocol buffer temporary type. Later we might encapsulate.*/
    typedef uint8_t* pbuf;
  
    /** UDP header */
    struct udp_header {
      port sport;
      port dport;
      uint16_t length;
      uint16_t checksum;
    };
  
    /** Full UDP Header with all sub-headers */
    struct full_header{
      Ethernet::header eth_hdr;
      IP4::ip_header ip_hdr;
      udp_header udp_hdr;
    }__attribute__((packed));
    
    ////////////////////////////////////////////
    
    inline addr_t local_ip() const
    {
      return _local_ip;
    }
    
    /** Input from network layer */
    int bottom(Packet_ptr pckt);
  
    /** Delegate output to network layer */
    inline void set_network_out(downstream del)
    {
      _network_layer_out = del;
    }
    
    /** Send UDP datagram from source ip/port to destination ip/port. 
      
        @param sip   Local IP-address
        @param sport Local port
        @param dip   Remote IP-address
        @param dport Remote port   */
    int transmit(std::shared_ptr<PacketUDP> udp);
  
    UDP(Inet<LinkLayer,IP4>&);
    
    // the UDP::Socket class
    using Socket = SocketUDP;
    friend class SocketUDP;
    
    //! @param port local port
    Socket& bind(port port);
    
  private: 
    downstream _network_layer_out;
    std::map<port, Socket> ports;
    addr_t _local_ip;
  };

}

#include "udp.inl"
#include "packet_udp.hpp"
#include "udp_socket.hpp"

#endif