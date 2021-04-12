#pragma once

#include "../ecs/Coordinator.h"
#include "../Components.h"
#include "../BasicEntities.h"

extern Coordinator gCoordinator;

class Fluids : public System
{
public:
    void update();

private:
    void Vstep(Fluid2D& fluid);
    void Sstep(Fluid2D& fluid);

    void addSource(Fluid2D& fluid);
    void diffuse(Fluid2D& fluid, float visc, std::vector<glm::vec3>& X, std::vector<glm::vec3>& X0);
    void advect(Fluid2D& fluid, std::vector<glm::vec3>& U, std::vector<glm::vec3>& Uprev);
    void setBnd(Fluid2D& fluid, std::vector<glm::vec3>& U);

    void updateRender(Fluid2D& fluid, Transform& transform);
};