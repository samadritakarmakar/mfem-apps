# Copyright (c) 2010, Lawrence Livermore National Security, LLC. Produced at the
# Lawrence Livermore National Laboratory. LLNL-CODE-443211. All Rights reserved.
# See file COPYRIGHT for details.
#
# This file is part of the MFEM library. For more information and source code
# availability see http://mfem.org.
#
# MFEM is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License (as published by the Free
# Software Foundation) version 2.1 dated February 1999.

# Sets the following variables
#   - SuiteSparse_FOUND
#   - SuiteSparse_INCLUDE_DIRS
#   - SuiteSparse_LIBRARIES
#
# We need the following libraries:
#   umfpack, cholmod, amd, camd, colamd, ccolamd, suitesparseconfig, klu, btf
#

include(MfemCmakeUtilities)
mfem_find_package(SuiteSparse SuiteSparse SuiteSparse_DIR "" "" "" ""
  "Paths to headers required by SuiteSparse."
  "Libraries required by SuiteSparse."
  ADD_COMPONENT "UMFPACK" "include;suitesparse" umfpack.h "lib" umfpack
  ADD_COMPONENT "KLU" "include;suitesparse" klu.h "lib" klu
  ADD_COMPONENT "AMD" "include;suitesparse" amd.h "lib" amd
  ADD_COMPONENT "BTF" "include;suitesparse" btf.h "lib" btf
  ADD_COMPONENT "CHOLMOD" "include;suitesparse" cholmod.h "lib" cholmod
  ADD_COMPONENT "COLAMD" "include;suitesparse" colamd.h "lib" colamd
  ADD_COMPONENT "CAMD" "include;suitesparse" camd.h "lib" camd
  ADD_COMPONENT "CCOLAMD" "include;suitesparse" ccolamd.h "lib" ccolamd
  ADD_COMPONENT "config" "include;suitesparse" SuiteSparse_config.h "lib"
    suitesparseconfig)

if (SuiteSparse_FOUND AND METIS_VERSION_5)
  message(STATUS " *** Warning: using SuiteSparse with METIS v5!")
endif()
