#include "AGI.hpp"
#include <sstream>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

const std::map<Code, std::string> BadCode::code2msg = {
    {510, "Invalid or unknown command"},
    {511, "Command Not Permitted on a dead channel"},
    {520, "End of proper usage"}
};



int AsteriskCallProxy::getResult() {
    /// Get's the code for a response and fast forwards the stream
    /// to the end of relult=
    // Multiline response looks like:
    // 200 result=0- name="x"
    // age=12
    // 200
    // Single line response is just
    // code result=0 [data]
    log << "getting result: ";
    Code code;
    in >> code >> std::ws;
    log << code << std::endl << std::flush;
    if (code != 200)
        throw BadCode(code);
    std::string resultLiteral;
    std::getline(in, resultLiteral, '=');
    if (resultLiteral != "result") {
        std::stringstream msg;
        msg << "Expected to find 'result' after code but got '"
            << resultLiteral << "' instead";
        throw BadParse(msg.str());
    }
    int result;
    in >> result;
    return result;
}

void AsteriskCallProxy::readConfig() {
    log << "Reading Config..." << std::endl << std::flush;
    static std::unordered_map<std::string, std::string*>
        settings {
            {"agi_request", &_config.request},
            {"agi_channel", &_config.channel},
            {"agi_language", &_config.language},
            {"agi_type", &_config.type},
            {"agi_uniqueid", &_config.uniqueid},
            {"agi_version", &_config.version},
            {"agi_callerid", &_config.callerid},
            {"agi_calleridname", &_config.calleridname},
            {"agi_callingpres", &_config.callingpres},
            {"agi_callingani2", &_config.callingani2},
            {"agi_callington", &_config.callington},
            {"agi_callingtns", &_config.callingtns},
            {"agi_dnid", &_config.dnid},
            {"agi_rdnis", &_config.rdnis},
            {"agi_context", &_config.context},
            {"agi_extension", &_config.extension},
            {"agi_priority", &_config.priority},
            {"agi_enhanced", &_config.enhanced},
            {"agi_accountcode", &_config.accountcode},
            {"agi_threadid", &_config.threadid}
        };
    while (true) {
        // Read in a line
        std::string line1;
        std::getline(in, line1);
        // Break on a blank lene
        if (line1.empty())
            break; 
        std::stringstream line(line1);
        // Read in the config item name
        std::string name;
        std::getline(line, name, ':');
        // Find which config storage operation to perform
        auto setting = settings.find(name);
        if (setting != settings.end()) {
            log << "Reading setting for '" << name << "'..." << std::flush;
            line >> *setting->second;
            log << "'" << *setting->second << "'" << std::endl << std::flush;
        } else {
            log << "Could not find setting for " << name << std::endl << std::flush;
            std::stringstream msg;
            msg << "No setting found for '"
                << name << "'";
            throw BadParse(msg.str());
        }
    }
}

void AsteriskCallProxy::answer() {
    _nbrcommandsent++;
    log << "Sending 'answer' command..." << std::flush;
    out << "ANSWER\n";
    int result = getResult();
    log << "Result: " << result << std::endl << std::flush;
    if (result != 0) {
        std::stringstream msg;
        msg << "ANSWER command needs a result of 0, but got: " << result;
        throw BadResult(msg.str());
    }
}
AsteriskCallProxy::ChannelStatus AsteriskCallProxy::currentchannelStatus() {

    return (ChannelStatus) channelStatus("");
}
AsteriskCallProxy::ChannelStatus AsteriskCallProxy::channelStatus(const std::string& channelName) {
    log << "CHANNEL STATUS " << channelName << "..." << std::flush;
    out << "CHANNEL STATUS " << channelName << '\n';
    int result = getResult();
    log << "Result: " << result << std::endl << std::flush;
    if ((result == -1) || (result > ChannelStatus::LAST))  {
        std::stringstream msg;
        msg << "CHANNEL STATUS command needs a result betweeen 0 and "
            << ChannelStatus::LAST << ", but got " << result << " instead";
        throw BadResult(msg.str());
    }
    return (ChannelStatus) result;
}
int AsteriskCallProxy::StreamFile(string fileName)
{
    _nbrcommandsent++;
log<< "\n streaming file "<<fileName<<".\n"<<flush;
out <<"STREAM FILE "+fileName+" \"\"\n"<<flush ;
return getResult();
}

Digit AsteriskCallProxy::controlStreamFile(
    const std::string& filename,
    const std::set<Digit>& digits,
    int skipms,
    char ffchar,
    char rewchar,
    char pausechar
) {
    /**
     * control stream file
     *
     * Usage: CONTROL STREAM FILE <filename> <escape digits> [skipms] [ffchar] [rewchr] [pausechr]
     *
     * Send the given file, allowing playback to be controled by the given digits, if any.
     *
     * Use double quotes for the digits if you wish none to be permitted.
     *
     * If <skipms> is provided then the audio will seek to sample offset before play starts.
     *
     * <ffchar> and <rewchar? default to * and # respectively.
     *
     * Remember, the file extension must not be included in the filename.
     *
     * Returns:
     * failure: 200 result=-1
     * failure on open: 200 result=0
     * success: 200 result=0
     * digit pressed: 200 result=<digit>
     *
     * <digit> is the ascii code for the digit pressed.
     *
     * NOTE: Unlike STREAM FILE, CONTROL STREAM FILE doesn't return the stream position when streaming stopped ('endpos')
     *
     **/
    out << "CONTROL STREAM FILE " << filename << ' ';
    log << "CONTROL STREAM FILE " << filename << ' ';
    if (!digits.empty())
        for (auto digit : digits)
            if (digit < 10)
                // Turn low number digits into printable chars
                out << int(digit);
            else
                // Assume it's already a printable char
                out << digit;
    else
        out << R"("")";
    // If for example ffchar is non-default, we have to push out all the args before it (skipms)
    int extraArgs = 0;
    // See how many extra args we must supply
    if ((pausechar != 0))
        extraArgs = 4;
    else if ((rewchar != '#'))
        extraArgs = 3;
    else if ((ffchar != '*'))
        extraArgs = 2;
    else if ((skipms > 0))
        extraArgs = 1;
    // Push out extra args
    if (extraArgs--) {
        log << ' ' << skipms;
        out << ' ' << skipms;
    } if (extraArgs--) {
        log << ' ' << ffchar;
        out << ' ' << ffchar;
    } if (extraArgs--) {
        log << ' ' << rewchar;
        out << ' ' << rewchar;
    } if (extraArgs--) {
        log << ' ' << pausechar;
        out << ' ' << pausechar;
    }
    log << "..." << std::flush;
    out << '\n';
    // Read the reply
    int result = getResult();
    log << "Result: " << result << std::endl << std::flush;
    if (result == -1)
        throw BadResult("CONTROL STREAM FILE got result of -1");
    else
        return result;
}

void AsteriskCallProxy::databaseDel(const std::string& family, const std::string& key) {
    _nbrcommandsent++;
    log << "DATABASE DEL " << family << ' ' << key << "..." << std::flush;
    out << "DATABASE DEL " << family << ' ' << key << '\n';
    int result = getResult();
    log << "Result: " << result << std::endl << std::flush;
    if (result != 0) {
        std::stringstream msg;
        msg << "DATABASE DEL " << family << ' ' << key << " needs a result of 0, but got: " << result;
        throw BadResult(msg.str());
    }
}
int AsteriskCallProxy::recordFile(string fileName,string format, int maxtime, int escseq)
{
    _nbrcommandsent++;
printf("RECORD FILE /tmp/testagi gsm 1234 3000\n");
out<<"RECORD FILE"<<fileName<<format<<escseq <<" "<<maxtime <<" \n";
return getResult();
}
int AsteriskCallProxy::SayNumber(int thenumber)
{
log<< "\n send asterisk a saynumber command: "<<thenumber<<".\n"<<flush;
  out<<"SAY NUMBER "<<thenumber << " \"\"\n"<<flush ;
return getResult();
sleep(3);// wait so asterisk finish the command
}
int AsteriskCallProxy::GetDigit(int timeout)
{
log<< "\n send asterisk agetdit  with timeout: "<<timeout<<".\n"<<flush;

  out<<"WAIT FOR DIGIT "<<timeout << "\n"<<flush ;
return getResult()-48;
}
int AsteriskCallProxy::Getnumber3Digit(int timeout)
{
int a,b,c, number;
a=GetDigit(timeout);
b=GetDigit(timeout);
c=GetDigit(timeout);

number=100*a+10*b+ c;
return number ;
}
int AsteriskCallProxy::hangupcurrentchanel()
{
  return (hangupchanel(""));
}
int AsteriskCallProxy::hangupchanel(const std::string& channelName)
{
    _nbrcommandsent++;
    out<< "HANGUP "<< channelName <<" \n"<<flush ;
    int result = getResult();
    if (result == -1)
        throw BadResult("CONTROL STREAM FILE got result of -1");
    else
        return result;

return getResult();
}



 ostream& operator <<(std::ostream& os, const AsteriskCallProxy& AsteriskCallProxy) {
     os<<" dump incoming call information";
    os << "agi_request: " << AsteriskCallProxy._config.request << endl;
     os << "agi_channel: " << AsteriskCallProxy._config.channel << endl;
     os << "agi_lang: " << AsteriskCallProxy._config.language << endl;
     os << "agi_type: " << AsteriskCallProxy._config.type << endl;
     os << "agi_callerid: " << AsteriskCallProxy._config.callerid << endl;
     os << "agi_dnid: " << AsteriskCallProxy._config.dnid << endl;
     os << "agi_context: " << AsteriskCallProxy._config.context << endl;
     os << "agi_extension: " << AsteriskCallProxy._config.extension << endl;
     os << "agi_priority: " << AsteriskCallProxy._config.priority << endl;
     os << "I have processed "<< AsteriskCallProxy._nbrcommandsent<<endl;
     os << "I had :'' "<< AsteriskCallProxy._nbrcommandfail<<"'failures so far"<<endl;
    return os;
};
 void Command::Execute(){

cout <<callsrv._nbrcommandok ;

 };
 void CommandSayNumber::Execute(){
// demo using the private variable of the callsrv so we can use it's in out log next and check call setting
cout <<callsrv._nbrcommandok ;
/**
 * control stream file
 *
 * Usage: CONTROL STREAM FILE <filename> <escape digits> [skipms] [ffchar] [rewchr] [pausechr]
 *
 * Send the given file, allowing playback to be controled by the given digits, if any.
 *
 * Use double quotes for the digits if you wish none to be permitted.
 *
 * If <skipms> is provided then the audio will seek to sample offset before play starts.
 *
 * <ffchar> and <rewchar? default to * and # respectively.
 *
 * Remember, the file extension must not be included in the filename.
 *
 * Returns:
 * failure: 200 result=-1
 * failure on open: 200 result=0
 * success: 200 result=0
 * digit pressed: 200 result=<digit>
 *
 * <digit> is the ascii code for the digit pressed.
 *
 * NOTE: Unlike STREAM FILE, CONTROL STREAM FILE doesn't return the stream position when streaming stopped ('endpos')
 *
 **/
callsrv.out <<"SAY NUMBER "<<thenumber << " \"\"\n"<<flush ;
// Read the reply
int result = getResult();
if (result == -1)
    throw BadResult("CONTROL STREAM FILE got result of -1");
else
    return result;

 };
