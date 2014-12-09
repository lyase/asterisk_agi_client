/// Makes the agi server say a number
#pragma once

#include "base.hpp"
#include "base.hpp"
#include <sstream>
namespace agi_proxy {
namespace command {
/*! A SayNumber   class
* \details This class is a command handler for asterisk AGI
*once executed within an asterisk context (stdin....)
* it will read the number
*in the voice chanel.
*unless interupted by user key # on phone
*asterisk will then reply
*Returns:
*failure: 200 result=-1 =>throws badresults
*success: 200 result=0
*digit pressed: 200 result=<digit> =>throws userinterupt lastresult holds then digit

* \author Lyase damasked from wt/dbo/tutorial.html
* \version 4.1a
* \date 1990-2014
* \pre First initialize the connection to asterisk.
* \bug .
*
*/
/*! \brief  this class defines a proxy to asterisk commands
*/
class SayNumber: public Base {
public:
     SayNumber(Proxy& proxy, int number) :  Base(proxy), _number(number) {}
     int number() const {
          return _number;
     }
     int lastresult() const {
          return _lastresult;
     }
protected:
     virtual const char* commandText() {
          return "SAY NUMBER";
     }
     virtual void addParams(std::ostream& out) {
          out << ' ' << _number << " \" # \"";
     }
     virtual void checkResult() {
          int result = getResult();
          switch(result) {
          case -1: {
               std::stringstream msg;
               msg << "CHANNEL STATUS command needs a 0 result  and "
                   <<  ", but got " << result << " instead";
               throw err::BadResult(msg.str());
          };
          case 0: {
          };
          default: {
//               throw err::userinterupt();
               _lastresult=result;

          }
          }
     }
private:
     int _number;
     int _lastresult;



};
}
}
