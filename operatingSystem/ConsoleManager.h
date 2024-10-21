#pragma once
#include <memory>
#include <unordered_map>
#include <Windows.h>

#include "AConsole.h"
#include "BaseScreen.h"

class ConsoleManager
{
public:
	typedef std::unordered_map<std::string, std::shared_ptr<AConsole>> ConsoleTable;

	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void drawConsole() const;
	void process() const;
	void switchConsole(std::string consoleName);

	void registerScreen(std::shared_ptr<BaseScreen> console);
	void switchToScreen(std::string screenName);
	void unregisterScreen(std::string screenName);

	void returnToPreviousConsole();
	void exitApplication();
	bool isRunning() const;

	HANDLE getConsoleHandle() const;

	void setCursorPosition(int posX, int posY) const;

private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&){};
	ConsoleManager& operator=(ConsoleManager const&) {};
	static ConsoleManager* sharedInstance;

	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;

	HANDLE consoleHandle;
	bool running = true;
};

