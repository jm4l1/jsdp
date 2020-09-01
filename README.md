
# JSDP : 

Simple library implementation of Session Description Protocol [RFC 4566 # SDP: Session Description Protocol.]([https://tools.ietf.org/html/rfc8259](https://tools.ietf.org/html/rfc4566)) This library provides parsing and generating of SDP that can be used with other session protocols such as SIP. 

## Supported Operated Systems
jSdp currently support building on Linux and MacOs.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)packages/make.htm)
* gcc/g++ >= 9.3
* Linux: gcc / g++ is installed by default on most Linux distros
* Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
* clang >= 9.3
## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `export CXX=<path_to_g++9> && export CC=<path_to_gcc9> && cmake .. && make`
4. Link generated library to you project

## Examples

```c++
#include "Sdp.h"
....

auto _sdp = Sdp(R"(v=0
o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5
s=SDP Seminar
i=A Seminar on the session description protocol
u=http://www.example.com/seminars/sdp.pdf
e=j.doe@example.com (Jane Doe)
c=IN IP4 224.2.17.12/127
t=2873397496 2873404696
a=recvonly
m=audio 49170 RTP/AVP 0
m=video 51372 RTP/AVP 99
a=rtpmap:99 h263-1998/90000)"
);

if(_sdp.IsValid())
{
    auto m = MediaDescription();
    m.SetMediaDescription("audio 12345 RTP/AVP 0");
    m.SetMediaTitle("meeting stream");
    m.AddMediaConnectionData("IN IP4 224.2.14.12/127");
    m.SetMediaAttribute("recvonly");
    _sdp.AddMediaSession(m);

    std::cout << _sdp.Serialize() ;
    /*
        v=0
        o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5
        s=SDP Seminar
        i=A Seminar on the session description protocol
        u=http://www.example.com/seminars/sdp.pdf
        e=j.doe@example.com (Jane Doe)
        c=IN IP4 224.2.17.12/127
        t=2873397496 2873404696
        a=recvonly
        m=audio 49170 RTP/AVP 0
        m=video 51372 RTP/AVP 99
        a=rtpmap:99 h263-1998/90000
        m=audio 12345 RTP/AVP 0
        i=meeting stream
        c=IN IP4 224.2.14.12/127
        a=recvonly
    */
    std::cout << "version is " << _sdp.GetVersion() << "\n";
    //version is 0
    std::cout << "Session name is " << _sdp.GetSessionName() << "\n";
    //Session name is SDP Seminar
    std::cout << "timing is " << _sdp.GetTiming() << "\n";
    //timing is 2873397496 2873404696
}

else
{
    std::cout << "Sdp is invalid , reason := " << _sdp._invalid_reason << "\n";
}
```