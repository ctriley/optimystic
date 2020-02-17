//
// Created by connor on 9/20/19.
//

#ifndef DISPATCHER_MODEL_HPP
#define DISPATCHER_MODEL_HPP


#ifdef GUROBI
#include <gurobi_c++.h>
#endif

#ifdef CPLEX
#include <ilcplex/ilocplex.h>
#include <vector>
#include <map>
#endif

#include "variable.hpp"
#include "constraint.hpp"

namespace optimystic {

  const char SOLVER_BINARY = 'B';
  const char SOLVER_INTEGER = 'I';
  const char SOLVER_CONTINUOUS = 'C';
  const char SOLVER_EQUAL = '=';
  const char SOLVER_LESS_EQUAL = '<';
  const char SOLVER_GREATER_EQUAL = '>';
  const double SOLVER_INFINITY = 1e+100;
  const int SOLVER_MINIMIZE = 1;
  const int SOLVER_MAXIMIZE = -1;
  const int SOLVER_OPTIMAL = 2;
  const int SOLVER_TIME_LIMIT = 9;
  const int SOLVER_INFEASIBLE_OR_UNBOUNDED = 4;
  const int SOLVER_INFEASIBLE = 3;
  const int SOLVER_UNBOUNDED = 5;

#ifdef CPLEX
  const std::map<char, IloNumVar::Type> CPLEX_TYPE_CONVERSION = {
      {SOLVER_INTEGER, IloNumVar::Type::Int},
      {SOLVER_CONTINUOUS, IloNumVar::Type::Float},
      {SOLVER_BINARY, IloNumVar::Type::Bool}
  };
  
  const std::map<int, int> CPLEX_STATUS_CONVERSION = {
    {IloAlgorithm::Status::Optimal, SOLVER_OPTIMAL},
    {IloAlgorithm::Status::InfeasibleOrUnbounded, SOLVER_INFEASIBLE_OR_UNBOUNDED},
    {IloAlgorithm::Status::Infeasible, SOLVER_INFEASIBLE},
    {IloAlgorithm::Status::Unbounded, SOLVER_UNBOUNDED},
    {IloAlgorithm::Status::Feasible, SOLVER_TIME_LIMIT}};
#endif

 /* template<typename Fn, Fn fn, typename... Args>
  typename std::result_of<Fn(Args...)>::type
  wrapper(Args&&... args) {
    return fn(std::forward<Args>(args)...);
  }
#define WRAPPER(FUNC) wrapper<decltype(&FUNC), &FUNC>

  template <typename X, int (X::*fn)(int, voidptr_t[], void*)>
  int wrap_function(int argc, std::voidptr_t args[], void *data) {
    return (static_cast<X*>(args[0])->*fn)(argc-1, &args[1], data);
  }*/


  /*template<typename Fn, Fn fn, typename... Args>
  typename std::result_of<Fn(Args...)>::type
  wrapper(Args... args) {
    try {
      return fn(std::forward<Args>(args)...);
    }
#ifdef GUROBI
    catch (GRBException &e) {
      std::cout << "Catching gurobi error from server" << std::endl;
      std::cout << "Error code = " << e.getErrorCode() << std::endl;
      std::cout << e.getMessage() << std::endl;
    } catch (...) {
      std::cout << "Exception during model prep" << std::endl;
    }
#elif defined CPLEX
    catch (IloException &e) {
      std::cout << "Exception: " << e.getMessage() << std::endl;
    } catch (...) {
      std::cout << "exception" << std::endl;
    }
#endif
  }*/


  class Model {
    public:
      Model();
      virtual ~Model();
      std::shared_ptr<Variable> addVar(double lb, double ub, double obj,
          char var_type, std::string name);
      std::shared_ptr<Constraint> addConstr(const std::vector<std::shared_ptr<Variable>>& vars,
          const std::vector<double>& coeffs, char sense,
          double rhs, const std::string& name="");
      void optimize();
      void update();
      int getStatus();
      void setTimeLimit(double limit);
      void setLogFile(const std::string& filename);
      void setOptimalityTol(double epsilon);
      double getObjectiveValue();
      int getNumVars();
      void write(const std::string &filename);
      void chgCoeff(const std::shared_ptr<Constraint>& constraint,
          const std::shared_ptr<Variable>& new_var, double coeff);
      void setOutput(bool b);
      void setSense(int s);
      void addQuadraticTermToObjective(const std::vector<double> &coeffs,
          const std::vector<std::shared_ptr<Variable>> &vars1,
          const std::vector<std::shared_ptr<Variable>> &vars2);
    private:
#ifdef GUROBI
      GRBModel* _grb_model;
#endif
#ifdef CPLEX
      std::shared_ptr<IloEnv> _cplex_env;
      std::shared_ptr<IloModel> _cplex_model;
      std::shared_ptr<IloCplex> _cplex_solver;
      IloExpr _cplex_objective_expr;
      IloObjective _cplex_obj;
      int _sense;
#endif
  };

}

#endif //DISPATCHER_MODEL_HPP
