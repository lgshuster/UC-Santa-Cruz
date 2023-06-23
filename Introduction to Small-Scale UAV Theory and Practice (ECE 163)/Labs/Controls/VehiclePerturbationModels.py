"""
Name: Leonid Shuster
Lab: 3
Role: Functions to create the linearized perturbation models from the stability derivatives and non-linear model
NOTE: Functions were conceptually talked about with Jimmy Chen, Xingyu Zhou, and Christian Sabile
"""

import math
import pickle
from ece163.Modeling import VehicleAerodynamicsModel as VAM
from ece163.Constants import VehiclePhysicalConstants as VPC
from ece163.Containers import States
from ece163.Containers import Inputs
from ece163.Containers import Linearized
from ece163.Utilities import MatrixMath
from ece163.Utilities import Rotations

# Function to fill transfer function parameters
def CreateTransferFunction(trimState, trimInputs):
    tf = Linearized.transferFunctions()

    tf.Va_trim = trimState.Va
    tf.alpha_trim = trimState.alpha

    # Special case if Va is close to 0
    if (math.isclose(trimState.Va, 0)):
        tf.beta_trim = math.copysign(math.pi / 2, trimState.u)
    else:
        tf.beta_trim = math.asin(trimState.v / trimState.Va)

    tf.gamma_trim = trimState.pitch - trimState.alpha
    tf.theta_trim = trimState.pitch
    tf.phi_trim = trimState.roll

    Vastar = trimState.Va
    alphastar = trimState.alpha
    thetastar = trimState.pitch

    deltaEstar = trimInputs.Elevator
    deltaTstar = trimInputs.Throttle

    # Equations from textbook
    # 5.23
    tf.a_phi1 = -1/2 * VPC.rho * (Vastar ** 2) * VPC.S * VPC.b * VPC.Cpp * (VPC.b / (2 * Vastar))
    # 5.24
    tf.a_phi2 = 1/2 * VPC.rho * (Vastar ** 2) * VPC.S * VPC.b * VPC.CpdeltaA

    # Page 71
    tf.a_beta1 = -(VPC.rho * Vastar * VPC.S) / (2 * VPC.mass) * VPC.CYbeta
    tf.a_beta2 = (VPC.rho * Vastar * VPC.S) / (2 * VPC.mass) * VPC.CYdeltaR

    # Page 73
    tf.a_theta1 = -(VPC.rho * (Vastar ** 2) * VPC.c * VPC.S) / (2 * VPC.Jyy) * VPC.CMq * (VPC.c / (2 * Vastar))
    tf.a_theta2 = -(VPC.rho * (Vastar ** 2) * VPC.c * VPC.S) / (2 * VPC.Jyy) * VPC.CMalpha
    tf.a_theta3 = (VPC.rho * (Vastar ** 2) * VPC.c * VPC.S) / (2 * VPC.Jyy) * VPC.CMdeltaE

    # Equations from UAV Supplemental page 26
    tf.a_V1 = ((VPC.rho * Vastar * VPC.S) / (VPC.mass)) * (VPC.CD0 + (VPC.CDalpha * alphastar) + (VPC.CDdeltaE * deltaEstar)) - ((1 / VPC.mass) * dThrust_dVa(Vastar, deltaTstar))
    tf.a_V2 = (1 / VPC.mass) * dThrust_dThrottle(Vastar, deltaTstar)
    tf.a_V3 = VPC.g0 * math.cos(thetastar - alphastar)

    return tf

# Function to calculate change in thrust when throttle is changed
def dThrust_dThrottle(Va, Throttle, epsilon=0.01):
    vehicAeroModel = VAM.VehicleAerodynamicsModel()

    # Calculate force with and without change in throttle and then find change
    Fx1, Mx1 = vehicAeroModel.CalculatePropForces(Va, Throttle)
    Fx2, Mx2 = vehicAeroModel.CalculatePropForces(Va, Throttle + epsilon)

    dTdDeltaT = (Fx2 - Fx1) / epsilon

    return dTdDeltaT

# Function to calculate change in thrust when Va (airspeed) is changed
def dThrust_dVa(Va, Throttle, epsilon=0.5):
    vehicAeroModel = VAM.VehicleAerodynamicsModel()

    # Calculate force with and without change in Va (airspeed) and then find change
    Fx1, Mx1 = vehicAeroModel.CalculatePropForces(Va, Throttle)
    Fx2, Mx2 = vehicAeroModel.CalculatePropForces(Va + epsilon, Throttle)

    dTdVa = (Fx2 - Fx1) / epsilon

    return dTdVa
