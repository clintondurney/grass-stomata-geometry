#include "FemWedges.hpp"

namespace mdx
{

  bool FemWedges::initialize(QWidget *parent)
  {
    mesh = currentMesh();
    if(!mesh)
      throw(QString("FemWedges::initialize No current mesh"));

    ccName = mesh->ccName();
    if(ccName.isEmpty())
      throw(QString("FemWedges::initialize No cell complex"));

    cs = &mesh->ccStructure(ccName);

    // Get the solver process
    if(!getProcess(parm("Solver Process"), solverProcess))
      throw(QString("FemWedges::initialize Unable to make solver process: %1").arg(parm("Solver Process")));
    solverProcess->initialize(parent);

    return true;
  }

  bool FemWedges::step()
  {
    if(!solverProcess)
      throw(QString("FemWedges::run Solver process pointer invalid"));

    mesh->updatePositions(ccName);
    return solverProcess->step();
  }

  bool FemWedges::rewind(QWidget *parent)
  {
    // To rewind, we'll reload the mesh
    Mesh *mesh = currentMesh();
    if(!mesh or mesh->file().isEmpty())
      throw(QString("No current mesh, cannot rewind"));
    MeshLoad meshLoad(*this);
    meshLoad.setParm("File Name", mesh->file());
    growthTime = 0;
    return meshLoad.run();
  }

  bool FemWedges::finalize(QWidget *parent)
  {
    if(!solverProcess)
      throw(QString("FemWedges::run Solver process pointer invalid"));

    bool result = solverProcess->finalize(parent);

    return result;
  }

  bool FemWedgesSelectFaces::run()
  {
    Mesh *mesh = currentMesh();
    if(!mesh)
      throw QString("SelectFemWedgesFaces::run No current mesh");

    QString ccName = mesh->ccName();
    if(ccName.isEmpty())
      throw QString("SelectFemWedgesFaces::run No current cell complex");

    CCStructure &cs = mesh->ccStructure(ccName);
    double distance = parm("Distance").toDouble();

    for(CCIndex f : cs.faces()) {
      auto &fIdx = mesh->indexAttr()[f];
      if(norm(fIdx.pos) <= distance)
        fIdx.selected = true; 
    }
    mesh->updateProperties(ccName);
    return true;
  }

  REGISTER_PROCESS(FemWedges);
  REGISTER_PROCESS(FemWedgesSolver);
  REGISTER_PROCESS(FemWedgesRefCfg);
  REGISTER_PROCESS(FemWedgesStressStrain);
  REGISTER_PROCESS(FemWedgesWedgeDerivs);
  REGISTER_PROCESS(FemWedgesSetMaterial);
  REGISTER_PROCESS(FemWedgeAnisoDir);
  REGISTER_PROCESS(FemWedgesPressureDerivs);
  REGISTER_PROCESS(FemWedgesSetPressure);
  REGISTER_PROCESS(FemWedgesVisDirections);
  REGISTER_PROCESS(FemWedgesSelectFaces);

  bool SelectTriangleLayer::run(CCStructure &cs, CCIndexDataAttr &indexAttr)
  {
    // Find selected triangles
    CCIndexVec lastT, newT;
    for(CCIndex t : cs.faces())
      if(indexAttr[t].selected and cs.bounds(t).size() == 3)
        lastT.push_back(t);

    CCIndexSet triangles;
    triangles.insert(lastT.begin(), lastT.end());
    
    do {
      for(auto t : lastT)
        for(auto e : cs.bounds(t)) {
          for(CCIndex n : cs.cobounds(e))
            if(n == t)
              continue;
            else if(triangles.count(n) == 0 and cs.bounds(n).size() == 3) {
              newT.push_back(n);
              triangles.insert(n);
              indexAttr[n].selected = true;
            }
        }
      std::swap(lastT, newT);
      newT.clear();
    } while(!lastT.empty());

    mdxInfo << triangles.size() << " triangles in layer" << endl;

    return true;
  }
  REGISTER_PROCESS(SelectTriangleLayer);

  bool MergeEdges::run(CCStructure &cs, CCIndexDataAttr &indexAttr)
  {
    int count = 0;
    // Get vertices between edges to merge
    for(CCIndex v : selectedVertices(cs, indexAttr)) {
      auto cb = cs.cobounds(v);
      if(cb.size() != 2) {
        mdxInfo << "mergeFaces: Vertex " << v << " has more than 2 edges, skipping" << endl;
        continue;
      }
      bool skip = false;
      for(CCIndex e : cb) {
        for(CCIndex f : cs.cobounds(e))
          if(cs.bounds(f).size() < 4) {
            skip = true;
            break;
          }
        if(skip)
          break;
      }
      if(skip) {
        mdxInfo << "mergeFaces: Merging vertex " << v << " would create face with less than 3 edges, skipping" << endl;
        continue;
      }
      // Switch orientation if not the same
      auto e0 = *cb.begin();
      auto e1 = *cb.rbegin();
      if(cs.ro(v, e0) == cs.ro(v, e1))
        cs.reverseOrientation(e1);
      SplitStruct ss(CCIndexFactory.getIndex(), v, e0, e1);
      if(!cs.mergeCells(ss))
        mdxInfo << "mergeFaces: Merge of edges failed for vertex:" << v << endl;
      else
        count++;
    }
    setStatus(QString("%1 edges merged").arg(count));

    return true;
  }
  REGISTER_PROCESS(MergeEdges);

  REGISTER_PROCESS(FemWedgeSetDirichlet);
  REGISTER_PROCESS(FemWedgeDirichletDerivs);



}
