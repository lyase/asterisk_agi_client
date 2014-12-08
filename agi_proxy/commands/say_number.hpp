/// Makes the agi server say a number
#pragma once

#include "base.hpp"
#include "base.hpp"
#include <sstream>
namespace agi_proxy {
namespace command {

class SayNumber: public Base {
public:
     SayNumber(Proxy& proxy, int number) :  Base(proxy), _number(number) {}
     int number() const {
          return _number;
     }
protected:
     virtual const char* commandText() {
          return "SAY NUMBER";
     }
     virtual void addParams(std::ostream& out) {
          out << ' ' << _number << " \"\"";
     }
     virtual void checkResult() {
          int result = getResult();
          if (result != 0 )  {
               std::stringstream msg;
               msg << "CHANNEL STATUS command needs a 0 result  and "
                   <<  ", but got " << result << " instead";
               throw err::BadResult(msg.str());
          }
     }

private:
     int _number;
};

}
}
