#include "ArcadePCH.h"
#include "GhostAI.h"
#include "Level.h"
#include "Pacman.h"

static constexpr uint64_t PEN_WAIT_DURATION = 5000;
static constexpr uint64_t SCATTER_MODE_DURATION = 10000;

GhostAI::GhostAI()
	: m_Ghost(nullptr)
{
}

void GhostAI::Init(Ghost& ghost, uint32_t lookAheadDistance, const Core::Vec2D scatterTarget, GhostName name)
{
	m_ScatterTarget = scatterTarget;
	m_LookAheadDistance = lookAheadDistance;
	m_Ghost = &ghost;
	m_Name = name;
	std::random_device r;
	m_AIRandomGenerator.seed(r());
	m_Timer = 0;
	SetState(GhostAIState::Scatter);
	m_LastState = GhostAIState::Scatter;
}

PacmanMovement GhostAI::Update(uint32_t dt, const Pacman& pacman, const Level& level, const std::vector<Ghost>& ghosts)
{
	if (m_Ghost)
	{
		if (m_State == GhostAIState::Scatter)
		{
			m_Timer += dt;
			if (m_Timer >= SCATTER_MODE_DURATION)
				SetState(GhostAIState::Chase);
		}

		PacmanMovement currentDirction = m_Ghost->GetMovementDirection();

		std::vector<PacmanMovement> tempDirections;

		std::vector<PacmanMovement> possibleDirection;
		possibleDirection = GetPerpendicularMovements(currentDirction);

		if (currentDirction != PacmanMovement::None)
			possibleDirection.push_back(currentDirction);

		for (const auto& pDirection : possibleDirection)
			tempDirections.push_back(pDirection);

		possibleDirection.clear();

		for (const auto& direction : tempDirections)
		{
			if (!level.WillCollide(m_Ghost->GetBoundingBox(), direction))
				possibleDirection.push_back(direction);
		}

		assert(possibleDirection.size() >= 0 && "Can't go anywhere!");

		std::sort(possibleDirection.begin(), possibleDirection.end(), [](const PacmanMovement& direction1, const PacmanMovement direction2)
		{
			return static_cast<int>(direction1) < static_cast<int>(direction2);
		});

		if (m_Ghost->IsVulnerable() && (m_State == GhostAIState::Scatter || m_State == GhostAIState::Chase))
		{
			std::uniform_int_distribution<size_t> distribution(0, possibleDirection.size() - 1);
			return possibleDirection[static_cast<int>(distribution(m_AIRandomGenerator))];
		}

		if (m_State == GhostAIState::Chase)
			ChangeTarget(GetChaseTarget(dt, pacman, level, ghosts));

		PacmanMovement directionToGoIn = PacmanMovement::None;
		uint32_t lowestDistance = UINT32_MAX;
		for (const auto& direction : possibleDirection)
		{
			Core::Vec2D movementVec = GetMovementVector(direction) * m_LookAheadDistance;
			Core::Rectangle bbox = m_Ghost->GetBoundingBox();
			bbox.MoveBy(movementVec);
			uint32_t distanceToTarget = bbox.GetCenterPoint().Distance(m_Target);
			if (distanceToTarget < lowestDistance)
			{
				directionToGoIn = direction;
				lowestDistance = distanceToTarget;
			}
		}
		assert(directionToGoIn != PacmanMovement::None);
		return directionToGoIn;
	}
	return PacmanMovement::None;
}

void GhostAI::Draw(Core::Window& window)
{
	if (m_Ghost)
	{
		Core::Circle targetCircle(m_Target, 4);
		window.Draw(targetCircle, m_Ghost->GetSpriteColor(), true, m_Ghost->GetSpriteColor());

		Core::Rectangle boundingBox = m_Ghost->GetBoundingBox();
		boundingBox.MoveBy(GetMovementVector(m_Ghost->GetMovementDirection()) * m_Ghost->GetBoundingBox().GetWidth());
		Core::Color c(m_Ghost->GetSpriteColor().GetRed(), m_Ghost->GetSpriteColor().GetGreen(), m_Ghost->GetSpriteColor().GetBlue(), 200);
		window.Draw(boundingBox, m_Ghost->GetSpriteColor(), true, c);
	}
}

void GhostAI::SetState(GhostAIState state)
{
	if (m_State == GhostAIState::Scatter || m_State == GhostAIState::Chase)
		m_LastState = m_State;

	m_State = state;
	switch (state)
	{
		case GhostAIState::InPen:
			break;
		case GhostAIState::ExitPen:
			break;
		case GhostAIState::GoToPen:
			break;
		case GhostAIState::Scatter:
			m_Timer = 0;
			ChangeTarget(m_ScatterTarget);
			break;
	}
}

void GhostAI::ChangeTarget(Core::Vec2D target)
{
	m_Target = target;
}

const Core::Vec2D& GhostAI::GetChaseTarget(uint32_t dt, const Pacman& pacman, const Level& level, const std::vector<Ghost>& ghosts)
{
	Core::Vec2D target;
	switch (m_Name)
	{
		case GhostName::Blinky:
			target = pacman.GetBoundingBox().GetCenterPoint();
			break;
		case GhostName::Pinky:
			target = pacman.GetBoundingBox().GetCenterPoint() + 2 * GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth();
			break;
		case GhostName::Inky:
		{
			Core::Vec2D pacmanOffsetPoint = pacman.GetBoundingBox().GetCenterPoint() + (GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth());
			target = (pacmanOffsetPoint - ghosts[static_cast<int>(GhostName::Blinky)].GetBoundingBox().GetCenterPoint()/* 2 + ghosts[static_cast<int>(GhostName::Blinky)].GetBoundingBox().GetCenterPoint()*/);
			break;
		}
		case GhostName::Clyde:
		{
			auto distanceToPacman = m_Ghost->GetBoundingBox().GetCenterPoint().Distance(pacman.GetBoundingBox().GetCenterPoint());
			if (distanceToPacman > pacman.GetBoundingBox().GetWidth() * 4)
				target = pacman.GetBoundingBox().GetCenterPoint();
			else
				target = m_ScatterTarget;
			break;
		}
	}
	return target;
}
