#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include "../../../Commun/library.h"
#include "../../../Commun/IInfoLib.h"

HLIB loadLib(const char *name)
{
	HLIB handle;

	handle = LOADLIB(name);
	if (!handle) return(nullptr);
	return(handle);
}

enum class ELibType
{
	Unknown,
	Gui,
	Map,
	Referee,
	Strategy
};

struct LibType
{
	static const char* GetLibTypeName(ELibType type)
	{
		switch (type)
		{
		case ELibType::Gui: return("Gui");
		case ELibType::Map: return("Map");
		case ELibType::Referee: return("Referee");
		case ELibType::Strategy: return("Strategy");
		}
		return(nullptr);
	}

	static const char* GetInitFuncName(ELibType type)
	{
		switch (type)
		{
		case ELibType::Gui: return("InitGui");
		case ELibType::Map: return("InitMap");
		case ELibType::Referee: return("InitReferee");
		case ELibType::Strategy: return("InitStrategy");
		}
		return(nullptr);
	}

	static pInitLib GetLibInfo(HLIB lib, ELibType &type)
	{
		static std::array<ELibType, 4> libTypes = { ELibType::Gui, ELibType::Map, ELibType::Referee, ELibType::Strategy };

		for (auto libType : libTypes)
		{
			pInitLib init;
			init = (pInitLib)GETPROC(lib, GetInitFuncName(libType));
			if (init)
			{
				type = libType;
				return(init);
			}
		}
		type = ELibType::Unknown;
		return(nullptr);
	}
};

class Info : public SInfo
{
public:
	inline void SetName(const char* name) { Name = name; }
	inline void AddMember(const char* name) { Members.push_back(name); }
	inline const char* GetName() const { return(Name.c_str()); } 
	inline std::size_t GetNbMembers() const { return(Members.size()); } 
	inline const char* GetMember(std::size_t i) const { return(Members[i].c_str()); } 
private:
	std::string Name;
	std::vector<std::string> Members;
};

int main()
{
	auto path = std::filesystem::current_path();

	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		if (!std::filesystem::is_regular_file(entry)) continue;
		if (entry.path().extension() != ".dll" && entry.path().extension() != ".so") continue;
		HLIB lib = loadLib(entry.path().string().c_str());
		if (!lib) continue;

		ELibType type = ELibType::Unknown;
		pInitLib init;
		init = LibType::GetLibInfo(lib, type);
		if (!init) continue;

		Info info;
		init(&info);
		std::cout << "File: " << entry.path().filename() << std::endl;
		std::cout << "Library type: " << LibType::GetLibTypeName(type) << std::endl;
		std::cout << "Library: " << info.GetName() << std::endl;
		std::cout << "Members: " << info.GetNbMembers() << std::endl;
		for (std::size_t i = 0; i < info.GetNbMembers(); ++i) std::cout << "\tMember #" << i << ": " << info.GetMember(i) << std::endl;
		std::cout << std::endl;

		CLOSELIB(lib);
	}

	return 0;
}