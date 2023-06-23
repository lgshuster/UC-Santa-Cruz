"""
Name: Leonid Shuster
Lab: 2/3/5 (updated CalculateAirspeed() and Update() in Lab 3 and getVehicleDerivative() and getVehicleDynamicsModel() in Lab 5)
Role: Class to compute vehicle aerodynamics
NOTE: Functions were conceptually talked about with Jimmy Chen, Xingyu Zhou, and Christian Sabile
"""

import math
from ..Containers import States
from ..Containers import Inputs
from ..Modeling import VehicleDynamicsModel as VDM
from ..Modeling import WindModel
from ..Utilities import MatrixMath
from ..Constants import VehiclePhysicalConstants as VPC

class VehicleAerodynamicsModel():
    def __init__(self, initialSpeed=VPC.InitialSpeed, initialHeight=VPC.InitialDownPosition):
        self.vehicleDynamics = VDM.VehicleDynamicsModel()
        self.vehicleDynamics.state.pn = VPC.InitialNorthPosition
        self.vehicleDynamics.state.pe = VPC.InitialEastPosition
        self.vehicleDynamics.state.pd = initialHeight
        self.vehicleDynamics.state.u = initialSpeed
        self.windModel = WindModel.WindModel()
        self.initialSpeed = initialSpeed
        self.initialHeight = initialHeight

        return

    # Function to calculate new Va, alpha, and beta
    def CalculateAirspeed(self, state, wind):
        # Equations from Dryden Wind Model Cheat Sheet needed to calculate wind + gusts
        Wn = wind.Wn
        We = wind.We
        Wd = wind.Wd

        # Calculate azimuth and elevation to rotate gusts
        if (Wn == 0 and We == 0 and Wd == 0):
            azimuthW = 0
            elevationW = 0
        else:
            azimuthW = math.atan2(We, Wn)
            elevationW = -math.asin(Wd / (math.hypot(Wn, We, Wd)))

        # Azimuth rotation matrix
        RAzimuthWRow1 = [math.cos(azimuthW), math.sin(azimuthW), 0]
        RAzimuthWRow2 = [-math.sin(azimuthW), math.cos(azimuthW), 0]
        RAzimuthWRow3 = [0, 0, 1]

        RAzimuthW = [RAzimuthWRow1, RAzimuthWRow2, RAzimuthWRow3]

        # Elevation rotation matrix
        RElevationWRow1 = [math.cos(elevationW), 0, -math.sin(elevationW)]
        RElevationWRow2 = [0, 1, 0]
        RElevationWRow3 = [math.sin(elevationW), 0, math.cos(elevationW)]

        RElevationW = [RElevationWRow1, RElevationWRow2, RElevationWRow3]

        # Azimuth-Elevation rotation matrix
        RAzimuthWElevationW = MatrixMath.matrixMultiply(RElevationW, RAzimuthW)

        Wu = wind.Wu
        Wv = wind.Wv
        Ww = wind.Ww

        Wuvw = [[Wu], [Wv], [Ww]]

        # Rotate Wu, Wv, Ww into inertial frame using azimuth-elevation rotation matrix
        WuvwInertial = MatrixMath.matrixMultiply(MatrixMath.matrixTranspose(RAzimuthWElevationW), Wuvw)

        Wned = [[Wn], [We], [Wd]]
        dcm = state.R

        # Calculate total wind and then rotate back into body
        WBody = MatrixMath.matrixMultiply(dcm, MatrixMath.matrixAdd(Wned, WuvwInertial))

        u = state.u
        v = state.v
        w = state.w

        # Calculate total speed of vehicle in body frame
        uR = u - WBody[0][0]
        vR = v - WBody[1][0]
        wR = w - WBody[2][0]

        # Calculate new Va, alpha, beta
        Va = math.hypot(uR, vR, wR)
        alpha = math.atan2(wR, uR)

        # Special case if Va is close to 0
        if (math.isclose(Va, 0)):
            beta = 0
        else:
            beta = math.asin(vR / Va)

        return Va, alpha, beta

    # Function to calculate coefficients of lift, drag, and moment as a function of alpha
    def CalculateCoeff_alpha(self, alpha):
        # Equations from lecture to get blending function
        sigmaNumer = 1 + math.exp(-VPC.M * (alpha - VPC.alpha0)) + math.exp(VPC.M * (alpha + VPC.alpha0))
        sigmaDenom = (1 + math.exp(-VPC.M * (alpha - VPC.alpha0))) * (1 + math.exp(VPC.M * (alpha + VPC.alpha0)))
        sigma = sigmaNumer / sigmaDenom

        # Equations from lecture to get coefficients of lift, drag, and moment as a function of alpha
        cL_alpha = ((1 - sigma) * (VPC.CL0 + (VPC.CLalpha * alpha))) + (sigma * 2 * math.sin(alpha) * math.cos(alpha))
        cD_alpha = ((1 - sigma) * (VPC.CDp + (((cL_alpha * alpha) ** 2) / (math.pi * VPC.AR * VPC.e)))) + (sigma * 2 * (math.sin(alpha) ** 2))
        cM_alpha = VPC.CM0 + (VPC.CMalpha * alpha)

        return cL_alpha, cD_alpha, cM_alpha

    # Function to calculate propeller forces
    def CalculatePropForces(self, Va, Throttle):
        # Equations from Propeller Cheat Sheet needed for a, b, c
        KT = 60 / (2 * math.pi * VPC.KV)
        KE = KT
        Vin = VPC.V_max * Throttle

        # Equations from Propeller Cheat Sheet needed for omega
        a = (VPC.rho * (VPC.D_prop ** 5) * VPC.C_Q0) / (4 * (math.pi ** 2))
        b = ((VPC.rho * (VPC.D_prop ** 4) * Va * VPC.C_Q1) / (2 * math.pi)) + ((KT * KE) / VPC.R_motor)
        c = (VPC.rho * (VPC.D_prop ** 3) * (Va ** 2) * VPC.C_Q2) - (KT * (Vin / VPC.R_motor)) + (KT * VPC.i0)

        # Special case if square root of negative number
        try:
            omega = (-b + math.sqrt((b ** 2) - (4 * a * c))) / (2 * a)
        except:
            omega = 100

        # Equation from Propeller Cheat Sheet needed for CT and CQ
        J = (2 * math.pi * Va) / (omega * VPC.D_prop)

        # Equations 3 and 4 from Propeller Cheat Sheet for Fx and Mx
        CT = VPC.C_T0 + (VPC.C_T1 * J) + (VPC.C_T2 * (J ** 2))
        CQ = VPC.C_Q0 + (VPC.C_Q1 * J) + (VPC.C_Q2 * (J ** 2))

        # Equations 1 and 2 from Propeller Cheat Sheet to get Fx and Mx
        Fx_prop = (VPC.rho * (omega ** 2) * (VPC.D_prop ** 4) * (CT)) / (4 * (math.pi ** 2))
        Mx_prop = -(VPC.rho * (omega ** 2) * (VPC.D_prop ** 5) * (CQ)) / (4 * (math.pi ** 2))

        return Fx_prop, Mx_prop

    # Function to gather total forces and update internal VehicleDynamicsModel state
    def Update(self, controls):
        state = self.vehicleDynamics.state

        self.windModel.Update()
        wind = self.windModel.Wind

        # Call updateForces() to get total forces
        forcesMomentsTotal = self.updateForces(state, wind, controls)

        # Call Update from VehicleDynamicsModel class with total forces to update state
        self.vehicleDynamics.Update(forcesMomentsTotal)

        return

    # Function to calculate aerodynamic forces
    def aeroForces(self, state):
        Va = state.Va
        alpha = state.alpha
        beta = state.beta

        # Special case if Va is 0, return forcesMoments class with all 0s
        if (Va == 0):
            return Inputs.forcesMoments()

        cL_alpha, cD_alpha, cM_alpha = self.CalculateCoeff_alpha(alpha)

        # Equation 4.19 from textbook
        cx_alpha = (-cD_alpha * math.cos(alpha)) + (cL_alpha * math.sin(alpha))
        cxq_alpha = (-VPC.CDq * math.cos(alpha)) + (VPC.CLq * math.sin(alpha))
        cz_alpha = (-cD_alpha * math.sin(alpha)) - (cL_alpha * math.cos(alpha))
        czq_alpha = (-VPC.CDq * math.sin(alpha)) - (VPC.CLq * math.cos(alpha))

        # Equation 4.18 from textbook, only aero forces
        forcesMomentsAero = Inputs.forcesMoments()

        # Lift and drag forces
        forceLiftDragRow1 = cx_alpha + (cxq_alpha * ((VPC.c / (2 * Va)) * state.q))
        forceLiftDragRow2 = VPC.CY0 + (VPC.CYbeta * beta) + (VPC.CYp * ((VPC.b / (2 * Va)) * state.p)) + (VPC.CYr * ((VPC.b / (2 * Va)) * state.r))
        forceLiftDragRow3 = cz_alpha + (czq_alpha * ((VPC.c / (2 * Va)) * state.q))

        forceLiftDrag = [[forceLiftDragRow1], [forceLiftDragRow2], [forceLiftDragRow3]]

        Q = (1/2 * VPC.rho * (Va ** 2) * VPC.S)

        forceLiftDrag = MatrixMath.matrixScalarMultiply(Q, forceLiftDrag)

        forcesMomentsAero.Fx = forceLiftDrag[0][0]
        forcesMomentsAero.Fy = forceLiftDrag[1][0]
        forcesMomentsAero.Fz = forceLiftDrag[2][0]

        # Equation 4.20 from textbook, only aero forces
        momentLiftDragRow1 = VPC.b * (VPC.Cl0 + (VPC.Clbeta * beta) + (VPC.Clp * ((VPC.b / (2 * Va)) * state.p)) + (VPC.Clr * ((VPC.b / (2 * Va)) * state.r)))
        momentLiftDragRow2 = VPC.c * (VPC.CM0 + (VPC.CMalpha * alpha) + (VPC.CMq * ((VPC.c / (2 * Va)) * state.q)))
        momentLiftDragRow3 = VPC.b * (VPC.Cn0 + (VPC.Cnbeta * beta) + (VPC.Cnp * ((VPC.b / (2 * Va)) * state.p)) + (VPC.Cnr * ((VPC.b / (2 * Va)) * state.r)))

        momentLiftDrag = [[momentLiftDragRow1], [momentLiftDragRow2], [momentLiftDragRow3]]

        momentLiftDrag = MatrixMath.matrixScalarMultiply(Q, momentLiftDrag)

        forcesMomentsAero.Mx = momentLiftDrag[0][0]
        forcesMomentsAero.My = momentLiftDrag[1][0]
        forcesMomentsAero.Mz = momentLiftDrag[2][0]

        return forcesMomentsAero

    # Function to calculate control forces including propeller forces
    def controlForces(self, state, controls):
        Va = state.Va
        alpha = state.alpha
        beta = state.beta

        deltaT = controls.Throttle
        deltaA = controls.Aileron
        deltaE = controls.Elevator
        deltaR = controls.Rudder

        forcesMomentsControl = Inputs.forcesMoments()

        # Equation 4.19 from textbook
        cxdeltae_alpha = (-VPC.CDdeltaE * math.cos(alpha)) + (VPC.CLdeltaE * math.sin(alpha))
        czdeltae_alpha = (-VPC.CDdeltaE * math.sin(alpha)) - (VPC.CLdeltaE * math.cos(alpha))

        # Equation 4.18 from textbook, only control forces
        # Lift and drag forces
        forceLiftDragRow1 = (cxdeltae_alpha * deltaE)
        forceLiftDragRow2 = (VPC.CYdeltaA * deltaA) + (VPC.CYdeltaR * deltaR)
        forceLiftDragRow3 = (czdeltae_alpha * deltaE)

        forceLiftDrag = [[forceLiftDragRow1], [forceLiftDragRow2], [forceLiftDragRow3]]

        Q = (1/2 * VPC.rho * (Va ** 2) * VPC.S)

        forceLiftDrag = MatrixMath.matrixScalarMultiply(Q, forceLiftDrag)

        forcesMomentsControl.Fx = forceLiftDrag[0][0]
        forcesMomentsControl.Fy = forceLiftDrag[1][0]
        forcesMomentsControl.Fz = forceLiftDrag[2][0]

        # Equation 4.20 from textbook, only control forces
        momentLiftDragRow1 = VPC.b * ((VPC.CldeltaA * deltaA) + (VPC.CldeltaR * deltaR))
        momentLiftDragRow2 = VPC.c * (VPC.CMdeltaE * deltaE)
        momentLiftDragRow3 = VPC.b * ((VPC.CndeltaA * deltaA) + (VPC.CndeltaR * deltaR))

        momentLiftDrag = [[momentLiftDragRow1], [momentLiftDragRow2], [momentLiftDragRow3]]

        momentLiftDrag = MatrixMath.matrixScalarMultiply(Q, momentLiftDrag)

        forcesMomentsControl.Mx = momentLiftDrag[0][0]
        forcesMomentsControl.My = momentLiftDrag[1][0]
        forcesMomentsControl.Mz = momentLiftDrag[2][0]

        # Getting propeller forces
        Fx_prop, Mx_prop = self.CalculatePropForces(Va, deltaT)

        # Add control forces with propeller forces
        forcesMomentsControl.Fx = forcesMomentsControl.Fx + Fx_prop
        forcesMomentsControl.Mx = forcesMomentsControl.Mx + Mx_prop

        return forcesMomentsControl

    # Function to return internal VehicleDynamicsModel derivative
    def getVehicleDerivative(self):
        return self.vehicleDynamics.dot

    # Function to return internal VehicleDynamicsModel
    def getVehicleDynamicsModel(self):
        return self.vehicleDynamics

    # Function to return internal VehicleDynamicsModel state
    def getVehicleState(self):
        return self.vehicleDynamics.state

    # Function to return internal WindModel state
    def getWindState(self):
        return self.windModel.Wind

    # Function to get gravity forces
    def gravityForces(self, state):
        forcesMomentsGravity = Inputs.forcesMoments()

        # Create 3x1 vector of force of gravity in inertial frame
        mg = [[0], [0], [VPC.mass * VPC.g0]]

        # Convert force of gravity into body frame
        forceGravity = MatrixMath.matrixMultiply(state.R, mg)

        forcesMomentsGravity.Fx = forceGravity[0][0]
        forcesMomentsGravity.Fy = forceGravity[1][0]
        forcesMomentsGravity.Fz = forceGravity[2][0]

        forcesMomentsGravity.Mx = 0
        forcesMomentsGravity.My = 0
        forcesMomentsGravity.Mz = 0

        return forcesMomentsGravity

    # Function to reset VehicleAerodynamicsModel object
    def reset(self):
        self.vehicleDynamics = VDM.VehicleDynamicsModel()
        self.vehicleDynamics.state.pn = VPC.InitialNorthPosition
        self.vehicleDynamics.state.pe = VPC.InitialEastPosition
        self.vehicleDynamics.state.pd = VPC.InitialDownPosition
        self.vehicleDynamics.state.u = VPC.InitialSpeed
        self.windModel = WindModel.WindModel()
        self.initialSpeed = VPC.InitialSpeed
        self.initialHeight = VPC.InitialDownPosition

        return

    def setVehicleDerivative(self, dot):
        self.vehicleDynamics.dot = dot

        return

    # Function to set internal VehicleDynamicsModel state
    def setVehicleState(self, state):
        self.vehicleDynamics.state = state

        return

    # Function to set internal WindModel state
    def setWindModel(self, Wn=0.0, We=0.0, Wd=0.0, drydenParameters=Inputs.drydenParameters()):
        self.windModel.Wind.Wn = Wn
        self.windModel.Wind.We = We
        self.windModel.Wind.Wd = Wd
        self.windModel.drydenParameters = drydenParameters

        return

    # Function to gather gravity, aerodynamic, and control forces and return total
    def updateForces(self, state, wind, controls):
        # Calculate new Va, alpha, and beta and update internal state
        Va, alpha, beta = self.CalculateAirspeed(state, wind)

        state.Va = Va
        state.alpha = alpha
        state.beta = beta

        # Gather new forces
        forcesMomentsGravity = self.gravityForces(state)
        forcesMomentsAero = self.aeroForces(state)
        forcesMomentsControl = self.controlForces(state, controls)

        forcesMomentsTotal = Inputs.forcesMoments()

        # Add total forces
        forcesMomentsTotal.Fx = forcesMomentsGravity.Fx + forcesMomentsAero.Fx + forcesMomentsControl.Fx
        forcesMomentsTotal.Fy = forcesMomentsGravity.Fy + forcesMomentsAero.Fy + forcesMomentsControl.Fy
        forcesMomentsTotal.Fz = forcesMomentsGravity.Fz + forcesMomentsAero.Fz + forcesMomentsControl.Fz

        forcesMomentsTotal.Mx = forcesMomentsAero.Mx + forcesMomentsControl.Mx
        forcesMomentsTotal.My = forcesMomentsAero.My + forcesMomentsControl.My
        forcesMomentsTotal.Mz = forcesMomentsAero.Mz + forcesMomentsControl.Mz

        return forcesMomentsTotal
