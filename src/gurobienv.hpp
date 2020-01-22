/* Copyright (C) Connor Riley, Pascal Van Hentenryck - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Connor Riley <ctriley@gatech.edu>, June 2018
 */


#ifndef SRC_ALGORITHMS_GUROBIENV_HPP
#define SRC_ALGORITHMS_GUROBIENV_HPP

#ifdef GUROBI

#include "gurobi_c++.h"

namespace optimystic {
  class GurobiEnv {
    public:
      static GurobiEnv &getInstance() {
        static GurobiEnv instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
      }

      GRBEnv* getEnv() {
        return _env;
      }

    private:
      GurobiEnv() {
        _env = new GRBEnv();
      }
      ~GurobiEnv() {
        delete _env;
      }

      GRBEnv* _env;

    public:
      GurobiEnv(GurobiEnv const &) = delete;
      void operator=(GurobiEnv const &)  = delete;
  };
}
#endif //SRC_ALGORITHMS_GUROBIENV_HPP
#endif