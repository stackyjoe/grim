#ifndef GRIM_TIMESTEPPER_H_
#define GRIM_TIMESTEPPER_H_

#include <petsc.h>
#include <petscviewerhdf5.h>
#include <unistd.h>
#include "../inputs.h"

#define EXPLICIT (0)
#define IMPLICIT (1)
#define IMEX     (2)

struct problemData;

struct timeStepper
{
  REAL t, dt, tDump;
  int timeStepCounter;
  int dumpCounter;

  SNES snes;
  DM dmdaWithGhostZones;
  DM dmdaWithoutGhostZones;
  DM connectionDMDA;
  DM dmdaDt;

  Vec primPetscVec;
  Vec residualPetscVec;
  Vec OldresidualPetscVec;
  Vec LastStepresidualPetscVec;
  Vec LambdaresidualPetscVec;
  Vec primPetscVecOld;
  Vec primPetscVecLastStep;
  Vec primPetscVecLambda;
  Vec primPetscVecInt;
  Vec primPetscVecHalfStep;
  Vec sourceTermsPetscVecOld;
  Vec divFluxPetscVecOld;
  Vec conservedVarsPetscVecOld;
  Vec connectionPetscVec;
  Vec dtPetscVec;

  #if (CONDUCTION)
    DM  gradTDM;
    DM  graduConDM;
    DM  graduConHigherOrderTermsDM;
    Vec gradTPetscVec;
    Vec graduConPetscVec;
    Vec graduConHigherOrderTerm1PetscVec;
  #endif
 
  #if (VISCOSITY)
    DM  graduConVisDM;
    DM  graduConHigherOrderTermsVisDM;
    Vec graduConVisPetscVec;
    Vec graduConHigherOrderTerm1VisPetscVec;
  #endif

  struct problemData *problemSpecificData;

  int computeOldSourceTermsAndOldDivOfFluxes;
  int computeDivOfFluxAtTimeN;
  int computeDivOfFluxAtTimeNPlusHalf;
  int computeSourceTermsAtTimeN;
  int computeSourceTermsAtTimeNPlusHalf;

  int X1Start, X1Size;
  int X2Start, X2Size;
  int X3Start, X3Size;
};

/* The problem library requires timeStepper struct, so put this include after
 * the definition of the struct */
#include "../geometry/geometry.h"
#include "../gridzone/gridzone.h"
#include "../boundary/boundary.h"
#include "../reconstruct/reconstruct.h"
#include "../riemannsolver/riemannsolver.h"
#include "../physics/physics.h"
#include "../problem/problem.h"

/* User functions */

void timeStepperInit(struct timeStepper ts[ARRAY_ARGS 1]);

void timeStep(struct timeStepper ts[ARRAY_ARGS 1]);

void timeStepperDestroy(struct timeStepper ts[ARRAY_ARGS 1]);

/* Internal functions */

PetscErrorCode computeResidual(SNES snes, 
                               Vec primPetscVec,
                               Vec residalPetscVec,
                               void *ptr);

void computeFluxesOverTile(const REAL primTile[ARRAY_ARGS TILE_SIZE],
                           const int iTile, const int jTile,
                           const int X1Start, const int X2Start,
                           const int X1Size, const int X2Size,
                           REAL fluxX1Tile[ARRAY_ARGS TILE_SIZE],
                           REAL fluxX2Tile[ARRAY_ARGS TILE_SIZE],
                           ARRAY(dtGlobal));

void setChristoffelSymbols(struct timeStepper ts[ARRAY_ARGS 1]);

void diagnostics(struct timeStepper ts[ARRAY_ARGS 1]);

#if (CONDUCTION)
  void initConductionDataStructures(struct timeStepper ts[ARRAY_ARGS 1]);
  void destroyConductionDataStructures(struct timeStepper ts[ARRAY_ARGS 1]);
#endif

#if (VISCOSITY)
  void initViscosityDataStructures(struct timeStepper ts[ARRAY_ARGS 1]);
  void destroyViscosityDataStructures(struct timeStepper ts[ARRAY_ARGS 1]);
#endif


#define WRITE_PARAM_INT(NAME) \
        do { \
        int __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_INT, &__NAME); \
        } while(0)


#define WRITE_PARAM_DOUBLE(NAME) \
        do { \
        double __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_DOUBLE, &__NAME); \
        } while(0)


#define WRITE_PARAM_COMPLEX(NAME) \
        do { \
        complex __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_COMPLEX, &__NAME); \
        } while(0)


#define WRITE_PARAM_LONG(NAME) \
        do { \
        long __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_LONG, &__NAME); \
        } while(0)


#define WRITE_PARAM_SHORT(NAME) \
        do { \
        short __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_SHORT, &__NAME); \
        } while(0)


#define WRITE_PARAM_FLOAT(NAME) \
        do { \
        float __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_FLOAT, &__NAME); \
        } while(0)


#define WRITE_PARAM_CHAR(NAME) \
        do { \
        char __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_CHAR, &__NAME); \
        } while(0)


#define WRITE_PARAM_STRING(NAME) \
        do { \
        string __NAME = NAME; \
        PetscViewerHDF5WriteAttribute(parametersViewer, \
        "/gammaUpdowndown", #NAME, PETSC_STRING, &__NAME); \
        } while(0)

#endif /* GRIM_TIMESTEPPER_H_ */
