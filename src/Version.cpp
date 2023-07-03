#include "Version.hpp"


namespace wf
{
    Version::Version(QString a_version_string)
    {
        setCurrentVersion(a_version_string);
        curl_global_init(CURL_GLOBAL_ALL);
        current_version_digits = toDigits(a_version_string.toStdString());
        load();
    }
    
    Version::~Version()
    {
        curl_global_cleanup();
        save();
    }
    
    QString Version::getCurrentVersion()
    {
        return current_version;
    }
    
    void Version::setCurrentVersion(QString a_version)
    {
        current_version = a_version;
        return;
    }
    
    bool Version::checkedRecently()
    {
        QDateTime current_time = QDateTime::currentDateTime();
        bool checked_recently = last_checked.secsTo(current_time) < recency_threshold;
        last_checked = current_time;
        return checked_recently;
    }
    
    UpdateStatus Version::isUpdateAvailable()
    {
        try
        {
            std::string all_releases_json = fetchJSONFromGithub();
            std::optional<std::string> latest_release_tag = readVersionFromJSON(all_releases_json);

            if (!latest_release_tag.has_value())
            {
                return UpdateStatus::Inaccessible;
            }

            upstream_version = QString::fromStdString(latest_release_tag.value());

            if (isNewerThanCurrent(latest_release_tag.value()))
            {
                return UpdateStatus::NewerAvailable;
            }
            else
            {
                return UpdateStatus::UpToDate;
            }
        }
        catch(const std::exception& e)
        {
            return UpdateStatus::Inaccessible;
        }
    }
    
    QString Version::getUpstreamVersion()
    {
        return upstream_version;
    }
    
    void Version::load()
    {
        last_checked = value("version/last_checked", "1970-01-01T00:00:00").toDateTime();
        return;
    }
    
    void Version::save()
    {
        setValue("version/last_checked", last_checked);
        return;
    }
    
    size_t Version::writeCallback(char *buffer, size_t size, size_t nmemb, void *userp)
    {
        ((std::string*)userp)->append(buffer, size * nmemb);
        return size * nmemb;
    }
    
    std::string Version::fetchJSONFromGithub()
    {
        CURL* curl = curl_easy_init();
        std::string buffer;

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/thomilist/wordfeud/releases");
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Thomilist/wordfeud");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        return buffer;
    }
    
    std::optional<std::string> Version::readVersionFromJSON(std::string a_json_string)
    {
        nlohmann::json json = nlohmann::json::parse(a_json_string);

        if (!json.is_array() && !json[0].contains("tag_name"))
        {
            return std::nullopt;
        }

        return json[0]["tag_name"];
    }
    
    std::vector<int> Version::toDigits(std::string a_version)
    {
        std::vector<int> digits;
        std::istringstream stream{a_version};
        std::string number;

        while (std::getline(stream, number, '.'))
        {
            digits.push_back(std::stoi(number));
        }

        return digits;
    }
    
    bool Version::isNewerThanCurrent(std::string a_version)
    {
        std::vector<int> version_digits = toDigits(a_version);

        for (size_t index = 0; index < version_digits.size(); ++index)
        {
            if (version_digits[index] > current_version_digits[index])
            {
                return true;
            }
        }

        return false;
    }
}