#pragma once
#include "xLog.h"
#include "Entity.pb.h"
//#include "SStartConfig.h"
class EntityMailBox
{
public:
    EntityMailBox() {}
    EntityMailBox(const PBEntityMailbox& pBMailbox) {
        CopyFrom(pBMailbox);
    }
    EntityMailBox(uint64_t eId, uint32_t lobbyGateId)
    {
        m_eId = eId;
        m_nServerId = lobbyGateId;
    }

    bool IsOk() const { return (m_eId > 0) && (m_nServerId >0 ); }
    void CopyTo(PBEntityMailbox* pPBMailbox) const
    {
        pPBMailbox->set_entity_id(m_eId);
        pPBMailbox->set_server_id(m_nServerId);
    }

    void CopyFrom(const PBEntityMailbox& pBMailbox)
    {
        m_eId = pBMailbox.entity_id();
        m_nServerId = pBMailbox.server_id();
    }

    bool IsLocalServer() const
    {
       // todo sky
       // return (m_nServerId == SServerStartConfig::getMe().GetServerSGUID());
        return false;
    }

    void Clear()
    {
        m_eId = 0;
        m_nServerId = 0;
    }

public:
    uint64_t m_eId = 0;                    //实体ID
    uint32_t m_nServerId = 0;                    //所在Gate
};

typedef std::shared_ptr<EntityMailBox> EntityMailBoxPtr;