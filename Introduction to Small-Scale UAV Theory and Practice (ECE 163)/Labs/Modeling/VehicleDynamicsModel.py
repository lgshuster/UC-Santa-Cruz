"""
Name: Leonid Shuster
Lab: 1/4 (had to make sure IntegrateState() also updated Va, alpha, beta, and chi in Lab 4)
Role: Class to compute vehicle dynamics
NOTE: Functions were conceptually talked about with Jimmy Chen, Xingyu Zhou, and Christian Sabile
"""

import math
from ..Containers import States
from ..Utilities import MatrixMath
from ..Utilities import Rotations
from ..Constants import VehiclePhysicalConstants as VPC

class VehicleDynamicsModel():
    def __init__(self, dT=VPC.dT):
        self.dT = dT
        self.state = States.vehicleState()
        self.dot = States.vehicleState()

        return

    # Function to get state and state derivative and perform integration
    def ForwardEuler(self, forcesMoments):
        state = self.getVehicleState()

        self.dot = self.derivative(state, forcesMoments) # Call derivative() to get state derivative

        newState = self.IntegrateState(self.dT, state, self.dot) # Call IntegrateState() to perform integration

        return newState

    # Function to integrate state using state derivative
    def IntegrateState(self, dT, state, dot):
        newState = States.vehicleState()

        # Equations from lecture to integrate state variables
        newState.pn = state.pn + (dot.pn * dT)
        newState.pe = state.pe + (dot.pe * dT)
        newState.pd = state.pd + (dot.pd * dT)
        
        newState.u = state.u + (dot.u * dT)
        newState.v = state.v + (dot.v * dT)
        newState.w = state.w + (dot.w * dT)

        # Equation 23 from Attitude Cheat Sheet to perform integration
        rMinus = state.R

        rPlus = MatrixMath.matrixMultiply(self.Rexp(dT, state, dot), rMinus) # Call Rexp() to get Rexp needed for integration of DCM

        eulerAngles = Rotations.dcm2Euler(rPlus) # Extract new euler angles

        newState.yaw = eulerAngles[0]
        newState.pitch = eulerAngles[1]
        newState.roll = eulerAngles[2]

        newState.p = state.p + (dot.p * dT)
        newState.q = state.q + (dot.q * dT)
        newState.r = state.r + (dot.r * dT)

        newState.R = rPlus

        newState.Va = state.Va
        newState.alpha = state.alpha
        newState.beta = state.beta
        newState.chi = state.chi

        return newState

    # Function to get Rexp needed for DCM integration
    def Rexp(self, dT, state, dot):
        # Equation 35 from Attitude Cheat Sheet to get new p, q, r
        pk = state.p
        qk = state.q
        rk = state.r

        pkDot = dot.p
        qkDot = dot.q
        rkDot = dot.r

        bodyAngleRates = [[pk], [qk], [rk]]
        bodyAngleAccel = [[pkDot], [qkDot], [rkDot]]

        w = MatrixMath.matrixAdd(bodyAngleRates, MatrixMath.matrixScalarMultiply(dT/2, bodyAngleAccel))

        p = w[0][0]
        q = w[1][0]
        r = w[2][0]

        # Equation 22 from Attitude Cheat Sheet to get Rexp
        normw = math.hypot(p, q, r)

        identity = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]

        skew = MatrixMath.matrixSkew(p, q, r)

        # Checking if normw is close to 0
        if (normw < 0.1):
            num1 = dT - (((dT ** 3) * (normw ** 2)) / 6) + (((dT ** 5) * (normw ** 4)) / 120) # Equation 24 from Attitude Cheat Sheet
            num2 = ((dT ** 2) / 2) - (((dT ** 4) * (normw ** 2)) / 24) + (((dT ** 6) * (normw ** 4)) / 720) # Equation 25 from Attitude Cheat Sheet
        else:
            num1 = math.sin(normw * dT) / normw
            num2 = (1 - math.cos(normw * dT)) / (normw ** 2)

        mat1 = MatrixMath.matrixScalarMultiply(num1, skew)
        mat2 = MatrixMath.matrixScalarMultiply(num2, MatrixMath.matrixMultiply(skew, skew))

        rExp = MatrixMath.matrixAdd(MatrixMath.matrixSubtract(identity, mat1), mat2)

        return rExp

    # Function to perform integration and update state
    def Update(self, forcesMoments):
        self.state = self.ForwardEuler(forcesMoments) # Call ForwardEuler to perform derivative and integration

        self.setVehicleState(self.state) # Update internal state

        return

    # Function to get state derivative
    def derivative(self, state, forcesMoments):
        dot = States.vehicleState() # Create new state object to hold derivatives

        # Get state variables
        pn = state.pn
        pe = state.pe
        pd = state.pd

        u = state.u
        v = state.v
        w = state.w

        yaw = state.yaw
        pitch = state.pitch
        roll = state.roll

        p = state.p
        q = state.q
        r = state.r

        # Get forces
        fx = forcesMoments.Fx
        fy = forcesMoments.Fy
        fz = forcesMoments.Fz

        # Equation 3.14 from textbook to get change in pn, pe, pd
        dcm = state.R

        dcmTranspose = MatrixMath.matrixTranspose(dcm)
        bodySpeed = [[u], [v], [w]]

        pDot = MatrixMath.matrixMultiply(dcmTranspose, bodySpeed)

        dot.pn = pDot[0][0]
        dot.pe = pDot[1][0]
        dot.pd = pDot[2][0]

        # Equation 3.15 from textbook to get change in u, v, w
        mat1 = [[(r * v) - (q * w)], [(p * w) - (r * u)], [(q * u) - (p * v)]]
        f = [[fx], [fy], [fz]]

        vDot = MatrixMath.matrixAdd(mat1, MatrixMath.matrixScalarMultiply(1/VPC.mass, f))

        dot.u = vDot[0][0]
        dot.v = vDot[1][0]
        dot.w = vDot[2][0]

        # Equation 3.16 from textbook to get change in roll, pitch, yaw
        mat2 = [[1, math.sin(roll) * math.tan(pitch), math.cos(roll) * math.tan(pitch)], [0, math.cos(roll), -math.sin(roll)], [0, math.sin(roll)/math.cos(pitch), math.cos(roll)/math.cos(pitch)]]
        bodyRates = [[p], [q], [r]]

        eulerRates = MatrixMath.matrixMultiply(mat2, bodyRates)

        dot.roll = eulerRates[0][0]
        dot.pitch = eulerRates[1][0]
        dot.yaw = eulerRates[2][0]

        Jxx = VPC.Jxx
        Jyy = VPC.Jyy
        Jzz = VPC.Jzz
        Jxz = VPC.Jxz

        # Equation 3.13 from textbook to get Gammas for Equation 3.17
        Gamma = VPC.Jdet
        Gamma1 = VPC.Gamma1
        Gamma2 = VPC.Gamma2
        Gamma3 = Jzz / Gamma
        Gamma4 = Jxz / Gamma
        Gamma5 = (Jzz - Jxx) / Jyy
        Gamma6 = Jxz / Jyy
        Gamma7 = VPC.Gamma7
        Gamma8 = Jxx / Gamma

        # Equation 3.17 from textbook to get change in p, q, r
        l = forcesMoments.Mx
        m = forcesMoments.My
        n = forcesMoments.Mz

        mat3 = [[(Gamma1 * p * q) - (Gamma2 * q * r)], [(Gamma5 * p * r) - (Gamma6 * (p ** 2 - r ** 2))], [(Gamma7 * p * q) - (Gamma1 * q * r)]]
        mat4 = [[(Gamma3 * l) + (Gamma4 * n)], [(1/Jyy) * m], [(Gamma4 * l) + (Gamma8 * n)]]

        bodyRatesDot = MatrixMath.matrixAdd(mat3, mat4)

        dot.p = bodyRatesDot[0][0]
        dot.q = bodyRatesDot[1][0]
        dot.r = bodyRatesDot[2][0]

        # Equation 19 from Attitude Cheat Sheet to get change in DCM
        rDot = MatrixMath.matrixScalarMultiply(-1, MatrixMath.matrixMultiply(MatrixMath.matrixSkew(p, q, r), dcm))
        dot.R = rDot

        dot.Va = state.Va
        dot.alpha = state.alpha
        dot.beta = state.beta
        dot.chi = state.chi

        return dot

    # Function to get state
    def getVehicleState(self):
        return self.state

    # Function to reset state and state derivative
    def reset(self):
        self.state = States.vehicleState()
        self.dot = States.vehicleState()

        return

    # Function to reset state
    def resetVehicleState(self):
        self.state = States.vehicleState()

        return

    # Function to set state
    def setVehicleState(self, state):
        self.state = state

        return
