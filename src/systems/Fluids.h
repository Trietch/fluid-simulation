#pragma once

#include "../ecs/Coordinator.h"
#include "../Components.h"
#include "../BasicEntities.h"

#include <Eigen/Sparse>
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/util/Constants.h>
#include <Eigen/src/IterativeLinearSolvers/BasicPreconditioners.h>
#include <Eigen/src/SparseCore/SparseMatrix.h>
#include <cstdint>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <numeric>
#include <fstream>

extern Coordinator gCoordinator;


class Fluids : public System
{
public:
    void init(std::shared_ptr<Renderer> renderer);
    void update(std::uint32_t iteration);
    void reset(bool force = false);

#ifdef DEBUG_GUI
    void fluidDebugTool();
    void fluidSetupDebug();
#endif

private:
    void Vstep(Fluid3D& fluid);
    void Sstep(Fluid3D& fluid);

    void addSource(const Fluid3D& fluid, std::vector<double>& X, const std::vector<double>& S) const;
    void diffuse(const Fluid3D& fluid, std::vector<double>& X, const std::vector<double>& Xprev, const std::uint8_t b, const Laplacian& A);

    void advect(Fluid3D& fluid, std::vector<double>& D, const std::vector<double>& Dprev, const std::vector<double>& X, const std::vector<double>& Y, const std::vector<double>& Z, const std::uint8_t b) const;
    void project(const Fluid3D& fluid, std::vector<double>& X, std::vector<double>& Y, std::vector<double>& Z, std::vector<double>& p, std::vector<double>& div);

    void GaussSeidelRelaxationLinSolve(const Fluid3D& fluid, std::vector<double>& X, std::vector<double>& Xprev, float a, float c, std::uint8_t b) const;
    void ConjugateGradientMethodLinSolve(const Fluid3D& fluid, std::vector<double>& X, const std::vector<double>& Xprev, const std::uint8_t bs, const Laplacian& A);

    void applyPreconditioner(const Fluid3D& fluid, const Eigen::VectorXd& r, const Laplacian& A, Eigen::VectorXd& z) const;
    //Eigen::SparseMatrix<double> ichol(const Eigen::SparseMatrix<double>& A) const;

    void setBnd(const Fluid3D& fluid, std::vector<double>& X, const std::uint8_t b) const;

    void updateRender(Fluid3D& fluid);
    void writeVolumeFile(Fluid3D& fluid, std::uint64_t iteration);

    std::shared_ptr<Renderer> _renderer = nullptr;
};

template<typename T>
void write(std::ofstream &f, T data)
{
    f.write(reinterpret_cast<const char *>(&data), sizeof(data));
}
