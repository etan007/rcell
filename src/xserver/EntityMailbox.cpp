#include "EntityMailbox.h"
#include "CellApp.h"
bool EntityMailBox::IsLocalServer() const
{
    return (m_nServerId == CellApp::getMe().GetServerID());
}
