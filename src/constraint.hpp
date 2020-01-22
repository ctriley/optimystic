//
// Created by connor on 9/20/19.
//

#ifndef DISPATCHER_CONSTRAINT_HPP
#define DISPATCHER_CONSTRAINT_HPP

#ifdef GUROBI
#include <gurobi_c++.h>
#elif defined CPLEX
#include <ilcplex/ilocplex.h>
#endif

#include "variable.hpp"

namespace optimystic {
  class Constraint {
  private:
#ifdef GUROBI
    std::shared_ptr<GRBConstr> _grb_constr;
#elif defined CPLEX
    std::shared_ptr<IloEnv> _cplex_env;
    std::shared_ptr<IloModel> _cplex_model;
    std::shared_ptr<IloCplex> _cplex_solver;
    std::shared_ptr<IloRange> _cplex_constr;
#endif
  public:
    Constraint() = default;
#ifdef GUROBI
    explicit Constraint(std::shared_ptr<GRBConstr> constr);
    std::shared_ptr<GRBConstr> getGRBConstr();
#elif defined CPLEX
    Constraint(std::shared_ptr<IloEnv> env, std::shared_ptr<IloModel> model,
        std::shared_ptr<IloCplex> solver,
        std::shared_ptr<IloRange> constr);
    std::shared_ptr<IloRange> getCplexConstraint();
#endif
    double getDualValue();
    void setName(const std::string& name);
  };

}

#endif //DISPATCHER_CONSTRAINT_HPP
