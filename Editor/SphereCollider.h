#ifndef _SPHERECOLLIDER_H_
#define _SPHERECOLLIDER_H_

#include "Collider.h"

namespace UltraEd
{
    class SphereCollider : public Collider
    {
    public:
        SphereCollider();
        SphereCollider(const std::vector<Vertex> &vertices);
        void Build();
        FLOAT GetRadius() { return m_radius; }
        nlohmann::json Save();
        void Load(const nlohmann::json &root);

    private:
        void SphereFromDistPoints(D3DXVECTOR3 &center, FLOAT &radius, const std::vector<Vertex> &vertices);
        void AdjustSphere(D3DXVECTOR3 &center, FLOAT &radius, const Vertex &vertex);
        void FindCenterWithRadius(D3DXVECTOR3 &center, FLOAT &radius, const std::vector<Vertex> &vertices);

    private:
        float m_radius;
    };
}

#endif
