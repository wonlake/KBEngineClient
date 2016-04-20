#include "KEntitydef.h"

namespace KBEngineClient
{
	std::map<std::string, KBEDATATYPE_BASE> EntityDef::datatypes;
	std::map<uint16, KBEDATATYPE_BASE> EntityDef::iddatatypes;
	std::map<std::string, int32> EntityDef::datatype2id;
	std::map<std::string, ScriptModule> EntityDef::moduledefs;
	std::map<uint16, ScriptModule> EntityDef::idmoduledefs;
	std::map<std::string, PropertyMap*> EntityDef::alldefpropertys;
	std::map<std::string, PropertyMap*> EntityDef::defpropertys_;
	std::map<uint16, PropertyMap*> EntityDef::iddefpropertys_;

	bool EntityDef::__entityAliasID;	// �Ż�EntityID��aoi��Χ��С��255��EntityID, ���䵽clientʱʹ��1�ֽ�αID 
	bool EntityDef::__entitydefAliasID;
}