#ifndef __VERSION_H__
#define __VERSION_H__

#include <sstream>
#include <string>
#include <vector>

#include <QSettings>
#include <QString>
#include <QDateTime>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace wf
{
    class Version : public QSettings
    {
        public:
            Version(QString a_version_string);
            ~Version();

            QString getCurrentVersion();
            void setCurrentVersion(QString a_version);
            bool checkedRecently();
            bool isUpdateAvailable();
            QString getUpstreamVersion();
        
        private:
            void load();
            void save();
            static size_t writeCallback(char *buffer, size_t size, size_t nmemb, void *userp);
            std::string fetchJSONFromGithub();
            std::string readVersionFromJSON(std::string a_json_string);
            std::vector<int> toDigits(std::string a_version);
            bool isNewerThanCurrent(std::string a_version);

            QString current_version;
            QString upstream_version;
            std::vector<int> current_version_digits;
            QDateTime last_checked;
            // Last 24 hours considered "recent"
            const qint64 recency_threshold = 60 * 60 * 24;
    };
}

#endif // __VERSION_H__