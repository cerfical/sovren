#include "WinMsgQueue.hpp"
#include "WinUtils.hpp"

namespace RENI {
	const MSG& WinMsgQueue::GetMsg() {
		SafeWin32ApiCall(GetMessage, &m_msg, nullptr, 0, 0);
		return m_msg;
	}

	bool WinMsgQueue::IsEmpty() const {
		return SafeWin32ApiCall(
			PeekMessage, &m_msg, nullptr, 0, 0, PM_NOREMOVE
		) == 0;
	}
}