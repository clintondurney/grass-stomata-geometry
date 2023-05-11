#ifndef FEM_MEMBRANES_HPP
#define FEM_MEMBRANES_HPP

#include <MDXProcessFem.hpp>
#include <MeshProcessSystem.hpp>

namespace mdx
{
  class FemWedgesSolver;

  // Main model class
  class FemWedges : public Process
  {
  public:
    FemWedges(const Process &proc) : Process(proc) 
    {
      setName("Model/CCF/01 FEM Wedges");
      setDesc("FEM simulation of pressurized cell");

      addParm("Solver Process", "Name of solver process", "Model/CCF/02 FEM Solver");
    }

    bool initialize(QWidget *parent);
    bool step();
    bool rewind(QWidget *parent);
    bool finalize(QWidget *parent);

  private:
    Mesh *mesh = 0;
    QString ccName;
    CCStructure *cs = 0;

    FemWedgesSolver *solverProcess = 0;

    double growthTime = 0;
  };

  class FemWedgesSolver : public fem::FemSolver
  {
  public:
    FemWedgesSolver(const Process &proc) : fem::FemSolver(proc) 
    {
      setName("Model/CCF/02 FEM Solver");
      setDesc("FEM Simulation using wedge elements");

      // Update parameters with our own defaults
      addParm("Triangle Derivs", "Name of process that implements solver derivatives", "");
      addParm("Wedge Derivs", "Name of process that implements solver derivatives", "Model/CCF/03 Wedge Derivs");
      addParm("Pressure Derivs", "Name of process that implements solver derivatives", "Model/CCF/08 Pressure Derivs");
      addParm("Stress-Strain", "Name of process that calculates the stress/strain", "Model/CCF/05 Stress-Strain");
      addParm("Dirichlet Derivs", "Process for Dirichlet derivatives", "Model/CCF/10 Dirichlet Derivs");
    }
  };

  class FemWedgesWedgeDerivs : public fem::ElementDerivs
  {
  public:
    FemWedgesWedgeDerivs(const Process &proc) : ElementDerivs(proc) 
    {
      setName("Model/CCF/03 Wedge Derivs");

      setParmDefault("Element Type", "Linear Wedge");
      setParmDefault("Element Attribute", "Wedge Element");
    }
  };

  class FemWedgesRefCfg : public fem::SetRefCfg
  {
  public:
    FemWedgesRefCfg(const Process &proc) : SetRefCfg(proc) 
    {
      setName("Model/CCF/04 Reference Configuration");

      setParmDefault("Element Type", "Linear Wedge");
      setParmDefault("Element Attribute", "Wedge Element");
    }
  };

  class FemWedgesStressStrain : public fem::StressStrain
  {
  public:
    FemWedgesStressStrain(const Process &proc) : StressStrain(proc) 
    {
      setName("Model/CCF/05 Stress-Strain");

      setParmDefault("Element Type", "Linear Wedge");
      setParmDefault("Element Attribute", "Wedge Element");
    }
  };

  class FemWedgesSetMaterial : public fem::SetTransIsoMaterial
  {
  public:
    FemWedgesSetMaterial(const Process &proc) : SetTransIsoMaterial(proc) 
    {
      setName("Model/CCF/06 Material Properties");
    }
  };


  class FemWedgeAnisoDir : public fem::SetAnisoDir
  {
  public:
    FemWedgeAnisoDir(const Process &proc) : SetAnisoDir(proc) 
    {
      setName("Model/CCF/07 Set Aniso Dir");

      setParmDefault("Element Type", "Linear Wedge");
      setParmDefault("Element Attribute", "Wedge Element");
    }
  };

  class FemWedgesSetPressure : public fem::SetPressure
  {
  public:
    FemWedgesSetPressure(const Process &proc) : fem::SetPressure(proc) 
    {
      setName("Model/CCF/08 Set Pressure");
    }
  };

  class FemWedgesPressureDerivs : public fem::PressureDerivs
  {
  public:
    FemWedgesPressureDerivs(const Process &proc) : fem::PressureDerivs(proc) 
    {
      setName("Model/CCF/08 Pressure Derivs");
    }
  };

  class FemWedgeSetDirichlet : public fem::SetDirichlet
  {
    public:
      FemWedgeSetDirichlet(const Process &proc) : SetDirichlet(proc)
    {
      setName("Model/CCF/09 Set Dirichlet");
    }
  };

  class FemWedgeDirichletDerivs : public fem::DirichletDerivs
  {
    public:
      FemWedgeDirichletDerivs(const Process &proc) : DirichletDerivs(proc)
    {
      setName("Model/CCF/10 Dirichlet Derivs");
    }
  };
  
  class FemWedgesVisDirections : public fem::VisDirections
  {
  public:
    FemWedgesVisDirections(const Process &proc) : VisDirections(proc) 
    {
      setName("Model/CCF/11 Vis Directions");
    }
  };

  class FemWedgesSelectFaces : public Process
  {
  public:
    FemWedgesSelectFaces(const Process &proc) : Process(proc) 
    {
      setName("Model/CCF/12 Select Faces");
      setDesc("Select all faces <= to a distance from the origin");

      addParm("Distance", "Distance under which faces will be selected", "49.0");
    }
    bool run();
  };

  class SelectTriangleLayer : public Process
  {
  public:
    SelectTriangleLayer(const Process &proc) : Process(proc) 
    {
      setName("Model/CCF/21 Select Triangle Layer");
      setDesc("Select all triangles in layer of selected triangle in a wedge mesh");
    }
    bool run()
    {
      Mesh *mesh = currentMesh();
      if(!mesh) 
        throw QString("%1::run No current mesh").arg(name());

      QString ccName = mesh->ccName();
      if(ccName.isEmpty())
        throw QString("%1::run No cell complex").arg(name());

      CCStructure &cs = mesh->ccStructure(ccName);
      CCIndexDataAttr &indexAttr = mesh->indexAttr();
 
      mesh->updateProperties(ccName);
      return run(cs, indexAttr);
    }
    bool run(CCStructure &cs, CCIndexDataAttr &indexAttr);
  };

  class MergeEdges : public Process
  {
  public:
    MergeEdges(const Process &proc) : Process(proc) 
    {
      setName("Model/CCF/22 Merge Edges");
      setDesc("Merge edges to remove selected vertices");
    }
    bool run()
    {
      Mesh *mesh = currentMesh();
      if(!mesh) 
        throw QString("%1::run No current mesh").arg(name());

      QString ccName = mesh->ccName();
      if(ccName.isEmpty())
        throw QString("%1::run No cell complex").arg(name());

      CCStructure &cs = mesh->ccStructure(ccName);
      CCIndexDataAttr &indexAttr = mesh->indexAttr();
 
      mesh->updateAll(ccName);
      return run(cs, indexAttr);
    }
    bool run(CCStructure &cs, CCIndexDataAttr &indexAttr);
  };
}
#endif

