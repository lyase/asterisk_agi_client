/** \file  /usr/share/asterisk/agi-bin/hellowithClass.cpp
*    Description:  demo code for a call handling prg for asterisk basic agi functions
the  prg sends "orders to asterisk by writting AGI COMMANDS STREAM FILE, SEND IMAGE.... on the std ouput 
asterisk will then reply on stdin of the prg  if command was succefull or failed see fct checkresult() has to be called after each call to asterisk 
 Usage: 
- Create an AGI  in /var/lib/asterisk/agi-bin, i.e.: compile with g++ hellowithClass.cpp  -o testagi 

or in: /usr/share/asterisk/agi-bin
- set permission: chown asterisk:asterisk /var/lib/asterisk/agi-bin/testagi
rm -rvf build/ ; mkdir build ; cd build ; cmake .. ; make testagiwithClass ; cd .. ; cp build/testagiwithClass ./testagi ; chown asterisk:asterisk ./testagi
rm /tmp/agireports.txt ;rm -rvf build/ ; mkdir build ; cd build ; cmake .. ; make testagiwithClass ; cd .. ; cp build/testagiwithClass ./testagi ; chown asterisk:asterisk ./testagi

./testagi<inputs
- Call using EAGI from your dialplan: exten => 100,1,EAGI(restagi) 
you can debug this agi script:
http://astbook.asteriskdocs.org/en/2nd_Edition/asterisk-book-html-chunk/asterisk-CHP-9-SECT-5.html
Debugging from the Operating System

As mentioned above, you should be able to run your program directly from the operating system to see how it behaves. The secret here is to act just like Asterisk does, providing your script with the following:

    A list of variables and their values, such as agi_test:1.

    A blank line feed (\n) to indicate that you’re done passing variables.

    Responses to each of the AGI commands from your AGI script. Usually, typing 200 response=1 is sufficient.

Trying your program directly from the operating system may help you to more easily spot bugs in your program.
you can automate this runing ./testagi <inputs
debian-asterisk:/usr/share/asterisk/agi-bin# cat inputs 
 agi_test:1

200 response=1


debian-asterisk:/usr/share/asterisk/agi-bin# 
 *\version  1.0
 *\date 01/12/12 10:06:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *\author:  Lyase Damasked
 *        Company:
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <iostream> 
#include <iostream>
#include <fstream>
#include <string> 
using namespace std ;
struct APIException : logic_error {
    static string code2msg(unsigned short code) {
        // TODO: Could copy some of the asterisk documentation in here,
        // eg. 200 = OK .. etc.
        stringstream s;
        s << code;
        return s.str();
    }
    APIException(unsigned short code) : logic_error(code2msg(code)) { }
};

struct ParseError : logic_error {
    explicit ParseError(const std::string& what) : logic_error(what) {}
};
class Agi_env 
/*! \class Agi_env
* \brief  this class represent aproxy to a managed call you can interact with the user 
* thru predefined function of this class
 
*/
{
public:
Agi_env();
void init_agi();
void dump_agi();
int checkresult();
int StreamFile(string);
int GetDigit(int );
/*! \fn int StreamFile(string in-Arg1)
* \brief 
*   @param[in]     in-Arg1 the name of File to be streamed to user.
* will ask the server to stream to call the sound file* \n
  @return status of command into asterisk or throw exeption if asterisk didn't reply
*/
int SendText(string);
int SendImage(string );
int SayNumber(int );
int hangup();
int Getnumber3Digit(int /*timeout*/);
int recordFile(string, string, int , int );
/*! \fn intrecordFile(string in-Arg1)
  * \brief asks asterisk to record call into file 

int Agi_env::recordfile(string fileName,string format, int maxtime, int escseq)
  *   @param[in]     in-Arg1 the name of File to be streamed to user.
  * will ask the server to stream to call the sound file* \n
    @return status of command into asterisk or throw exeption if asterisk didn't reply
  */

private:
		char request[100]; 	// guessing here
		char channel[80]; 	// from definition in channels.h
		char lang[20];		// from definition in channels.h
		char type[100];		// guessing
		char callerid[100];	// guessing
		char dnid[100];		// guessing
		char context[80];	// from definition in channels.h
		char extension[80];	// from definition in channels.h
		int priority;		// from definition in channels.h
	};

using namespace std;
int tests = 0,
	pass = 0,
	fail =0;
ofstream myfile;

int main () {
int resultcode=0;
Agi_env agi;
agi.dump_agi();
myfile.open ("/tmp/agireports.txt");
myfile << " logging to global file "<<resultcode <<".\n"<<flush;

fprintf( stderr, "4. Testing 'saynumber'...\n");
resultcode=agi.SayNumber(192837466);
 myfile << "I  said a number the asterisk replied  result is:\""<<resultcode <<"\".\n if 0 maybe the initial value not returned code from server\n"<<flush;

/*
// command to send text msg to current call
resultcode =agi.SendText( "hello world");

// command to send an image  to current call
resultcode =agi.SendImage( "asterisk-image");
// command to read number to  current call

// command to read user input will return ascii code of first digit enteredby  current call
fprintf( stderr, "5. Testing 'waitdtmf'...\n");
*/
resultcode =agi.GetDigit( -1);
myfile << "reading dtmf I got from dtmf following code:"<<resultcode <<".\n"<<flush;


fprintf( stderr, "6. Testing playback...\n");
resultcode =agi.StreamFile("beep");
resultcode=agi.Getnumber3Digit(-1);
resultcode=agi.SayNumber(resultcode);
resultcode =agi.StreamFile("beep");
resultcode =agi.StreamFile("/tmp/testagi");
// command to record sound message of 30s or until user hits keyboard in  current call
//default location for file is usr/share/asterisk/sounds/testagi.gsm
// [Dec  2 06:18:23] WARNING[12108]: file.c:1160 ast_writefile: Unable to open file /usr/share/asterisk/sounds/testagi.gsm: Permission denied
//change volume sox -v 2.0 /tmp/testagi.gsm testagi.gsm


fprintf( stderr, "6. Testing 'record'...\n");
printf("RECORD FILE /tmp/testagi gsm 1234 3000\n");
fflush(stdout);
resultcode = agi.checkresult();
// command to playback sound file to  current call
fprintf( stderr, "6a. Testing 'record' playback...\n");
printf("STREAM FILE /tmp/testagi \"\"\n");
fflush(stdout);
resultcode = agi.checkresult();
fprintf(stderr,"================== Complete ======================\n");
fprintf(stderr,"%d tests completed, %d passed, %d failed\n",tests,pass,fail);
fprintf(stderr,"==================================================\n");
agi.hangup();
myfile.close();
exit(0);
}

void Agi_env::dump_agi(){
fprintf(stderr,"agi_request: %s\n",request);
fprintf(stderr,"agi_channel: %s\n",channel);
fprintf(stderr,"agi_lang: %s\n",lang);
fprintf(stderr,"agi_type: %s\n",type);
fprintf(stderr,"agi_callerid: %s\n",callerid);
fprintf(stderr,"agi_dnid: %s\n",dnid);
fprintf(stderr,"agi_context: %s\n",context);
fprintf(stderr,"agi_extension: %s\n",extension);
fprintf(stderr,"agi_priority: %d\n",priority);
}

void Agi_env::init_agi()   
    /*! \fn init_agi()
    * \brief constructor
    * to be used as to empty  asterisk comunication buffer \n
    * and parse the string to get incoming call  parameters
    */{
char line[200]; // big just incase
char *name,*value;
while(fgets(line,200,stdin)){
if(!strncmp(line,"agi_",4)){
	// lets parse this line
	if(name = strtok(line,":")){
		value = strtok(NULL,"\n"); // grab remaining minus the newline
		value++; // an attempt to get rid of the leading space. 
		if(!strncmp(name,"agi_request",11)){
			strncpy(request,value,100);
			}
		if(!strncmp(name,"agi_channel",11)){
			strncpy(channel,value,80);
			}
		if(!strncmp(name,"agi_language",12)){
			strncpy(lang,value,20);
			}
		if(!strncmp(name,"agi_type",8)){
			strncpy(type,value,100);
			}
		if(!strncmp(name,"agi_callerid",12)){
			strncpy(callerid,value,100);
			}
		if(!strncmp(name,"agi_dnid",8)){
			strncpy(dnid,value,100);
			}
		if(!strncmp(name,"agi_context",8)){
			strncpy(context,value,80);
			}
		if(!strncmp(name,"agi_extension",8)){
			strncpy(extension,value,80);
			}
		if(!strncmp(name,"agi_priority",8)){
			priority = atoi(value);
			}
		}
	}else{
		break;
		}
}
}
						
int Agi_env::checkresult ()
/*! \fn checkresult()
* \brief 
* you should call this to check asterisk replies after each command send to asterisk * \n
* parsing the incoming string will report status of command into asterisk
usual results: "200 result=0
c�▒�N��䉿��T�▒��c��䉿z�T���c�`w�▒�ov���c���c��䉿`
                                                 T���c��ov��剿��p���c��ov���p���v�0�ov���p���v��Pv��ov���p�d剿F�T��ov�0��c���p�".

*/
{
char *parts;
char result[200]; 
int code;
if(!fgets(result,200,stdin)){
	exit(0); // should only get here on error or EOF we could not read in stdin a reply from asterisk at least 200char
	}
tests++; // we got a message back a command was run
string resultstr;
for(int i = 0; i != 200; i++)
   resultstr += result[i];
myfile << "\n I got following string back :\""<<resultstr <<"\".\n"<<flush;
if(strncmp(result,"200",3)){ 
	fail++;
myfile<<" \n I fail236 existing\n";
	return 0;
	}else{
		pass++;
		if(parts = strtok(result,"=")){
			if(parts = strtok(NULL,"\n")){ // grab remaining minus the newline
				code = atoi(parts);
				}
			else{
myfile<<" \n I fail245 existing\n";
				exit(0); // means we didn't get a result code
				}
			}else{
myfile<<" \n I fail249 existing\n";
				exit(0); // means strtok failed originally
				}
			}
myfile << "\n I parsed and found result code is :\""<<code <<"\".\n"<<flush;
	return code;
}
Agi_env::Agi_env()
{
  init_agi() ;
}
int Agi_env::StreamFile(string fileName)
{
fprintf( stderr, "1. Testing 'sendfile'...\n");
cout <<"STREAM FILE "+fileName+" \"\"\n"<<flush ;
return checkresult();
}
int Agi_env::SendText(string themsg)
{
  cout <<"SEND TEXT \" "+themsg+" \"\n"<< flush ;
return checkresult();
}
int Agi_env::SendImage(string theimageName)
{

  cout <<"SEND IMAGE \" "+theimageName+" \"\n"<<flush ;
return checkresult();
}
int Agi_env::SayNumber(int thenumber)
{
  cout<<"SAY NUMBER "<<thenumber << " \"\"\n"<<flush ;
return checkresult();
}
int Agi_env::GetDigit(int timeout)
{
  cout<<"WAIT FOR DIGIT "<<timeout << "\n"<<flush ;
return checkresult()-48;
}
int Agi_env::Getnumber3Digit(int timeout)
{
int a,b,c, number;
a=GetDigit(timeout);
b=GetDigit(timeout);
c=GetDigit(timeout);

number=100*a+10*b+ c;
return number ;
}
int Agi_env::hangup()
{
  cout<<"HANGUP "<< " \"\"\n"<<flush ;
return checkresult();
}
int Agi_env::recordFile(string fileName,string format, int maxtime, int escseq)
{
printf("RECORD FILE /tmp/testagi gsm 1234 3000\n");
cout<<"RECORD FILE"<<fileName<<format<<escseq <<" "<<maxtime <<" \n";
return checkresult();
}
