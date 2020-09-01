#ifndef __SDP_H__
#define __SDP_H__

#include <vector>
#include <string>
#include <optional>

// Session description
//          v=  (protocol version)
//          o=  <username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
//          s=  (session name)
//          i=* (session information)
//          u=* (URI of description)
//          e=* (email address)
//          p=* (phone number)
//          c=* (connection information -- not required if included in
//               all media)
//          b=* (zero or more bandwidth information lines)
//          One or more time descriptions ("t=" and "r=" lines; see below)
//          z=* (time zone adjustments)
//          k=* (encryption key)
//          a=* (zero or more session attribute lines)
//          Zero or more media descriptions

//       Time description
//          t=  (time the session is active)
//          r=* (zero or more repeat times)

//       Media description, if present
//          m=  (media name and transport address)
//          i=* (media title)
//          c=* (connection information -- optional if included at
//               session level)
//          b=* (zero or more bandwidth information lines)
//          k=* (encryption key)
//          a=* (zero or more media attribute lines)

typedef std::vector<std::pair<char,std::string>> description_t;
class SdpDescription
{
    public :
        SdpDescription() = default;
        SdpDescription(const std::string&);
        ~SdpDescription() = default;
        bool HasAttribute(char type) const;
        void SetAttribute(char type, std::string value, bool allow_multiple);
        std::optional<std::string> GetAttribute(char type) const;
        std::vector<std::string> GetAttributes(char type) const;
        std::string Serialize() const;
    protected:
        description_t _description_map;
};
class MediaDescription : public SdpDescription
{
    public :
        MediaDescription() = default;
        MediaDescription(const std::string&);
        ~MediaDescription() = default;
        std::vector<std::string> GetMediaDescription() const;
        std::string GetMediaTitle() const;
        std::vector<std::string> GetMediaConnectionData() const;
        std::vector<std::string> GetMediaBandwidth() const;
        std::string GetMediaEncryptionKey() const;
        std::vector<std::string> GetMediaAttributes() const;
        void SetMediaDescription(const std::string);
        void SetMediaTitle(const std::string);
        void AddMediaConnectionData(const std::string);
        void SetMediaBandwidth(const std::string);
        void SetMediaEncryptionKey(const std::string);
        void SetMediaAttribute(const std::string);

};
class Sdp{
    public:
        Sdp() =default;
        Sdp(const std::string&);
        ~Sdp() =default;
        void SetVersion(const std::string);
        void SetOrigin(const std::string);
        void SetSessionName(const std::string);
        void SetSessionInformation(const std::string);
        void SetURI(const std::string);
        void SetEmail(const std::string);
        void SetPhone(const std::string);
        void SetSessionConnectionData(const std::string);
        void SetSessionBandwidth(const std::string);
        void SetTiming(const std::string);
        void SetRepeat(const std::string);
        void SetTimezoneAdjustment(const std::string);
        void SetSessionEncryptionKey(const std::string);
        void SetSessionAttribute(const std::string);
        void AddMediaSession(const MediaDescription);

        std::string GetVersion() const;
        std::string GetOrigin() const;
        std::string GetSessionName() const;
        std::string GetSessionInformation() const;
        std::string GetSessionURI() const;
        std::string GetSessionEmail() const;
        std::string GetSessionPhone() const;
        std::vector<std::string> GetSessionConnectionData() const;
        std::vector<std::string> GetSessionBandWidth() const;
        std::string GetTiming() const;
        std::vector<std::string> GetRepeat() const;
        std::string GetTimezoneAdjustment() const;
        std::string GetSessionEncryptionKey() const;
        std::vector<std::string> GetSessionAttributes() const;
        std::vector<MediaDescription>  GetMediaSessions() const;

        std::string Serialize() const;
        bool IsValid() { return _valid;};
        std::string _invalid_reason;
    private:
        bool _valid;
        SdpDescription _session_description;
        SdpDescription _time_description;
        SdpDescription _additional_session_info;
        std::vector<MediaDescription> _media_descriptions;
};

#endif