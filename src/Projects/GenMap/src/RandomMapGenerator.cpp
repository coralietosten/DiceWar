#include "interface_lib.h"
#include "MapLoader.h"
#include "RandomMap.h"

API_EXPORT void InitMap(SInfo* info)
{
	info->SetName("Random map");
	info->AddMember("Augustin Bernard");
}

API_EXPORT IMap* MapFactory()
{
	IMap* map=new MapLoader();
	return(map);
}

API_EXPORT void MapDelete(IMap* obj)
{
	delete(obj);
}
