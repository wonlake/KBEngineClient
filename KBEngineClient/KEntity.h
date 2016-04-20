#pragma once

#include "KMailbox.h"
#include "KDataTypes.h"
#include "KEntitydef.h"

namespace KBEngineClient
{

	class KBEngineApp;
	//the base game object
	class Entity
	{
	public:
		int32 id = 0;
		std::string classtype;
		Vector3 position = { 0.0f, 0.0f, 0.0f };
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
		float velocity = 0.0f;
		
		bool isOnGround = true;
		
		
		MailBox* baseMailbox = nullptr;
		MailBox* cellMailbox = nullptr;
		
		bool inWorld = false;
		PropertyMap alldefpropertys;
		PropertyMap defpropertys_;
		PropertyIdMap iddefpropertys_;
		
		Entity():inWorld(false),id(0),velocity(0.0f),isOnGround(true)
		{
		}

		virtual void onUpdatePropertys(MemoryStream& s){};
		
		virtual void destroy ()
		{
		}
		
		bool isPlayer();
				
		void addDefinedPropterty(std::string name, object v)
		{
			Property newp;
			newp.name = name;
			newp.properUtype = 0;
			newp.val = v;
			newp.setmethod = "";
			defpropertys_[name] = &newp;
		}
		
		object getDefinedPropterty(std::string name)
		{
			PropertyMapItr it = defpropertys_.find(name);
			if ( it != defpropertys_.end())
				return defpropertys_[name]->val;
            return 0;
		}
		
		void setDefinedPropterty(std::string name, object val)
		{
			defpropertys_[name]->val = val;
		}
		
		object getDefinedProptertyByUType(uint16 utype)
		{
			PropertyIdMapItr it = iddefpropertys_.find(utype);
			if ( it != iddefpropertys_.end())
				return iddefpropertys_[utype]->val;
			
            return 0;
		}
		
		void setDefinedProptertyByUType(uint16 utype, object val)
		{
			iddefpropertys_[utype]->val = val;
		}
		
		virtual void __init__()
		{
		}

		virtual void baseCall(std::string methodname, MemoryStream& stream)
		{
			Method& method = * EntityDef::moduledefs[classtype].base_methods[methodname];
			uint16 methodID = method.methodUtype;
			baseMailbox->newMail();
			baseMailbox->bundle->writeUint16(methodID);
			if( stream.opsize() > 0 ) //no paramter works. bugs when has paramter. 
				baseMailbox->bundle->writeStream( &stream );
			baseMailbox->postMail(0);
		}

		virtual void cellCall(std::string methodname, MemoryStream& stream)
		{
			Method& method = *EntityDef::moduledefs[classtype].cell_methods[methodname];
			uint16 methodID = method.methodUtype;
			cellMailbox->newMail();
			cellMailbox->bundle->writeUint16(methodID);
			if (stream.opsize() > 0) //no paramter works. bugs when has paramter. 
				cellMailbox->bundle->writeStream(&stream);
			cellMailbox->postMail(0);
		}
	
		virtual void enterWorld()
		{
			inWorld = true;
		}
		
		virtual void leaveWorld()
		{
			inWorld = false;
		}
		
		virtual void set_position(object old)
		{
		}

		virtual void set_direction(object old)
		{
		}

		virtual void onRemoteMethodCall( std::string methodname, MemoryStream& s )
		{
			std::cout << "oh no, please check it!" << std::endl;
		}
	};

};

