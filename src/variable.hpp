//
// Created by connor on 9/20/19.
//

#ifndef DISPATCHER_VARIABLE_HPP
#define DISPATCHER_VARIABLE_HPP

#include <string>
#include <memory>
#ifdef GUROBI
#include <gurobi_c++.h>
#elif defined CPLEX
#include <ilcplex/ilocplex.h>
#endif

namespace optimystic {
  class Variable {
  private:
#ifdef GUROBI
    std::shared_ptr<GRBVar> _grb_var;
#elif defined CPLEX
    std::shared_ptr<IloEnv> _cplex_env;
    std::shared_ptr<IloModel> _cplex_model;
    std::shared_ptr<IloCplex> _cplex_solver;
    std::shared_ptr<IloNumVar> _cplex_var;
    IloConversion _conversion;

#endif
  public:
    Variable() = default;
#ifdef GUROBI
    explicit Variable(std::shared_ptr<GRBVar> var);
#elif defined CPLEX
    Variable(std::shared_ptr<IloEnv> env, std::shared_ptr<IloModel> model,
        std::shared_ptr<IloCplex> solver,
        std::shared_ptr<IloNumVar> var);
#endif
    void relax();
    void unRelax();
    double getValue();
    double getReducedCost();
    std::string getName() const;
#ifdef GUROBI
    std::shared_ptr<GRBVar> getGRBVar();
#elif defined CPLEX
    std::shared_ptr<IloNumVar> getCplexVar();
#endif
  };
}

#endif //DISPATCHER_VARIABLE_HPP
