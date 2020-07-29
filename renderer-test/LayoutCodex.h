#pragma once
#include "DynamicConstant.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace dynamical
{
	class LayoutCodex
	{
	public:
		static dynamical::CookedLayout Resolve(dynamical::RawLayout&& layout) noxnd;
	private:
		static LayoutCodex& Get_() noexcept;
		std::unordered_map<std::string, std::shared_ptr<dynamical::LayoutElement>> map;
	};
}