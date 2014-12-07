/* Makes the agi server play a sound file return:200 result=0
unless user hit # then return:
200 result=<digit> Digi=ascii(of key pressed" 35 in our case.
make sound file:
 sox ABBA.mp3 -r 8k -c 1 -e gsm testingagi.gsm
cp testingagi.gsm /usr/share/asterisk/sounds/
chown asterisk:asterisk /usr/share/asterisk/sounds/testingagi.gsm
chmod 640  /usr/share/asterisk/sounds/testagi.gsm
 make video files:
 ffmpeg -i miniatel.avi -s qcif -vcodec h263 -acodec aac -ac 1 -ar 8000 -r 25 -ab 32 -y -strict experimental testingagi.3gp
  avconv  -i ~/zombies.avi  -s qcif -vcodec h263 -acodec aac -ac 1 -ar 8000 -r 25 -ab 32 -y -strict experimental testingagi.3gp
*/
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
          out << ' ' << _soundFileName<<"  # ";

     }
private:
     std::string _soundFileName;
};

}
}

