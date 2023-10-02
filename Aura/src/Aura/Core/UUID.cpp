#include "pch.h"
#include "UUID.h"

#include <random>

namespace
{
	std::random_device s_RandDevice;
	std::mt19937_64 s_Engine(s_RandDevice());
	std::uniform_int_distribution<uint64_t> s_Dist;
}

namespace Aura
{


	UUID::UUID() : m_uuid(s_Dist(s_Engine))
	{

	}

	UUID::UUID(uint64_t uuid) : m_uuid(uuid)
	{

	}

	UUID::UUID(const UUID& other) : m_uuid(other.m_uuid)
	{

	}
}
