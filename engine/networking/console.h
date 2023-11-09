#pragma once
#include <string>
#include <unordered_map>
#include <functional>

namespace Game
{

class Console
{
private:
	const char* windowLabel;
	char* inputBuff;
	size_t inputBuffSize;
	std::unordered_map<std::string, std::function<void(const std::string&)>> commands;

	char* outputBuff;
	size_t outputLineSize;
	size_t outputLineCount;

public:
	Console(const char* _windowLabel, size_t _inputBufferSize, size_t _outputLineSize, size_t _outputLineCount);
	~Console();

	void SetCommand(const std::string& name, std::function<void(const std::string&)> function);
	void AddOutput(const std::string& output);
	void Draw();

private:
	void ClearInputBuff();
	void ClearOutputBuff();
	void ReadCommand();
};
}