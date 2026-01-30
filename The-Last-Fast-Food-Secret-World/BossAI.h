#pragma once
#include <iostream>

using namespace std;

class AI
{
public:
	AI();
	~AI();
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
	int GetBossCardIndex(string gameStateDescription);

private:
	string m_ApiKey;
	string ToUTF8(const string& str);
};




