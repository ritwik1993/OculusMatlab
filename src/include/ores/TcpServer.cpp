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
      //std::cout << "Just sent " << yawData << std::endl;
    }
}
void TcpServer::Read_Handler(const boost::system::error_code& ec,
				std::size_t bytes_transferred){
  if (!ec)
    {
      std::string line1, line2, line3, line4;
     std::istream is(&input_buffer_);
     std::string test;
     is >> test;
     std::istringstream iss(test);
     getline(iss, line1, ',');
     getline(iss, line2, ',');
     getline(iss, line3, ',');
     getline(iss, line4, ',');
    
     if (!line1.empty() && !line2.empty()
	 && !line3.empty() && !line4.empty())
       {
	 es_dataYaw = std::stof(line1);
	 es_dataPOS = std::stof(line2);
	 es_datasES = std::stof(line3);
	 es_dataDist = std::stof(line4);	 
            }
    }
  else
    std::cout << "Error reading:" << ec.message() << std::endl;}

void TcpServer::Accept_Handler(const boost::system::error_code& ec){
  if (!ec)
    {
      std::cout << "Accepted a connection! - Now switching to write mode " << std::endl;
      connectMode = 1;
    }
}

void TcpServer::Write_Data(){
  if (connectMode){
    //SAY("Send data");
    std::ostream ss(&output_buffer_);
    ss << std::fixed << std::setprecision(2) << yawData <<
      ";" << xData << ";" << yData << "\r";
    async_write(socket, output_buffer_,
		boost::bind(&TcpServer::Write_Handler, this, placeholders::error, placeholders::bytes_transferred));
    svc.reset();
    svc.run();
    }
}


void TcpServer::UpdateYaw(double data) {
  yawData = data;
}

void TcpServer::UpdateX(float data) {
  xData = data;
}

void TcpServer::UpdateY(float data) {
  yData = data;
} 

void TcpServer::Read_Data(){
  if (connectMode){ 
    async_read_until(socket, input_buffer_, "\r" , boost::bind(&TcpServer::Read_Handler, this,
    						  placeholders::error,
							       placeholders::bytes_transferred));
     svc.reset();
    svc.run();
  }
}

float TcpServer::Get_EsDataYaw(){
  return es_dataYaw;
}

float TcpServer::Get_EsDataPOS(){
  return es_dataPOS;
}

float TcpServer::Get_EsDatasES(){
  return es_datasES;
}

float TcpServer::Get_EsDataDist(){
  return es_dataDist;
}

TcpServer::~TcpServer(){
  svc.stop();
}
