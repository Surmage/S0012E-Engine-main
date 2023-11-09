#include "config.h"
#include "console.h"
#include "imgui.h"

namespace Game
{
Console::Console(const char* _windowLabel, size_t _inputBufferSize, size_t _outputLineSize, size_t _outputLineCount)
{
	windowLabel = _windowLabel;
	inputBuffSize = _inputBufferSize;
	inputBuff = new char[inputBuffSize+1];

	outputLineSize = _outputLineSize;
	outputLineCount = _outputLineCount;
	outputBuff = new char[outputLineCount * outputLineSize + 1];

	ClearInputBuff();
	ClearOutputBuff();
}

Console::~Console()
{
	delete[] inputBuff;
	delete[] outputBuff;
}

void Console::SetCommand(const std::string& name, std::function<void(const std::string&)> func)
{
	commands[name] = func;
}

void Console::AddOutput(const std::string& output)
{
	for (size_t i = 1; i < outputLineCount; i++)
	{
		for (size_t j = 0; j+1 < outputLineSize; j++)
		{
			// move line up
			size_t prevLineIndex = (i - 1) * outputLineSize + j;
			size_t currLineIndex = i * outputLineSize + j;
			outputBuff[prevLineIndex] = outputBuff[currLineIndex];

			// add the output
			if (i + 1 == outputLineCount)
			{
				outputBuff[currLineIndex] = (j < output.size() ? output[j] : ' ');
			}
		}
	}
}

void Console::Draw()
{
	ImGui::Begin(windowLabel);

	ImGui::Text(outputBuff);
	
	if (ImGui::InputText("Input", inputBuff, inputBuffSize, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		ReadCommand();
	}
	if (ImGui::Button("Enter command"))
	{
		ReadCommand();
	}

	ImGui::End();
}

void Console::ClearInputBuff()
{
	for (size_t i = 0; i < inputBuffSize+1; i++)
		inputBuff[i] = '\0';
}

void Console::ClearOutputBuff()
{
	for (size_t i = 0; i < outputLineCount; i++)
	{
		for (size_t j = 0; j < outputLineSize; j++)
		{
			size_t index = i * outputLineSize + j;
			outputBuff[index] = (j + 1 < outputLineSize ? ' ' : '\n');
		}
	}

	outputBuff[outputLineCount * outputLineSize] = '\0';
}

void Console::ReadCommand()
{
	bool hasName = false;
	std::string commandName;
	std::string commandArg;

	for (size_t i = 0; i < inputBuffSize && inputBuff[i] != '\0'; i++)
	{
		char c = inputBuff[i];
		if (!hasName && c == ' ')
		{
			hasName = true;
		}
		else if (!hasName)
		{
			commandName += c;
		}
		else
		{
			commandArg += c;
		}
	}

	if (commands.count(commandName) == 0)
		printf("\n[WARNING] Invalid command '%s'.\n", commandName.c_str());
	else
		commands[commandName](commandArg);

	ClearInputBuff();
}
}