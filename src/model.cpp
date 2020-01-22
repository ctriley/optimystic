//
// Created by connor on 9/20/19.
//

#include "model.hpp"

#include <utility>

#ifdef GUROBI
#include "gurobienv.hpp"
#endif

namespace optimystic {
  Model::Model() {
#ifdef GUROBI
    _grb_model = new GRBModel(*GurobiEnv::getInstance().getEnv());
#elif defined CPLEX
    _cplex_env = std::make_shared<IloEnv>();
    _cplex_model = std::make_shared<IloModel>(*_cplex_env);
    _cplex_solver = std::make_shared<IloCplex>(*_cplex_env);
    _cplex_objective_expr = IloExpr(*_cplex_env);
    _cplex_obj = IloMinimize(*_cplex_env, _cplex_objective_expr);
    _cplex_model->add(_cplex_obj);
#endif
  }

  Model::~Model() {
#ifdef GUROBI
    delete _grb_model;
#elif defined CPLEX
    _cplex_env->end();
#endif
  }

  std::shared_ptr<Variable> Model::addVar(double lb, double ub, double obj,
      char var_type, std::string name) {
#ifdef GUROBI
    GRBVar grb_var = _grb_model->addVar(lb, ub, obj, var_type, std::move(name));
    std::shared_ptr<GRBVar> grb_var_shared = std::make_shared<GRBVar>(grb_var);
    return std::make_shared<Variable>(grb_var_shared);
#elif defined CPLEX
    IloNumVar var(*_cplex_env, lb, ub, CPLEX_TYPE_CONVERSION.at(var_type),
        name.c_str());
    _cplex_objective_expr += (obj * var);
    auto cplex_shared_var = std::make_shared<IloNumVar>(var);
    return std::make_shared<Variable>(_cplex_env, _cplex_model, _cplex_solver,
        cplex_shared_var);
#endif
  }

  std::shared_ptr<Constraint> Model::addConstr(const std::vector<std::shared_ptr<Variable>>& vars,
      const std::vector<double>& coeffs, char sense,
      double rhs, const std::string& name) {
#ifdef GUROBI
    GRBLinExpr expr;
    std::vector<GRBVar> grb_vars;
    for(auto var : vars) {
      grb_vars.push_back(*(var->getGRBVar()));
    }
    expr.addTerms(&coeffs[0], &grb_vars[0], (int) vars.size());
    GRBConstr constraint = _grb_model->addConstr(expr, sense, rhs, name);
    std::shared_ptr<GRBConstr> grb_constr_shared = std::make_shared<GRBConstr>(constraint);
    return std::make_shared<Constraint>(grb_constr_shared);
#elif defined CPLEX
    IloExpr expr(*_cplex_env);
  for(int i = 0; i < vars.size(); ++i) {
    expr += (coeffs[i] * (*vars[i]->getCplexVar()));
  }
  IloRange constraint;
  switch(sense) {
    case '>': constraint = (expr >= rhs);
      break;
    case '<': constraint = (expr <= rhs);
      break;
    case '=': constraint = (expr == rhs);
      break;
    default: throw std::logic_error("No matching sense");
      break;
  }
  _cplex_model->add(constraint);
  std::shared_ptr<IloRange> shared_constraint = std::make_shared<IloRange>(constraint);
  return std::make_shared<Constraint>(_cplex_env, _cplex_model, _cplex_solver,
      shared_constraint);
#endif
  }

  int Model::getStatus() {
#ifdef GUROBI
    return _grb_model->get(GRB_IntAttr_Status);
#elif defined CPLEX
    return CPLEX_STATUS_CONVERSION.at(_cplex_solver->getStatus());
#endif
  }

  void Model::setTimeLimit(double limit) {
#ifdef GUROBI
    _grb_model->getEnv().set(GRB_DoubleParam_TimeLimit, limit);
#elif defined CPLEX
    _cplex_solver->setParam(IloCplex::TiLim, limit);
#endif
  }

  void Model::setLogFile(const std::string& filename) {
#ifdef GUROBI
    _grb_model->getEnv().set(GRB_StringParam_LogFile,
        filename);
#elif defined CPLEX
#endif
  }

  void Model::optimize() {
#ifdef GUROBI
    _grb_model->optimize();
#elif defined CPLEX
    update();
    _cplex_solver->solve();
#endif
  }

  void Model::update() {
#ifdef GUROBI
    _grb_model->update();
#elif defined CPLEX
    _cplex_solver->clearModel();
    _cplex_model->remove(_cplex_obj);
    _cplex_obj = IloMaximize(*_cplex_env, _cplex_objective_expr);
    if(_sense < 0) {
      _cplex_obj = IloMaximize(*_cplex_env, _cplex_objective_expr);
    }
    _cplex_model->add(_cplex_obj);
    _cplex_solver->extract(*_cplex_model);
#endif
  }

  double Model::getObjectiveValue() {
#ifdef GUROBI
    return _grb_model->get(GRB_DoubleAttr_ObjVal);
#elif defined CPLEX
    return _cplex_solver->getObjValue();
#endif
  }

  void Model::setOptimalityTol(double epsilon) {
#ifdef GUROBI
    _grb_model->getEnv().set(GRB_DoubleParam_OptimalityTol, epsilon);
#elif defined CPLEX
    _cplex_solver->setParam(IloCplex::Param::Simplex::Tolerances::Feasibility, epsilon);
#endif
  }

  void Model::write(const std::string &filename) {
#ifdef GUROBI
    _grb_model->write(filename);
#elif defined CPLEX
    _cplex_solver->exportModel(filename.c_str());
#endif
  }

  int Model::getNumVars() {
#ifdef GUROBI
    return _grb_model->get(GRB_IntAttr_NumVars);
#elif defined CPLEX
    return _cplex_solver->getNcols();
#endif
  }

  void Model::chgCoeff(const std::shared_ptr<Constraint>& constraint,
      const std::shared_ptr<Variable>& new_var, double coeff) {
#ifdef GUROBI
    _grb_model->chgCoeff(*constraint->getGRBConstr(),
        *new_var->getGRBVar(), coeff);
#elif defined CPLEX
    _cplex_model->remove(*constraint->getCplexConstraint());
    IloExpr new_expr = constraint->getCplexConstraint()->getExpr();
    new_expr += (coeff * (*new_var->getCplexVar()));
    constraint->getCplexConstraint()->setExpr(new_expr);
    _cplex_model->add(*constraint->getCplexConstraint());
#endif
  }

  void Model::setOutput(bool b) {
    if(!b) {
#ifdef GUROBI
      _grb_model->getEnv().set(GRB_IntParam_OutputFlag, 0);
#elif defined CPLEX
      _cplex_solver->setOut(_cplex_env->getNullStream());
#endif
    }
  }

  void Model::setSense(int s) {
#ifdef GUROBI
    _grb_model->set(GRB_IntAttr_ModelSense, s);
#elif defined CPLEX
    _sense = s;
#endif
  }

}
