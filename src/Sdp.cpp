#include "Sdp.h"
#include <sstream>
#include <iostream>

SdpDescription::SdpDescription(const std::string& description_string)
{
    std::stringstream description_stream(description_string);
   for(std::string description_line ;  std::getline( description_stream , description_line ) ;)
   {
       char type = description_line[0];
       auto value = std::string(description_line.begin() + 2 , description_line.end());
       SetAttribute(type,value,true);
   }
}
bool SdpDescription::HasAttribute(char type) const
{
    for(auto kv_pair : _description_map)
    {
        if(kv_pair.first == type)
        {
            return true;
        }
    }
    return false ;
}
void SdpDescription::SetAttribute(char type, std::string value, bool allow_multiple)
{ 
    if(allow_multiple)
    {
        _description_map.push_back({type, value});
        return;
    }
    if(!HasAttribute(type))
    {
        _description_map.push_back({type, value});
        return;
    }
    for(auto kv_pair : _description_map)
    {
        if(kv_pair.first == type)
        {
            kv_pair.second = value;
            break;
        }
    }
    return;
};
std::optional<std::string> SdpDescription::GetAttribute(char type) const
{
    if(!HasAttribute(type)){
        return {};
    }
    std::string value;
    for(auto kv_pair : _description_map)
    {
        if(kv_pair.first == type)
        {
            value =  kv_pair.second;
            break;
        }
    }
    return value;
}
std::vector<std::string> SdpDescription::GetAttributes(char type) const
{
    std::vector<std::string> attributes;
    if(!HasAttribute(type)){
        return attributes;
    }
    for(auto kv_pair : _description_map)
    {
        if(kv_pair.first == type)
        {
            attributes.emplace_back(kv_pair.second);
            break;
        }
    }
    return attributes;
}
std::string SdpDescription::Serialize() const
{
    std::stringstream sdp_stream;
    for(auto kv_pair : _description_map)
    {
        sdp_stream << kv_pair.first << "=" << kv_pair.second <<"\n";
    }
    return sdp_stream.str();
}

void MediaDescription::SetMediaDescription(const std::string media_description)
{
    SetAttribute('m' , media_description , true);
}
std::vector<std::string> MediaDescription::GetMediaDescription() const
{
    return GetAttributes('m');
}
void MediaDescription::SetMediaTitle(const std::string media_title)
{
    SetAttribute('i' , media_title , false);
}
std::string MediaDescription::GetMediaTitle() const
{
    return GetAttribute('i').value();
}
void MediaDescription::AddMediaConnectionData(const std::string media_connection_info)
{
    SetAttribute('c' , media_connection_info , true);
}
std::vector<std::string> MediaDescription::GetMediaConnectionData() const
{
    return GetAttributes('c');
}
void MediaDescription::SetMediaBandwidth(const std::string media_bandwidh_info)
{
    SetAttribute('b' , media_bandwidh_info , true);
}
std::vector<std::string> MediaDescription::GetMediaBandwidth() const
{
    return GetAttributes('b');
}
void MediaDescription::SetMediaEncryptionKey(const std::string encryption_key)
{
    SetAttribute('k' , encryption_key , false);
}
std::string MediaDescription::GetMediaEncryptionKey() const
{
    return GetAttribute('k').value();
}
void MediaDescription::SetMediaAttribute(const std::string media_attribute)
{
    SetAttribute('a' , media_attribute , true);
}
std::vector<std::string> MediaDescription::GetMediaAttributes() const
{
    return GetAttributes('a');
}

Sdp::Sdp(const std::string& sdp_string){
    ssize_t time_start_pos;
    ssize_t time_end;
    //find where session description ends first  m
    time_start_pos = sdp_string.find("t=");
    if(time_start_pos == std::string::npos)
    {
        _valid = false;
        _invalid_reason = "No Time Desription Detected";
        return;
    }
    auto session_string  = std::string(sdp_string.begin() ,sdp_string.begin() + time_start_pos );
    _session_description = SdpDescription(session_string); 

    std::stringstream sdp_stream(std::string(sdp_string.begin() + time_start_pos  , sdp_string.end()));
    char next_char = sdp_stream.peek();
    while(next_char == 't' || next_char=='r')
    {
        std::string line;
        std::getline(sdp_stream,line);
        char type = line[0];
        auto value = std::string(line.begin() + 2 , line.end());
        _time_description.SetAttribute(type,value,true);
        next_char = sdp_stream.peek();
    }
    while(next_char == 'z' || next_char=='a' || next_char=='k')
    {
        std::string line;
        std::getline(sdp_stream,line);
        char type = line[0];
        auto value = std::string(line.begin() + 2 , line.end());
        _additional_session_info.SetAttribute(type,value,true);
        next_char = sdp_stream.peek();
    }
    while(next_char !=  std::char_traits<char>::eof())
    {
        if(next_char != 'm')
        {
            _valid = false;
            _invalid_reason = "Mo m value found in media description";
            return;
        }
        MediaDescription media_description;
        std::string line;
        while(next_char !=  std::char_traits<char>::eof())
        {
            getline(sdp_stream,line);
            char type = line[0];
            auto value = std::string(line.begin() + 2 , line.end());
            media_description.SetAttribute(type,value,true);
            next_char = sdp_stream.peek();
            if( next_char == 'm')
            {
                break;
            }
        }
        _media_descriptions.push_back(media_description);
    }
    _valid = true;
}
void Sdp::SetVersion(const std::string version)
{
    _session_description.SetAttribute('v' , version , false);
}
std::string Sdp::GetVersion() const
{
    return _session_description.GetAttribute('v').value();
}
void Sdp::SetOrigin(const std::string origin)
{
    _session_description.SetAttribute('o' , origin , false);
}
std::string Sdp::GetOrigin() const
{
    return _session_description.GetAttribute('o').value();
}
void Sdp::SetSessionName(const std::string session)
{
    _session_description.SetAttribute('s' , session , false);
}
std::string Sdp::GetSessionName() const
{
    return _session_description.GetAttribute('s').value();
}
void Sdp::SetSessionInformation(const std::string information)
{
    _session_description.SetAttribute('i' , information , false);
}
std::string Sdp::GetSessionInformation() const
{
    return _session_description.GetAttribute('i').value();
}
void Sdp::SetURI(const std::string uri)
{
    _session_description.SetAttribute('u' , uri , false);
}
std::string Sdp::GetSessionURI() const
{
    return _session_description.GetAttribute('u').value();
}
void Sdp::SetEmail(const std::string email)
{
    _session_description.SetAttribute('e' , email , false);
}
std::string Sdp::GetSessionEmail() const
{
    return _session_description.GetAttribute('e').value();
}
void Sdp::SetPhone(const std::string phone)
{
    _session_description.SetAttribute('p' , phone , false);
}
std::string Sdp::GetSessionPhone() const
{
    return _session_description.GetAttribute('p').value();
}
void Sdp::SetSessionConnectionData(const std::string connection_data)
{
    _session_description.SetAttribute('c' , connection_data , false);
}
std::vector<std::string> Sdp::GetSessionConnectionData() const
{
    return _session_description.GetAttributes('c');
}
void Sdp::SetSessionBandwidth(const std::string bandwidth_info)
{
    _session_description.SetAttribute('b' , bandwidth_info , true);
}
std::vector<std::string> Sdp::GetSessionBandWidth() const
{
    return _session_description.GetAttributes('b');
}
void Sdp::SetTiming(const std::string timing)
{
    _time_description.SetAttribute('t' , timing , false);
}
std::string Sdp::GetTiming() const
{
    return _time_description.GetAttribute('t').value();
}
void Sdp::SetRepeat(const std::string repeat)
{
    _time_description.SetAttribute('r' , repeat , true);
}
std::vector<std::string> Sdp::GetRepeat() const
{
    return _time_description.GetAttributes('r');
}
void Sdp::SetTimezoneAdjustment(const std::string tz_adjustment)
{
    _additional_session_info.SetAttribute('z' , tz_adjustment , false);
}
std::string Sdp::GetTimezoneAdjustment() const
{
    return _additional_session_info.GetAttribute('z').value();
}
void Sdp::SetSessionEncryptionKey(const std::string encryption_key)
{
    _additional_session_info.SetAttribute('k' , encryption_key , false);
}
std::string Sdp::GetSessionEncryptionKey() const
{
    return _additional_session_info.GetAttribute('k').value();
}
void Sdp::SetSessionAttribute(const std::string session_attribute)
{
    _additional_session_info.SetAttribute('a' , session_attribute , true);
}
std::vector<std::string> Sdp::GetSessionAttributes() const
{
    return _additional_session_info.GetAttributes('a');
}
void Sdp::AddMediaSession(const MediaDescription media_description)
{
    _media_descriptions.emplace_back(media_description);
}
std::vector<MediaDescription> Sdp::GetMediaSessions() const
{
    return _media_descriptions;
}
std::string Sdp::Serialize() const
{
    std::string sdp_string=  _session_description.Serialize() + _time_description.Serialize() + _additional_session_info.Serialize();
    for(auto media_description : _media_descriptions)
    {
        sdp_string += media_description.Serialize();
    }
    return sdp_string;
}