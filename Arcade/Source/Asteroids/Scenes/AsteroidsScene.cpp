#include "ArcadePCH.h"
#include "AsteroidsScene.h"
#include "Asteroids/AsteroidConstants.h"

static constexpr size_t STARTING_NUM_ASTEROIDS = 16;
static constexpr unsigned int NUM_LIVES = 3;
static constexpr int SCORE_VAL = 50;
static constexpr int LEVEL_START_TIME = 5000;

static constexpr int READY_TIME_TRANSITION_THRESHOLD = 5000;
static constexpr int READY_TIME_DISPLAY_THRESHOLD = 3000;
static constexpr int GO_TIME_TRANSITION_THRESHOLD = 2000;
static constexpr int GO_TIME_DISPLAY_THRESHOLD = 1000;

static const std::string GAME_OVER_STR = "Game Over";
static const std::string READY_STR = "Ready?";
static const std::string GO_STR = "GO!";
static const std::string SCORE_STR = "Score";

static const std::string SPACE_SHIP_SPRITE_NAME = "space_ship";
static const std::string MISSILE_SPRITE_NAME = "missile_1";
static std::vector<std::string> asteroidNames = {"big_rock", "medium_rock", "small_rock"};

static constexpr float POWER_UP_LIFE_TIME = 5.f;
static constexpr float POWER_UP_SPEED = 5.f;

static constexpr float SCREEN_SHAKE_POWER = 5.f;
static constexpr float SCREEN_SHAKE_TIME = 0.3f;

AsteroidsScene::AsteroidsScene()
{
	std::random_device r;
	m_RandomGenerator.seed(r());
}

bool AsteroidsScene::Init()
{
    m_AsteroidsSprites.Load("AsteroidsSprites");
    bool loaded = m_BGImage.Load(Core::Application::Get().GetBasePath() + "StarrySpace.bmp");
    assert(loaded);

    m_Ship.Init(m_AsteroidsSprites, Core::Vec2D(static_cast<float>(Core::Application::Get().GetWindow().GetWidth()) / 2.f, static_cast<float>(Core::Application::Get().GetWindow().GetHeight()) / 2.f));
    m_StringRect = Core::Rectangle(Core::Vec2D(0.f, static_cast<float>(Core::Application::Get().GetWindow().GetHeight()) / 2.f - 50.f), Core::Application::Get().GetWindow().GetWidth(), 100);

    ResetGame();

    Core::KeyboardButtonAction leftKeyAction;
    leftKeyAction.Key = Core::GameController::LeftKey();
    leftKeyAction.Action = [this](uint32_t dt, Core::InputState state)
    {
        if (Core::GameController::IsPressed(state))
            m_Ship.UpdateYaw(AsteroidsShipYawMovement::ShipHeadingTurnLeft);
        else
            m_Ship.UpdateYaw(AsteroidsShipYawMovement::ShipHeadingNone);
    };
    m_GameController.AddKeyboardButtonAction(leftKeyAction);

    Core::KeyboardButtonAction rightKeyAction;
    rightKeyAction.Key = Core::GameController::RightKey();
    rightKeyAction.Action = [this](uint32_t dt, Core::InputState state)
    {
        if (Core::GameController::IsPressed(state))
            m_Ship.UpdateYaw(AsteroidsShipYawMovement::ShipHeadingTurnRight);
        else
            m_Ship.UpdateYaw(AsteroidsShipYawMovement::ShipHeadingNone);
    };
    m_GameController.AddKeyboardButtonAction(rightKeyAction);

    Core::KeyboardButtonAction upKeyAction;
    upKeyAction.Key = Core::GameController::UpKey();
    upKeyAction.Action = [this](uint32_t dt, Core::InputState state)
    {
        if (Core::GameController::IsPressed(state))
            m_Ship.EngageThrusters();
        else
            m_Ship.DisengageThrusters();
    };
    m_GameController.AddKeyboardButtonAction(upKeyAction);

    Core::KeyboardButtonAction actionKeyAction;
    actionKeyAction.Key = Core::GameController::ActionKey();
    actionKeyAction.Action = [this](uint32_t dt, Core::InputState state)
    {
        if (Core::GameController::IsPressed(state) && m_GameState == AsteroidsGameState::PlayGame)
            m_Ship.Fire();
        else if (Core::GameController::IsPressed(state) && m_GameState == AsteroidsGameState::GameOver)
            ResetGame();
    };
    m_GameController.AddKeyboardButtonAction(actionKeyAction);

    Core::KeyboardButtonAction cancelKeyAction;
    cancelKeyAction.Key = Core::GameController::CancelKey();
    cancelKeyAction.Action = [this](uint32_t dt, Core::InputState state)
    {
        if (Core::GameController::IsPressed(state) && m_GameState == AsteroidsGameState::GameOver)
            Core::Application::Get().PopScene();
    };
    m_GameController.AddKeyboardButtonAction(cancelKeyAction);

    Core::Sprite bgSprite;
    bgSprite.width = m_BGImage.GetWidth();
    bgSprite.height = m_BGImage.GetHeight();

    Core::Application::Get().GetWindow().DrawBackground(m_BGImage, bgSprite, Core::Vec2D::Zero);

    return true;
}

void AsteroidsScene::Update(uint32_t dt)
{
    if (m_GameState == AsteroidsGameState::GameOver)
        return;

    if (m_GameState == AsteroidsGameState::PlayGame)
        PlayGame(dt);
    else if (m_GameState == AsteroidsGameState::LevelStarting)
    {
        m_LevelStartTimer -= dt;
        if (m_LevelStartTimer <= 0)
            m_GameState = AsteroidsGameState::PlayGame;
    }
}

void AsteroidsScene::Draw(Core::Window& window)
{
    Core::Sprite bgSprite;
    bgSprite.width = m_BGImage.GetWidth();
    bgSprite.height = m_BGImage.GetHeight();
    Core::Application::Get().GetWindow().DrawBackground(m_BGImage, bgSprite, Core::Vec2D::Zero);

    if (m_GameState != AsteroidsGameState::LevelStarting)
    {
        m_Ship.Draw(window);

        for (auto& asteroid : m_Asteroids)
            asteroid.Draw(window);

        for (auto& powerUp : m_PowerUps)
            powerUp.Draw(window);
    }

    const auto& font = Core::Application::Get().GetFont();
    Core::Vec2D textDrawPosition;

    float windowWidth = static_cast<float>(Core::Application::Get().GetWindow().GetWidth());
    float windowHeight = static_cast<float>(Core::Application::Get().GetWindow().GetHeight());

    Core::DrawTransform transform;
    transform.Pos = Core::Vec2D::Zero;
    transform.RotationAngle = 0.0f;
    transform.Scale = 1.0f;

    Core::ColorParams colorParams;
    colorParams.Alpha = 1.0f;
    colorParams.BilinearFiltering = false;
    colorParams.Gradient.XParam = Core::GradientXParam::NoXGradient;
    colorParams.Gradient.YParam = Core::GradientYParam::NoYGradient;
    colorParams.Overlay = Core::Color::White();

    Core::UVParams uvParams;

    DrawScore(window, windowWidth, windowHeight, font, textDrawPosition, transform, colorParams, uvParams);

    constexpr float X_PAD = 1.0f;

    auto sprite = m_AsteroidsSprites.GetSprite(SPACE_SHIP_SPRITE_NAME);
    float xPos = X_PAD;
    constexpr float scale = 0.75f;

    for (int i = 0; i < m_Lives; ++i)
    {
        transform.Pos = Core::Vec2D(xPos, windowHeight - static_cast<float>(sprite.height));
        transform.Scale = scale;

        window.Draw(m_AsteroidsSprites, SPACE_SHIP_SPRITE_NAME, transform, colorParams, uvParams);
        xPos += X_PAD + static_cast<uint32_t>(std::round(static_cast<float>(sprite.width) * scale));
    }

    sprite = m_AsteroidsSprites.GetSprite(MISSILE_SPRITE_NAME);
    xPos = windowWidth - m_Ship.AmmoLeft() * (X_PAD + sprite.width);

    for (uint32_t i = 0; i < m_Ship.AmmoLeft(); ++i)
    {
        transform.Pos = Core::Vec2D(xPos, windowHeight - static_cast<float>(sprite.height) - X_PAD);
        transform.Scale = 1.0f;
        transform.RotationAngle = 0.0f;

        window.Draw(m_AsteroidsSprites, MISSILE_SPRITE_NAME, transform, colorParams, uvParams);
        xPos += X_PAD + sprite.width;
    }

    if (m_GameState == AsteroidsGameState::GameOver)
    {
        textDrawPosition = font.GetDrawPosition(GAME_OVER_STR, m_StringRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
        transform.Pos = textDrawPosition;
        colorParams.Overlay = Core::Color::Red();
        window.Draw(font, GAME_OVER_STR, transform, colorParams, uvParams);
    }

    if (m_GameState == AsteroidsGameState::LevelStarting)
    {
        if (m_LevelStartTimer <= READY_TIME_TRANSITION_THRESHOLD && m_LevelStartTimer > READY_TIME_DISPLAY_THRESHOLD)
        {
            textDrawPosition = font.GetDrawPosition(READY_STR, m_StringRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
            transform.Pos = textDrawPosition;
            colorParams.Overlay = Core::Color::Red();
            window.Draw(font, READY_STR, transform, colorParams, uvParams);
        }
        else if (m_LevelStartTimer <= READY_TIME_DISPLAY_THRESHOLD && m_LevelStartTimer > GO_TIME_TRANSITION_THRESHOLD)
        {
            textDrawPosition = font.GetDrawPosition(READY_STR, m_StringRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
            transform.Pos = textDrawPosition;
            colorParams.Overlay = Core::Color::Red();
            window.Draw(font, READY_STR, transform, colorParams, uvParams);
        }
        else if (m_LevelStartTimer <= GO_TIME_TRANSITION_THRESHOLD && m_LevelStartTimer > GO_TIME_DISPLAY_THRESHOLD)
        {
            textDrawPosition = font.GetDrawPosition(GO_STR, m_StringRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
            transform.Pos = textDrawPosition;
            colorParams.Overlay = Core::Color::Red();
            window.Draw(font, GO_STR, transform, colorParams, uvParams);
        }
        else if (m_LevelStartTimer <= GO_TIME_DISPLAY_THRESHOLD)
        {
            textDrawPosition = font.GetDrawPosition(GO_STR, m_StringRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
            transform.Pos = textDrawPosition;
            colorParams.Overlay = Core::Color::Red();
            window.Draw(font, GO_STR, transform, colorParams, uvParams);
        }
    }
}

const std::string& AsteroidsScene::GetSceneName() const
{
    static std::string sceneName = "Asteroids!";
    return sceneName;
}

void AsteroidsScene::SetupAsteroids()
{
    for (size_t i = m_Asteroids.size(); i < STARTING_NUM_ASTEROIDS; ++i)
        AddRandomAsteroid();
}

void AsteroidsScene::AddRandomAsteroid()
{
    static std::uniform_int_distribution<size_t> spriteNamesDistribution(0, asteroidNames.size() - 1);

    Core::Vec2D spawnLocation = GetSpawnLocation();
    Core::Vec2D vel = GetSpawnVelocity(spawnLocation);

    size_t asteroidIndex = static_cast<size_t>(spriteNamesDistribution(m_RandomGenerator));
    std::string name = asteroidNames[asteroidIndex];
    AsteroidSize size = static_cast<AsteroidSize>(asteroidIndex + 1);

    assert(size != AsteroidSize::None);

    AddAsteroid(m_Asteroids, spawnLocation, vel, size);
}

void AsteroidsScene::RemoveDestroyedAsteroids(uint32_t dt)
{
    auto it = std::remove_if(m_Asteroids.begin(), m_Asteroids.end(), [](const Asteroid& a)
    {
        return a.IsDestroyed();
    });

    if (it != m_Asteroids.end())
        m_Asteroids.erase(it, m_Asteroids.end());
}

Core::Vec2D AsteroidsScene::GetSpawnLocation()
{
    Core::Sprite largestSprite = m_AsteroidsSprites.GetSprite("big_rock");
    static std::vector<int> xSpawnLocations = {-static_cast<int>(largestSprite.width), static_cast<int>(largestSprite.width + Core::Application::Get().GetWindow().GetWidth())};
    static std::uniform_int_distribution<size_t> xSpawnDistribution(static_cast<size_t>(0), static_cast<size_t>(xSpawnLocations.size() - 1));
    static std::uniform_int_distribution<int> ySpawnDistribution(-static_cast<int>(largestSprite.height), static_cast<int>(Core::Application::Get().GetWindow().GetHeight() + largestSprite.height));

    int xSpawnLocation = xSpawnLocations[static_cast<size_t>(xSpawnDistribution(m_RandomGenerator))];
    int ySpawnLocation = static_cast<int>(ySpawnDistribution(m_RandomGenerator));

    return Core::Vec2D(static_cast<float>(xSpawnLocation), static_cast<float>(ySpawnLocation));
}

Core::Vec2D AsteroidsScene::GetSpawnVelocity(const Core::Vec2D& position)
{
    static std::uniform_real_distribution<float> positiveVelComp(10, 30);
    static std::uniform_real_distribution<float> negativeVelComp(-30, -10);

    Core::Vec2D vel;
    Core::Vec2D spawnLocation = GetSpawnLocation();
    if (spawnLocation.GetX() > 0)
        vel.SetX(static_cast<float>(negativeVelComp(m_RandomGenerator)));
    else
        vel.SetX(static_cast<float>(positiveVelComp(m_RandomGenerator)));

    if (spawnLocation.GetY() > Core::Application::Get().GetWindow().GetHeight() / 2)
        vel.SetY(static_cast<float>(negativeVelComp(m_RandomGenerator)));
    else
        vel.SetY(static_cast<float>(positiveVelComp(m_RandomGenerator)));

    return vel;
}

bool AsteroidsScene::AmmoOutOfBounds(const std::unique_ptr<ShipAmmo>& ammo)
{
    return ammo->IsOutOfBounds(0, 0, Core::Application::Get().GetWindow().GetWidth(), Core::Application::Get().GetWindow().GetHeight());
}

std::vector<Asteroid> AsteroidsScene::BreakAsteroid(const Core::Vec2D& position, const Core::Vec2D& velocity, const Asteroid& asteroid, float ammoScale)
{
    std::vector<Asteroid> asteroidsToAdd;

    size_t numAsteroidsToSpawn = static_cast<size_t>(asteroid.GetSize()) + 1;

    if (numAsteroidsToSpawn < static_cast<size_t>(AsteroidSize::Num) && Core::IsEqual(ammoScale, 1.f))
    {
        float dot = velocity.GetUnitVec().Dot(-asteroid.GetVelocity().GetUnitVec());
        float deltaAngle = acosf(dot);

        if (numAsteroidsToSpawn > 0)
        {
            Core::Vec2D newVec2 = asteroid.GetVelocity().Rotation(deltaAngle);
            AsteroidSize asteroidSize = static_cast<AsteroidSize>(numAsteroidsToSpawn);
            AddAsteroid(asteroidsToAdd, asteroid.GetPosition(), newVec2, asteroidSize);
            for (size_t i = 0; i < numAsteroidsToSpawn - 1; ++i)
            {
                newVec2 = newVec2.Rotation(deltaAngle);
                AddAsteroid(asteroidsToAdd, asteroid.GetPosition(), newVec2, asteroidSize);
            }
        }
    }
    return asteroidsToAdd;
}

void AsteroidsScene::AddAsteroid(std::vector<Asteroid>& asteroids, const Core::Vec2D& spawnLocation, const Core::Vec2D& velocity, AsteroidSize size)
{
    static std::uniform_real_distribution<float> slowRotationDistribution(Core::PI / 8.f, Core::PI / 5.f);
    static std::uniform_real_distribution<float> mediumRotationDistribution(Core::PI / 3.f, Core::PI);
    static std::uniform_real_distribution<float> fastRotationDistribution(1.1f * Core::PI, 1.9f * Core::PI);

    if (size > AsteroidSize::None)
    {
        std::string name = asteroidNames[static_cast<size_t>(size) - 1];
        float rotationRate = 0;

        if (size == AsteroidSize::Large)
            rotationRate = static_cast<float>(slowRotationDistribution(m_RandomGenerator));
        else if (size == AsteroidSize::Medium)
            rotationRate = static_cast<float>(mediumRotationDistribution(m_RandomGenerator));
        else
            rotationRate = static_cast<float>(fastRotationDistribution(m_RandomGenerator));

        Asteroid asteroid;
        asteroid.Init(m_AsteroidsSprites, name, size, spawnLocation, velocity, rotationRate);
        asteroids.push_back(asteroid);
    }
}

Core::Vec2D AsteroidsScene::WrapAround(const Core::Vec2D& objectPosition, const Core::Vec2D& objectSize)
{
    Core::Vec2D newPos = objectPosition;

    if (objectPosition.GetX() > Core::Application::Get().GetWindow().GetWidth() + objectSize.GetX())
        newPos.SetX(-objectSize.GetX() + 0.01f);
    else if (objectPosition.GetX() < -objectSize.GetX())
        newPos.SetX(Core::Application::Get().GetWindow().GetWidth() + objectSize.GetX() - 0.01f);

    if (objectPosition.GetY() < -objectSize.GetY())
        newPos.SetY(Core::Application::Get().GetWindow().GetHeight() + objectSize.GetY() - 0.01f);
    else if (objectPosition.GetY() > Core::Application::Get().GetWindow().GetHeight() + objectSize.GetY())
        newPos.SetY(-objectSize.GetY() + 0.01f);

    return newPos;
}

void AsteroidsScene::ResetLevel()
{
    m_Asteroids.clear();
    m_Ship.Respawn(Core::Vec2D(static_cast<float>(Core::Application::Get().GetWindow().GetWidth()) / 2.f, static_cast<float>(Core::Application::Get().GetWindow().GetHeight()) / 2.f));
    SetupAsteroids();
    m_GameState = AsteroidsGameState::LevelStarting;
    m_LevelStartTimer = LEVEL_START_TIME;
}

void AsteroidsScene::ResetGame()
{
    m_Lives = NUM_LIVES;
    m_Score = 0;
    ResetLevel();
}

int AsteroidsScene::ScoreValue(AsteroidSize asteroidSizeHit)
{
    if (asteroidSizeHit == AsteroidSize::None || asteroidSizeHit == AsteroidSize::Num)
        return 0;

    return static_cast<int>(asteroidSizeHit) * SCORE_VAL;
}

void AsteroidsScene::PlayGame(uint32_t dt)
{
    if (m_Ship.IsDestroyed())
    {
        --m_Lives;
        if (m_Lives >= 0)
            ResetLevel();
        else
            m_GameState = AsteroidsGameState::GameOver;

        return;
    }

    m_Ship.Update(dt);

    Core::Vec2D newShipPosition = WrapAround(m_Ship.GetPosition(), Core::Vec2D(m_Ship.GetWidth(), m_Ship.GetHeight()));
    m_Ship.SetPosition(newShipPosition);

    int64_t powerUpHitIndex = -1;
    for (size_t i = 0; i < m_PowerUps.size(); ++i)
    {
        PowerUp& powerUp = m_PowerUps[i];

        if (powerUp.IsActive())
        {
            powerUp.Update(dt);

            Core::Vec2D newPowerUpPosition = WrapAround(powerUp.GetPosition(), Core::Vec2D(powerUp.GetWidth(), powerUp.GetHeight()));
            powerUp.SetPosition(newPowerUpPosition);

            if (m_Ship.GetBoundingCircle().Intersects(powerUp.GetBoundingCircle()))
            {
                powerUp.ExecuteEffect();
                powerUpHitIndex = i;
                break;
            }
        }
    }

    if (powerUpHitIndex >= 0)
        m_PowerUps.erase(m_PowerUps.begin() + powerUpHitIndex);

    RemoveDestroyedAsteroids(dt);
    Core::Sprite largestSprite = m_AsteroidsSprites.GetSprite("big_rock");

    Core::Vec2D largestSpriteSize = Core::Vec2D(static_cast<float>(largestSprite.width), static_cast<float>(largestSprite.height));
    for (auto& asteroid : m_Asteroids)
    {
        asteroid.Update(dt);
        if (!asteroid.IsDestroyed() && !asteroid.IsExploding() && m_Ship.GetBoundingCircle().Intersects(asteroid.GetBoundingCircle()))
        {
            if (COLLISIONS)
            {
                m_Ship.HitByAsteroid();
                m_PowerUps.clear();
            }
        }

        if (asteroid.IsOutOfBounds(largestSpriteSize))
        {
            Core::Vec2D pos = GetSpawnLocation();
            asteroid.SetPosition(pos);
            asteroid.SetVelocity(GetSpawnVelocity(pos));
        }
    }

    const auto& ammo = m_Ship.GetAmmoInFlight();
    std::vector<size_t> ammoToAddBack;

    for (const auto& ammoInstance : ammo)
    {
        std::vector<Asteroid> asteroidsToAdd;

        bool missileHit = false;
        size_t asteroidIndex = 0;

        std::vector<Core::Vec2D> intersections = {};
        intersections.reserve(10);

        for (size_t i = 0; i < m_Asteroids.size(); ++i)
        {
            const Asteroid& asteroid = m_Asteroids[i];

            if (!asteroid.IsDestroyed() && !asteroid.IsExploding() && ammoInstance->Intersects(asteroid.GetBoundingCircle(), intersections))
            {
                missileHit = true;
                asteroidsToAdd = BreakAsteroid(intersections[0], ammoInstance->GetVelocity(), asteroid, ammoInstance->Scale());
                asteroidIndex = i;

                m_Score += ScoreValue(asteroid.GetSize());
                break;
            }
            else
                intersections.clear();
        }

        if (missileHit)
        {
            if (SCREEN_SHAKE)
                Core::Application::Get().GetWindow().Shake(SCREEN_SHAKE_POWER, SCREEN_SHAKE_TIME);

            m_Asteroids[asteroidIndex].HitByAmmo(intersections);
        }

        if (missileHit && asteroidsToAdd.size() == 0)
            MaybeSpawnPowerUp(intersections[0]);

        if ((missileHit && ammoInstance->DisappearsOnHit()) || AmmoOutOfBounds(ammoInstance))
            ammoToAddBack.push_back(ammoInstance->GetID());

        m_Asteroids.insert(m_Asteroids.end(), asteroidsToAdd.begin(), asteroidsToAdd.end());
    }

    for (auto ammoInstance : ammoToAddBack)
    {
        m_Ship.RegainAmmo(ammoInstance);
    }

    SetupAsteroids();
}

PowerUp AsteroidsScene::MakePowerUp(PowerUpType type, const Core::Vec2D& spawnLocation, const Core::Vec2D& velocity)
{
    switch (type)
    {
        case PowerUpType::Grow:
        {
            PowerUp growPowerUp;
            growPowerUp.Init(m_AsteroidsSprites, "grow", spawnLocation, velocity, POWER_UP_LIFE_TIME, [this]()
            {
                m_Ship.Grow();
            });

            return growPowerUp;
        }
        case PowerUpType::Laser:
        {
            PowerUp laserPowerUp;
            laserPowerUp.Init(m_AsteroidsSprites, "laser", spawnLocation, velocity, POWER_UP_LIFE_TIME, [this]()
            {
                m_Ship.SwitchToLaser();
            });

            return laserPowerUp;
        }
        default:
            assert(false && "Invalid power up type");
            break;
    }

    return PowerUp();
}

void AsteroidsScene::SpawnPowerUp(const Core::Vec2D& spawnLocation, const Core::Vec2D& velocity)
{
    static std::uniform_int_distribution<size_t> powerUpTypesDistribution(static_cast<int>(PowerUpType::Grow), static_cast<int>(PowerUpType::Num) - 1);

    PowerUpType type = static_cast<PowerUpType>(powerUpTypesDistribution(m_RandomGenerator));
    m_PowerUps.push_back(MakePowerUp(type, spawnLocation, velocity));
}

void AsteroidsScene::MaybeSpawnPowerUp(const Core::Vec2D& spawnLocation)
{
    static std::uniform_int_distribution<size_t> powerUpSpawnDistribution(0, 99);
    size_t spawnRNG = powerUpSpawnDistribution(m_RandomGenerator);
    if (spawnRNG >= 80)
    {
        static std::uniform_real_distribution<float> powerUpDirectionDistribution(0.0f, Core::TWO_PI);
        float angle = powerUpDirectionDistribution(m_RandomGenerator);
        Core::Vec2D velocity(POWER_UP_SPEED * static_cast<float>(std::cos(angle)), POWER_UP_SPEED * static_cast<float>(std::sin(angle)));
        SpawnPowerUp(spawnLocation, velocity);
    }
}

void AsteroidsScene::DrawScore(Core::Window& window, float windowWidth, float windowHeight, const Core::BitmapFont& font, Core::Vec2D& textDrawPosition, Core::DrawTransform& transform, Core::ColorParams& colorParams, Core::UVParams& uvParams)
{
    Core::Rectangle highScoreRect = Core::Rectangle(Core::Vec2D(0.f, 2.f), static_cast<uint32_t>(windowWidth / 2.f), 20);

    textDrawPosition = font.GetDrawPosition(SCORE_STR, highScoreRect, Core::BitmapFontXAlignment::Left, Core::BitmapFontYAlignment::Center);
    textDrawPosition = textDrawPosition + Core::Vec2D(5.f, 0.f);
    transform.Pos = textDrawPosition;

    window.Draw(font, SCORE_STR, transform, colorParams, uvParams);

    highScoreRect = Core::Rectangle(Core::Vec2D(windowWidth / 2.f, 2.f), static_cast<uint32_t>(windowWidth / 2.f), 20);
    std::string scoreStr = std::to_string(m_Score);

    textDrawPosition = font.GetDrawPosition(scoreStr, highScoreRect, Core::BitmapFontXAlignment::Right, Core::BitmapFontYAlignment::Center);
    textDrawPosition = textDrawPosition - Core::Vec2D(5, 0);

    transform.Pos = textDrawPosition;
    window.Draw(font, scoreStr, transform, colorParams, uvParams);
}
