"""
Name: Leonid Shuster
Lab: 3
Role: Class to implement Dryden gust model and update wind
NOTE: Functions were conceptually talked about with Jimmy Chen, Xingyu Zhou, and Christian Sabile
"""

import math
import random
from ..Containers import States
from ..Containers import Inputs
from ..Utilities import MatrixMath
from ..Constants import VehiclePhysicalConstants as VPC

class WindModel():
    def __init__(self, dT=VPC.dT, Va=VPC.InitialSpeed, drydenParameters=VPC.DrydenNoWind):
        self.dT = dT
        self.Va = Va
        self.drydenParameters = drydenParameters

        self.Wind = States.windState()

        self.x_u = [[0]] # 1x1
        self.x_v = [[0], [0]] # 2x1
        self.x_w = [[0], [0]] # 2x1

        self.Phi_u = [[1]] # 1x1
        self.Gamma_u = [[0]] # 1x1
        self.H_u = [[1]] # 1x1

        self.Phi_v = [[1, 0], [0, 1]] # 2x2
        self.Gamma_v = [[0], [0]] # 2x1
        self.H_v = [[1, 1]] # 1x2

        self.Phi_w = [[1, 0], [0, 1]] # 2x2
        self.Gamma_w = [[0], [0]] # 2x1
        self.H_w = [[1, 1]] # 1x2

        self.CreateDrydenTransferFns(dT, Va, drydenParameters)

        return

    # Function to create Dryden transfer functions in order to update wind
    def CreateDrydenTransferFns(self, dT, Va, drydenParameters):
        # Special case if drydenParameters is all 0s
        if (drydenParameters == VPC.DrydenNoWind):
            self.Phi_u = [[1]] # 1x1
            self.Gamma_u = [[0]] # 1x1
            self.H_u = [[1]] # 1x1

            self.Phi_v = [[1, 0], [0, 1]] # 2x2
            self.Gamma_v = [[0], [0]] # 2x1
            self.H_v = [[1, 1]] # 1x2

            self.Phi_w = [[1, 0], [0, 1]] # 2x2
            self.Gamma_w = [[0], [0]] # 2x1
            self.H_w = [[1, 1]] # 1x2

            return

        # Special case if Va is close to 0
        if (math.isclose(Va, 0)):
            raise ArithmeticError("Va is close to 0")

        Lu = drydenParameters.Lu
        Lv = drydenParameters.Lv
        Lw = drydenParameters.Lw

        sigmau = drydenParameters.sigmau
        sigmav = drydenParameters.sigmav
        sigmaw = drydenParameters.sigmaw

        # Equations from Dryden Wind Model Cheat Sheet
        # u
        # Phi
        self.Phi_u[0][0] = math.exp(-(Va / Lu) * dT)
        # Gamma
        self.Gamma_u[0][0] = (Lu / Va) * (1 - (math.exp(-(Va / Lu) * dT)))
        # H
        self.H_u[0][0] = sigmau * math.sqrt((2 * Va) / (math.pi * Lu))

        # v
        # Phi
        self.Phi_v[0][0] = 1 - ((Va / Lv) * dT)
        self.Phi_v[0][1] = -((Va / Lv) ** 2) * dT
        self.Phi_v[1][0] = dT
        self.Phi_v[1][1] = 1 + ((Va / Lv) * dT)

        self.Phi_v = MatrixMath.matrixScalarMultiply(math.exp(-(Va / Lv) * dT), self.Phi_v)

        # Gamma
        self.Gamma_v[0][0] = dT
        self.Gamma_v[1][0] = (((Lv / Va) ** 2) * (math.exp((Va / Lv) * dT) - 1)) - ((Lv / Va) * dT)

        self.Gamma_v = MatrixMath.matrixScalarMultiply(math.exp(-(Va / Lv) * dT), self.Gamma_v)

        # H
        self.H_v[0][0] = 1
        self.H_v[0][1] = Va / (math.sqrt(3) * Lv)

        self.H_v = MatrixMath.matrixScalarMultiply(sigmav * math.sqrt((3 * Va) / (math.pi * Lv)), self.H_v)

        # w
        # Phi
        self.Phi_w[0][0] = 1 - ((Va / Lw) * dT)
        self.Phi_w[0][1] = -((Va / Lw) ** 2) * dT
        self.Phi_w[1][0] = dT
        self.Phi_w[1][1] = 1 + ((Va / Lw) * dT)

        self.Phi_w = MatrixMath.matrixScalarMultiply(math.exp(-(Va / Lw) * dT), self.Phi_w)

        # Gamma
        self.Gamma_w[0][0] = dT
        self.Gamma_w[1][0] = (((Lw / Va) ** 2) * (math.exp((Va / Lw) * dT) - 1)) - ((Lw / Va) * dT)

        self.Gamma_w = MatrixMath.matrixScalarMultiply(math.exp(-(Va / Lw) * dT), self.Gamma_w)

        # H
        self.H_w[0][0] = 1
        self.H_w[0][1] = Va / (math.sqrt(3) * Lw)

        self.H_w = MatrixMath.matrixScalarMultiply(sigmaw * math.sqrt((3 * Va) / (math.pi * Lw)), self.H_w)

        return

    # Function to update wind using Dryden transfer functions
    def Update(self, uu=None, uv=None, uw=None):
        if (uu == None):
            uu = random.gauss(0, 1)
        if (uv == None):
            uv = random.gauss(0, 1)
        if (uw == None):
            uw = random.gauss(0, 1)

        # Equations from Dryden Cheat Sheet
        # Step 2
        # u
        mat1_u = MatrixMath.matrixMultiply(self.Phi_u, self.x_u)
        mat2_u = MatrixMath.matrixScalarMultiply(uu, self.Gamma_u)

        self.x_u = MatrixMath.matrixAdd(mat1_u, mat2_u)

        # v
        mat1_v = MatrixMath.matrixMultiply(self.Phi_v, self.x_v)
        mat2_v = MatrixMath.matrixScalarMultiply(uv, self.Gamma_v)

        self.x_v = MatrixMath.matrixAdd(mat1_v, mat2_v)

        # w
        mat1_w = MatrixMath.matrixMultiply(self.Phi_w, self.x_w)
        mat2_w = MatrixMath.matrixScalarMultiply(uw, self.Gamma_w)

        self.x_w = MatrixMath.matrixAdd(mat1_w, mat2_w)

        # Step 3
        self.Wind.Wu = MatrixMath.matrixMultiply(self.H_u, self.x_u)[0][0]
        self.Wind.Wv = MatrixMath.matrixMultiply(self.H_v, self.x_v)[0][0]
        self.Wind.Ww = MatrixMath.matrixMultiply(self.H_w, self.x_w)[0][0]

        return

    # Function to get Dryden transfer functions
    def getDrydenTransferFns(self):
        return self.Phi_u, self.Gamma_u, self.H_u, self.Phi_v, self.Gamma_v, self.H_v, self.Phi_w, self.Gamma_w, self.H_w

    # Function to get wind state
    def getWind(self):
        return self.Wind

    # Function to get reset wind model
    def reset(self):
        self.dT = VPC.dT
        self.Va = VPC.InitialSpeed
        self.drydenParameters = VPC.DrydenNoWind

        self.Wind = States.windState()

        self.x_u = [[0]] # 1x1
        self.x_v = [[0], [0]] # 2x1
        self.x_w = [[0], [0]] # 2x1

        return

    # Function to set wind state
    def setWind(self, windState):
        self.Wind = windState

        return
