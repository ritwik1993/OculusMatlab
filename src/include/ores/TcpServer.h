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