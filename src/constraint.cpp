//
// Created by connor on 9/20/19.
//

#include "constraint.hpp"

#include <utility>

namespace optimystic {

#ifdef GUROBI
  Constraint::Constraint(std::shared_ptr<GRBConstr> constr): _grb_constr(std::move(constr)) {}

  std::shared_ptr<GRBConstr> Constraint::getGRBConstr() {
    return _grb_constr;
  }
#elif defined CPLEX
  Constraint::Constraint(std::shared_ptr<IloEnv> env, std::shared_ptr<IloModel> model,
                         std::shared_ptr<IloCplex> solver,
                         std::shared_ptr<IloRange> constr): _cplex_env(env),
                         _cplex_model(model), _cplex_solver(solver),
                         _cplex_constr(constr) {}

  std::shared_ptr<IloRange> Constraint::getCplexConstraint() {
    return _cplex_constr;
  }

#endif
  double Constraint::getDualValue() {
#ifdef GUROBI
    return _grb_constr->get(GRB_DoubleAttr_Pi);
#elif defined CPLEX
    return _cplex_solver->getDual(*_cplex_constr);
#endif
  }

  void Constraint::setName(const std::string& name) {
#ifdef GUROBI
    _grb_constr->set(GRB_StringAttr_ConstrName, name);
#elif defined CPLEX
    _cplex_constr->setName(name.c_str());
#endif
  }


}
