#pragma once

#include <xhash>

namespace Aura
{
    class UUID
    {
    public:
        UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other);

		operator const uint64_t() const { return m_uuid; }
		operator uint64_t() { return m_uuid; }

    private:
        uint64_t m_uuid;
    };


}

namespace std {

	template <>
	struct hash<::Aura::UUID>
	{
		std::size_t operator()(const ::Aura::UUID& uuid) const
		{
			// uuid is already a randomly generated number, and is suitable as a hash key as-is.
			// this may change in future, in which case return hash<uint64_t>{}(uuid); might be more appropriate
			return uuid;
		}
	};
}