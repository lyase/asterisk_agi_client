/// Makes asterisk answer the phone
#pragma once

#include "base.hpp"
#include <sstream>

namespace agi_proxy {
namespace command {
/*! A Answer   class
* \details This class is a command handler for asterisk AGI
*once executed within an asterisk context (stdin....)
* it will Answer the incomming call

*unless interupted by user key # on phone
*asterisk will then reply
*Returns:
*failure: 200 result=-1 =>throws badresults
*success: 200 result=0

* \author Lyase damasked from wt/dbo/tutorial.html
* \version 4.1a
* \date 1990-2014
* \pre First initialize the connection to asterisk.
* \bug .
*
*/
/*! \brief  this class defines a proxy to asterisk commands
*/
class Answer : public Base {
public:
     Answer(Proxy& proxy) : Base(proxy) {}
protected:
     virtual const char* commandText() {
          return "ANSWER";
     }
     virtual void checkResult() {
          int result = getResult();
          if (result == -1 )  {
               std::stringstream msg;
               msg << "CHANNEL STATUS command needs a 0 result  and "
                   <<  ", but got " << result << " instead";
               throw err::BadResult(msg.str());
          } else {
          }
     }
};

}
}

