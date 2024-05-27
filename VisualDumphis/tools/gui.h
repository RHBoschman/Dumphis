#pragma once

#include <iostream>
#include <string>

namespace tls
{
	class Gui {
	public:
		static bool askClosedQuestion(const std::string& msg, const bool noNl = false);
		static std::string askOpenQuestion(const std::string& msg, const bool noNl = false);

	private:
	};
}
