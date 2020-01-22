//
// Created by connor on 10/9/19.
//
#include "gtest/gtest.h"

#include <model.hpp>

namespace optimystic {
  class ModelTest : public ::testing::Test {
  protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    ModelTest() {
      // You can do set-up work for each test here.
    }

    ~ModelTest() override {
      // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
      // Code here will be called immediately after the constructor (right
      // before each test).
    }

    void TearDown() override {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }
  };


  TEST_F(ModelTest, OptimizationTest) {
      // Create an empty model
      Model model = Model();
      model.setLogFile("miplog1.log");

      // Create variables
      auto x = model.addVar(0.0, 1.0, 1.0, SOLVER_BINARY, "x");
      auto y = model.addVar(0.0, 1.0, 1.0, SOLVER_BINARY, "y");
      auto z = model.addVar(0.0, 1.0, 2.0, SOLVER_BINARY, "z");

      // Add constraint: x + 2 y + 3 z <= 4
      std::vector<double> coeffs1 = {1.0, 2.0, 3.0};
      std::vector<std::shared_ptr<Variable>> variables1 = {x, y, z};
      model.addConstr(variables1, coeffs1, SOLVER_LESS_EQUAL, 4.0, "c0");

      // Add constraint: x + y >= 1
      std::vector<double> coeffs2 = {1.0, 1.0};
      std::vector<std::shared_ptr<Variable>> variables2 = {x, y};
      model.addConstr(variables2, coeffs2, SOLVER_GREATER_EQUAL, 1.0, "c1");

      // Optimize model
      model.setSense(SOLVER_MAXIMIZE);
      model.optimize();

      std::cout << "X value: " << x->getValue() << std::endl;
      std::cout << "Y value: " << y->getValue() << std::endl;
      std::cout << "Z value: " << z->getValue() << std::endl;

      std::cout << "Obj: " << model.getObjectiveValue() << std::endl;
      bool correct = false;
      if(x->getValue() == 1.0 && y->getValue() == 0.0 && z->getValue() == 1.0) {
        correct = true;
      }
      ASSERT_TRUE(correct);
  }

}