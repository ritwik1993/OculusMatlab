#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <sstream>
#include <iomanip>

using namespace boost::asio;
using namespace boost::asio::ip;

class TcpServer {
 private:
  io_service svc;  
  tcp::socket socket{svc};
  double yawData = 0;
  unsigned short ipPort;
  bool connectMode = 0;
  void Write_Handler(const boost::system::error_code&,
		     std::size_t);
  void Accept_Handler(const boost::system::error_code&);
 public:
  TcpServer(unsigned short );
  void WriteData();
  void UpdateYaw(double);
  ~TcpServer();
};
