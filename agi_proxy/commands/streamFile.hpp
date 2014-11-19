/// Makes the agi server say a number
#pragma once

#include "base.hpp"
namespace agi_proxy {
namespace command {

class StreamFile: public Base {
public:
     StreamFile(Proxy& proxy, std::string soundFileName) :  Base(proxy), _soundFileName(soundFileName) {}
     std::string number() const {
          return _soundFileName;
     }
protected:
     virtual const char* commandText() {
          return "STREAM FILE";
     }
     virtual void addParams(std::ostream& out) {
          out << ' ' << _soundFileName;
     }
private:
     std::string _soundFileName;
};

}
}

