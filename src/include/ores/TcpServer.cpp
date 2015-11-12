/************************************************************************************

 Authors     :   Ritwik Ummalaneni <ritwik1993@gmail.com>
 Copyright   :   Copyright Ritwik Ummalaneni. All Rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 ************************************************************************************/
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
void TcpServer::Read_Handler(const boost::system::error_code& ec,
				std::size_t bytes_transferred){
  if (!ec)
    {
      std::cout << "Just Read " << std::endl;
    }
}

void TcpServer::Accept_Handler(const boost::system::error_code& ec){
  if (!ec)
    {
      std::cout << "Accepted a connection! - Now switching to write mode " << std::endl;
      connectMode = 1;
    }
}

void TcpServer::Write_Data(){
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

void TcpServer::Read_Data(){
  if (connectMode){
     boost::asio::streambuf input_buffer_;
     async_read_until(socket, input_buffer_, '\n', boost::bind(&TcpServer::Read_Handler, this,
						  placeholders::error,
							placeholders::bytes_transferred));
     std::string line;
     std::istream is(&input_buffer_);
     std::getline(is, line);
     if (!line.empty())
       {
     std::cout << "Recieved: " << line << std::endl;
       }
  }
}

TcpServer::~TcpServer(){
  svc.stop();
}
