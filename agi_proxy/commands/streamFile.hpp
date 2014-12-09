
#pragma once

#include "base.hpp"
namespace agi_proxy {
namespace command {
/*! A StreamFile   class
* \details This class is a command handler for asterisk AGI
*once executed within an asterisk context (stdin....)
* it will  server play a sound file
*return:
**failure: 200 result=-1 =>throws badresults
*success: 200 result=0
*digit pressed: 200 result=<digit> =>throws userinterupt lastresult holds then digit
*make sound file from mp3:
 sox ABBA.mp3 -r 8k -c 1 -e gsm testingagi.gsm
* install sound file:
cp testingagi.gsm /usr/share/asterisk/sounds/
chown asterisk:asterisk /usr/share/asterisk/sounds/testingagi.gsm
chmod 640  /usr/share/asterisk/sounds/testagi.gsm
 make video files:
 ffmpeg -i miniatel.avi -s qcif -vcodec h263 -acodec aac -ac 1 -ar 8000 -r 25 -ab 32 -y -strict experimental testingagi.3gp
  avconv  -i ~/zombies.avi  -s qcif -vcodec h263 -acodec aac -ac 1 -ar 8000 -r 25 -ab 32 -y -strict experimental testingagi.3gp
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
     virtual void checkResult() {
          int result = getResult();
          switch(result) {
          case -1: {
               std::stringstream msg;
               msg << "CHANNEL STATUS command needs a 0 result  and "
                   <<  ", but got " << result << " instead";
               throw err::BadResult(msg.str());
          };
          case 0:
          {}
          break ;
          case 35:
          {}
          break ;
          default: {
               throw err::userinterupt();
               _lastresult=result;

          }
          }
     }
private:
     std::string _soundFileName;
     int _lastresult;
};

}
}

