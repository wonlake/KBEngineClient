#include "KMailbox.h"
#include "KBEngineApp.h"

namespace KBEngineClient
{
	MailBox::MailBox(void) :id(0), classtype(""), bundle(0), type(MAILBOX_TYPE::MAILBOX_TYPE_CELL)
	{
		networkInterface_ = KBEngineApp::instance()._networkInterface.get();
	}
}