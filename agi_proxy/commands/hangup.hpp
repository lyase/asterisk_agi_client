/// Makes asterisk answer the phone
#pragma once

#include "base.hpp"
#include <sstream>

namespace agi_proxy {
namespace command {
/*! A HangUp   class
* \details This class is a command handler for asterisk AGI
*once executed within an asterisk context (stdin....)
* it will HangUp the incomming call current channel

*unless interupted by user key # on phone
*asterisk will then reply
*Returns:
*failure: 200 result=-1 =>throws badresults
success: 200 result=1

* \author Lyase damasked from wt/dbo/tutorial.html
* \version 4.1a
* \date 1990-2014
* \pre First initialize the connection to asterisk.
* \bug .
*
*/
/*! \brief  this class defines a proxy to asterisk commands
*/
class HangUp: public Base {
public:
     enum Status {
          downAvailable=0, // Channel is down and available
          downReserved=1,  // Channel is down, but reserved
          offHook=2,       // Channel is off hook
          dialling=3,      // Digits (or equivalent) have been dialed
          ringing=4,       // Line is ringing
          remoteRinging=5, // Remote end is ringing
          up=6,            // Line is up
          busy=7,          // Line is busy
          LAST=busy
     };
private:
     std::string _channelName;
public:
     HangUp(Proxy& proxy, const std::string& channelName="") :  Base(proxy), _channelName(channelName) {}
     const std::string& channelName() const {
          return _channelName;
     }
     Status status() const {
          return (Status)result();
     }
protected:
     virtual void checkResult() {
          int result = getResult();
          if ((result == -1) || (result > Status::LAST))  {
               std::stringstream msg;
               msg << "CHANNEL STATUS command needs a result betweeen 0 and "
                   << Status::LAST << ", but got " << result << " instead";
               throw err::BadResult(msg.str());
          }
     }
     virtual const char* commandText() {
          return "HANGUP";
     }
     virtual void addParams(std::ostream& out) {
          if (!_channelName.empty())
               out << ' ' << _channelName;
     }
};

}
}

