/** \file  /usr/share/asterisk/agi-bin/hellowithClass.cpp
*    Description:  demo code for a call handling prg for asterisk basic agi functions
the  prg sends "orders to asterisk by writting AGI COMMANDS STREAM FILE, SEND IMAGE.... on the std ouput 
asterisk will then reply on stdin of the prg  if command was succefull or failed see fct checkresult() has to be called after each call to asterisk 
 Usage: 
- Create an AGI  in /var/lib/asterisk/agi-bin, i.e.: compile with g++ hellowithClass.cpp  -o testagi 
or in: /usr/share/asterisk/agi-bin
- set permission: chown asterisk:asterisk /var/lib/asterisk/agi-bin/testagi
- Call using EAGI from your dialplan: exten => 100,1,EAGI(restagi) 

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
#include <iostream> 
#include <string> 
using namespace std ;

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
 Agi_env agi;
int tests = 0,
	pass = 0,
	fail =0;

int main () {
int resultcode;
agi.dump_agi();
// command to play the beep file to current call
resultcode =agi.StreamFile( "beep");

// command to send text msg to current call

resultcode =resultcode =agi.SendText( "hello world");

// command to send an image  to current call

fflush(stdout);
resultcode =resultcode =agi.SendImage( "asterisk-image");
// command to read number to  current call
fprintf( stderr, "4. Testing 'saynumber'...\n");
resultcode =resultcode =agi.SayNumber( 192837466);
// command to read user input will return ascii code of first digit enteredby  current call
fprintf( stderr, "5. Testing 'waitdtmf'...\n");

resultcode =resultcode =agi.GetDigit( 1000);
// command to record sound message of 30s or until user hits keyboard in  current call
//default location for file is usr/share/asterisk/sounds/testagi.gsm
// [Dec  2 06:18:23] WARNING[12108]: file.c:1160 ast_writefile: Unable to open file /usr/share/asterisk/sounds/testagi.gsm: Permission denied

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
}

void Agi_env::dump_agi(){
fprintf(stderr,"agi_request: %s\n",agi.request);
fprintf(stderr,"agi_channel: %s\n",agi.channel);
fprintf(stderr,"agi_lang: %s\n",agi.lang);
fprintf(stderr,"agi_type: %s\n",agi.type);
fprintf(stderr,"agi_callerid: %s\n",agi.callerid);
fprintf(stderr,"agi_dnid: %s\n",agi.dnid);
fprintf(stderr,"agi_context: %s\n",agi.context);
fprintf(stderr,"agi_extension: %s\n",agi.extension);
fprintf(stderr,"agi_priority: %d\n",agi.priority);
}

void Agi_env::init_agi()   /*! \fn init_agi()
    * \brief constructor
    * to be used as to empty  asterisk comunication buffer \n
    * and parse the string to get incoming call  parameters
    */{
char line[200]; // big just incase
char *name,*value,*temp;
while(fgets(line,200,stdin)){
if(!strncmp(line,"agi_",4)){
	// lets parse this line
	if(name = strtok(line,":")){
		value = strtok(NULL,"\n"); // grab remaining minus the newline
		value++; // an attempt to get rid of the leading space. 
		if(!strncmp(name,"agi_request",11)){
			strncpy(agi.request,value,100);
			}
		if(!strncmp(name,"agi_channel",11)){
			strncpy(agi.channel,value,80);
			}
		if(!strncmp(name,"agi_language",12)){
			strncpy(agi.lang,value,20);
			}
		if(!strncmp(name,"agi_type",8)){
			strncpy(agi.type,value,100);
			}
		if(!strncmp(name,"agi_callerid",12)){
			strncpy(agi.callerid,value,100);
			}
		if(!strncmp(name,"agi_dnid",8)){
			strncpy(agi.dnid,value,100);
			}
		if(!strncmp(name,"agi_context",8)){
			strncpy(agi.context,value,80);
			}
		if(!strncmp(name,"agi_extension",8)){
			strncpy(agi.extension,value,80);
			}
		if(!strncmp(name,"agi_priority",8)){
			agi.priority = atoi(value);
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
*/
{
char *parts;
char result[200]; 
int code;
if(!fgets(result,200,stdin)){
	exit(0); // should only get here on error or EOF we could not read in stdin a reply from asterisk at least 200char
	}
tests++; // we got a message back a command was run
if(!strncmp(result,"200",3)){ 
	fail++;
	return 0;
	}else{
		pass++;
		if(parts = strtok(result,"=")){
			if(parts = strtok(NULL,"\n")){ // grab remaining minus the newline
				code = atoi(parts);
				}
			else{
				exit(0); // means we didn't get a result code
				}
			}else{
				exit(0); // means strtok failed originally
				}
			}
	return code;
}
Agi_env::Agi_env()
{
  init_agi() ;
}
int Agi_env::StreamFile(string fileName)
{
fprintf( stderr, "1. Testing 'sendfile'...\n");
cout <<"STREAM FILE "+fileName+" \"\"\n";
fflush(stdout);
return checkresult();
}
int Agi_env::SendText(string themsg)
{
  cout <<"SEND TEXT \" "+themsg+"\"\n";
  fflush(stdout);
return checkresult();
}
int Agi_env::SendImage(string theimageName)
{

  cout <<"SEND IMAGE \" "+theimageName+"\"\n";
  fflush(stdout);
return checkresult();
}
int Agi_env::SayNumber(int thenumber)
{
  cout<<"SAY NUMBER "<<thenumber << "\"\"\n";
fflush(stdout);
return checkresult();
}
int Agi_env::GetDigit(int timeout)
{
  cout<<"WAIT FOR DIGIT "<<timeout << "\"\"\n";
fflush(stdout);
return checkresult();
}