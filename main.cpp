#include "agi_proxy/proxy.hpp"
#include "agi_proxy/command_runner.hpp"
#include "agi_proxy/commands/all.hpp"

#include <vector>
#include <fstream>
#include <iostream>

#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>
/** \file  /usr/share/asterisk/agi-bin/main.cpp
  2 *    Description:  demo code for a call handling prg for asterisk basic agi functions
  3 the  prg sends "orders to asterisk by writting AGI COMMANDS STREAM FILE, SEND IMAGE.... on the std ouput
  4 asterisk will then reply on stdin of the prg  if command was succefull or failed see fct checkresult() has to be called after each call to asterisk
  5  Usage:
  6 - Create an AGI  in /var/lib/asterisk/agi-bin, i.e.: compile with g++ main.cpp  -o /var/lib/asterisk/agi-bin/testagi
  7 or in: /usr/share/asterisk/agi-bin
  8 - set permission: chown asterisk:asterisk /var/lib/asterisk/agi-bin/testagi
  9 - Call using EAGI from your dialplan: exten => 100,1,EAGI(restagi)
 10
 11  *\version  1.0
 12  *\date 01/12/12 10:06:50
 13  *       Revision:  none
 14  *       Compiler:  gcc
 15  *
 16  *\author:  Lyase Damasked
 17  *        Company:
 18  *
 19  * =====================================================================================
 20  */

namespace bio = boost::iostreams;

int main(int, char**)
{
     std::ofstream log;
     log.open ("info.log");

     // Logging
     bio::filtering_ostream live;
     live.push(bio::file_sink("live.log", std::ios_base::app));

     // std::out tee
     const bio::tee_filter<std::ostream> teeFilter(live);

     bio::filtering_ostream out;
     out.push(teeFilter);
     out.push(std::cout);

     // std::in tee
     bio::filtering_istream in;
     in.push(teeFilter, 0);
     in.push(std::cin, 0);

     // Make the proxy
     agi_proxy::Proxy p(std::cin, out, log);
     p.readConfig();

     // Set up the commands
     namespace cmd = agi_proxy::command;
     std::vector<agi_proxy::CommandRunner> commands;
     commands.emplace_back(new cmd::Answer(p));

     commands.emplace_back(new cmd::SayNumber(p, 123456789));
     commands.emplace_back(new cmd::ChannelStatus(p));
     commands.emplace_back(new cmd::StreamFile(p, "testagi"));
     commands.emplace_back(new cmd::HangUp(p));
     // Run the commands
     try {
for (auto& command : commands) {
               command();
          }
     } catch (std::exception e) {
          log << "Exception: " << e.what() << std::endl;
     } catch (...) {
          log << "Unkown Exception: " << std::endl;
     }
     return 0;
}
