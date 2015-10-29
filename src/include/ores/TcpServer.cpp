#include "Common.h"

using namespace boost::asio;
using namespace boost::asio::ip;


TcpServer::TcpServer(unsigned short port = 1700)
  : ipPort(port){
  tcp::acceptor acc(svc, tcp::endpoint(tcp::v4(), ipPort));
  acc.listen();
  acc.async_accept(socket, boost::bind(&TcpServer::Accept_Handler, this, placeholders::error));
  SAY("Waiting for a New connection");
  svc.run();
}

void TcpServer::Write_Handler(const boost::system::error_code& ec,
				std::size_t bytes_transferred){
  if (!ec)
    {
      std::cout << "Just sent " << yawData << std::endl;
    }
}

void TcpServer::Accept_Handler(const boost::system::error_code& ec){
  if (!ec)
    {
      std::cout << "Accepted a connection! - Now switching to write mode " << std::endl;
      connectMode = 1;
    }
}

void TcpServer::WriteData(){
  if (connectMode){
    SAY("Sent data");
    std::ostringstream ss;
    std::string sendBuffer;
    ss << std::fixed << std::setprecision(2);
    ss << yawData;
    sendBuffer = ss.str() + "\r";
    async_write(socket, buffer(sendBuffer), boost::bind(&TcpServer::Write_Handler, this,
						  placeholders::error,
						   placeholders::bytes_transferred));
    }
}


void TcpServer::UpdateYaw(double data) {
  yawData = data;
}  

TcpServer::~TcpServer(){
  svc.stop();
}
