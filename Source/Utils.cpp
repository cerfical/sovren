#include "Utils.hpp"
#include <iomanip>

namespace RENI {
	std::ostream& operator<<(std::ostream& out, const Color& color) {
		const auto fmtFlags = out.flags();
		out << '#' << std::hex << std::setfill('0')
			<< std::setw(2) << static_cast<int>(color.r)
			<< std::setw(2) << static_cast<int>(color.g)
			<< std::setw(2) << static_cast<int>(color.b)
			<< std::setw(2) << static_cast<int>(color.a);
		out.flags(fmtFlags);
		return out;
	}
}