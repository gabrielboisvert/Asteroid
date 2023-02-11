#pragma once
#include "IGameManager.h"
#include "IniParser.h"
#include "InputScanner.h"
#include "HUDManager.h"
#include "ScrollingManager.h"
#include "PhysiqueManager.h"
#include "SoundManager.h"
#include "HighScoreManager.h"

namespace Asteroid {
	class GameManager : public IGameManager {
	private:
		static GameManager instance;
		GameManager();
		Config cfg;
		InputScanner inputScanner;
		HUDManager hubManager;
		ScrollingManager scrollingManager;
		PhysiqueManager physiqueManager;
		SoundManager soundManager;
		HighScoreManager highScoreManager;

	public:
		static GameManager& getInstance();

		void init(Window& window, const char* configFile) override;
		
		Config& getConfig();
		
		ScrollingManager& getScrollManager();

		InputScanner& getInputScanner();

		Renderer& getRenderer();

		PhysiqueManager& getPhysiqueManager();

		SoundManager& getSoundManager();

		HighScoreManager& getHighScoreManager();

		void setState(GameState state);

		void update() override;

		void updateState();
	};
}