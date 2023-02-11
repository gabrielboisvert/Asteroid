#include "pch.h"
#include "SoundManager.h"
#include "GameManager.h"

Asteroid::SoundManager::SoundManager() 
{
	this->currentMusic = -1;
	this->music = nullptr;
}

void Asteroid::SoundManager::init()
{
	Section* musicSection = GameManager::getInstance().getConfig().getSection("music");
	for (std::unordered_map<std::string, std::string>::iterator it = musicSection->keyvalues.begin(); it != musicSection->keyvalues.end(); it++)
		this->playlist.push_back(it->second);
	
	this->music = new sf::Music;
	this->music->setVolume(5);

	Config& cfg = GameManager::getInstance().getConfig();
	this->soundState[SoundState::NEW_GAME] = this->createSound(*cfg.getValue("soundEffects", "jingle_new_game_sfx"));
	this->soundState[SoundState::GAME_OVER] = this->createSound(*cfg.getValue("soundEffects", "jingle_game_over_sfx"));
	this->soundState[SoundState::NEW_HI_SCORE] = this->createSound(*cfg.getValue("soundEffects", "jingle_new_hi_score_sfx"));

	this->changeMusic();
}

void Asteroid::SoundManager::update()
{
	if (this->music->getStatus() == sf::SoundSource::Stopped)
		this->changeMusic();

	if (this->soundEffect.size() > 0 && this->soundEffect[0]->getStatus() == sf::SoundSource::Stopped)
	{
		delete this->soundEffect[0];
		this->soundEffect.pop_front();
	}
}

void Asteroid::SoundManager::changeMusic()
{
	this->currentMusic = (int64_t(this->currentMusic) + 1) % this->playlist.size();
	if (this->music->openFromFile(this->playlist[this->currentMusic]))
		this->music->play();
}

sf::SoundBuffer* Asteroid::SoundManager::createSound(const std::string& filename)
{
	sf::SoundBuffer* buffer = new sf::SoundBuffer;
	buffer->loadFromFile(filename);
	return buffer;
}

void Asteroid::SoundManager::addSound(sf::SoundBuffer* buff)
{
	if (buff == nullptr)
		return;
	sf::Sound* sound = new sf::Sound(*buff);
	sound->setVolume(40);
	sound->play();
	this->soundEffect.push_back(sound);
}

void Asteroid::SoundManager::clearSoundEffect()
{
	for (unsigned int i = 0; i < this->soundEffect.size(); i++)
		delete this->soundEffect[i];
	this->soundEffect.clear();
}

void Asteroid::SoundManager::playState()
{
	GameState state = GameManager::getInstance().getState();

	switch (state)
	{
	case Asteroid::GameState::MAIN_MENU:
		break;
	case Asteroid::GameState::RUNNING: this->addSound(this->soundState[SoundState::NEW_GAME]);
		break;
	case Asteroid::GameState::PAUSED:
		break;
	case Asteroid::GameState::GAME_OVER: 
		this->addSound(this->soundState[SoundState::GAME_OVER]);
		if (GameManager::getInstance().getHighScoreManager().getIsNewHighScore())
			this->addSound(this->soundState[SoundState::NEW_HI_SCORE]);
		break;
	case Asteroid::GameState::QUIT:
		break;
	default:
		break;
	}
}

Asteroid::SoundManager::~SoundManager()
{
	delete this->music;
	this->clearSoundEffect();

	for (std::map<SoundState, sf::SoundBuffer*>::iterator it = this->soundState.begin(); it != this->soundState.end(); it++)
		delete it->second;
	this->soundState.clear();
}
