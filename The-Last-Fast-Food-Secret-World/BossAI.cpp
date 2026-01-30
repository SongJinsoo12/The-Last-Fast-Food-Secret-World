#include "BossAI.h"

#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using json = nlohmann::json;

AI::AI()
{
    m_ApiKey = "AIzaSyC-MLFxjL4TbGMxdpE8Y-X7MK0pfJL2jwU";
}

AI::~AI()
{
}

size_t AI::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

int AI::GetBossCardIndex(string gameStateDescription)
{
    curl_version_info_data* vinfo = curl_version_info(CURLVERSION_NOW);
    string protocols = "Supported Protocols: ";
    if (vinfo->protocols) {
        for (int i = 0; vinfo->protocols[i]; i++) {
            protocols += string(vinfo->protocols[i]) + " ";
        }
    }
    OutputDebugStringA((protocols + "\n").c_str());

    CURL* curl;
    CURLcode res;
    string readBuffer;
    int selectedIndex = -1;

    curl = curl_easy_init();
    if (curl) {
        string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=" + m_ApiKey;

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        string promptText = "너는 햄버거 카드 게임의 보스 AI다. \n"
            "현재 상황을 분석해서 '가장 좋은 카드의 인덱스(0부터 시작)'를 선택해라.\n"
            "반드시 아래 JSON 형식으로만 답해라. 설명은 필요 없다.\n"
            "형식: {\"card_index\": 숫자}\n\n"
            "상황: " + gameStateDescription;

        json payload = {
            {"contents", {
                {
                    {"parts", {
                        {{"text", ToUTF8(promptText)}} 
                    }}
                }
            }}
        };

        string jsonStr = payload.dump();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            string errorMsg = "Curl Failed Code: " + to_string(res) + " / Reason: " + curl_easy_strerror(res) + "\n";
            OutputDebugStringA(errorMsg.c_str());
        }

        if (res != CURLE_OK) {
            OutputDebugStringA("Curl Failed\n");
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    try {
        if (readBuffer.empty()) {
            OutputDebugStringA("Error: Response is empty.\n");
            return -1;
        }

        string logMsg = "Gemini Response: " + readBuffer + "\n";
        OutputDebugStringA(logMsg.c_str());

        auto responseJson = json::parse(readBuffer);

        if (!responseJson.contains("candidates")) {
            OutputDebugStringA("Error: 'candidates' key not found. API Key might be invalid or quota exceeded.\n");

            if (responseJson.contains("error")) {
                string errorDetail = responseJson["error"]["message"].get<string>();
                string errorLog = "API Error Message: " + errorDetail + "\n";
                OutputDebugStringA(errorLog.c_str());
            }
            return -1; 
        }

        if (responseJson["candidates"].empty()) {
            OutputDebugStringA("Error: Candidates array is empty.\n");
            return -1;
        }

        string contentText = responseJson["candidates"][0]["content"]["parts"][0]["text"];

        size_t jsonStart = contentText.find("{");
        size_t jsonEnd = contentText.rfind("}");

        if (jsonStart != string::npos && jsonEnd != string::npos) {
            string jsonPart = contentText.substr(jsonStart, jsonEnd - jsonStart + 1);
            auto aiChoice = json::parse(jsonPart);

            if (aiChoice.contains("card_index")) {
                if (aiChoice["card_index"].is_number()) {
                    selectedIndex = aiChoice["card_index"].get<int>();
                }
                else if (aiChoice["card_index"].is_string()) {
                    selectedIndex = stoi(aiChoice["card_index"].get<string>());
                }
            }
        }
    }
    catch (exception& e) {
        string errorMsg = "JSON Exception caught: ";
        errorMsg += e.what();
        errorMsg += "\n";
        OutputDebugStringA(errorMsg.c_str());
        return 0; 
    }

    return selectedIndex;
}

string AI::ToUTF8(const string& str)
{
    if (str.empty()) return "";

    int nLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), NULL, NULL);
    wstring wstr;
    wstr.resize(nLen);

    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), &wstr[0], nLen);

    int nLenUTF8 = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    string strUTF8;
    strUTF8.resize(nLenUTF8 - 1); 

    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strUTF8[0], nLenUTF8, NULL, NULL);

    return strUTF8;
}
