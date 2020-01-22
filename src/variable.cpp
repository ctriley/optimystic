//
// Created by connor on 9/20/19.
//

#include "variable.hpp"

#include <utility>

#include "model.hpp"

namespace optimystic {
#ifdef GUROBI
  Variable::Variable(std::shared_ptr<GRBVar> var): _grb_var(std::move(var)) {}

  std::shared_ptr<GRBVar> Variable::getGRBVar() {
    return _grb_var;
  }
#elif defined CPLEX
  Variable::Variable(std::shared_ptr<IloEnv> env, std::shared_ptr<IloModel> model,
      std::shared_ptr<IloCplex> solver,
      std::shared_ptr<IloNumVar> var): _cplex_env(std::move(env)),
      _cplex_model(std::move(model)), _cplex_solver(std::move(solver)),
      _cplex_var(std::move(var)) {}

  std::shared_ptr<IloNumVar> Variable::getCplexVar() {
    return _cplex_var;
  }
#endif

  void Variable::relax() {
#ifdef GUROBI
    _grb_var->set(GRB_CharAttr_VType, 'C');
#elif defined CPLEX
    IloConversion conversion(*_cplex_env, *_cplex_var,
                  CPLEX_TYPE_CONVERSION.at('C'));
    _conversion = conversion;
    _cplex_model->add(conversion);
#endif
  }

  void Variable::unRelax() {
#ifdef GUROBI
    _grb_var->set(GRB_CharAttr_VType, 'I');
#elif defined CPLEX
    _conversion.end();
#endif
  }

  double Variable::getReducedCost() {
#ifdef GUROBI
    return _grb_var->get(GRB_DoubleAttr_RC);
#elif defined CPLEX
    return _cplex_solver->getReducedCost(*_cplex_var);
#endif
  }

  std::string Variable::getName() const {
#ifdef GUROBI
    return _grb_var->get(GRB_StringAttr_VarName);
#elif defined CPLEX
    return _cplex_var->getName();
#endif
  }

  double Variable::getValue() {
#ifdef GUROBI
    return _grb_var->get(GRB_DoubleAttr_X);
#elif defined CPLEX
    return _cplex_solver->getValue(*_cplex_var);
#endif
  }

}