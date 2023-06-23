"""
Name: Leonid Shuster
Lab: 4
Role: Class to implement the closed loop control using PD, PI, and PID control loops
NOTE: Functions were conceptually talked about with Jimmy Chen, Xingyu Zhou, and Christian Sabile
"""

import math
import sys
import ece163.Containers.Inputs as Inputs
import ece163.Containers.Controls as Controls
import ece163.Constants.VehiclePhysicalConstants as VPC
import ece163.Modeling.VehicleAerodynamicsModel as VehicleAerodynamicsModule

# Class to implement PD control loop
class PDControl():
    def __init__(self, kp=0.0, kd=0.0, trim=0.0, lowLimit=0.0, highLimit=0.0):
        self.kp = kp
        self.kd = kd
        self.trim = trim
        self.lowLimit = lowLimit
        self.highLimit = highLimit

        return

    # Function to calculate output of PD control loop
    def Update(self, command=0.0, current=0.0, derivative=0.0):
        # Calculate error
        error = command - current

        # Calculate output
        u = (self.kp * error) - (self.kd * derivative) + self.trim

        # If output goes out of bounds
        if (u < self.lowLimit):
            u = self.lowLimit
        elif (u > self.highLimit):
            u = self.highLimit

        return u

    # Function to set gains for PD control loop
    def setPDGains(self, kp=0.0, kd=0.0, trim=0.0, lowLimit=0.0, highLimit=0.0):
        self.kp = kp
        self.kd = kd
        self.trim = trim
        self.lowLimit = lowLimit
        self.highLimit = highLimit

        return

# Class to implement PI control loop
class PIControl():
    def __init__(self, dT=VPC.dT, kp=0.0, ki=0.0, trim=0.0, lowLimit=0.0, highLimit=0.0):
        self.dT = dT
        self.kp = kp
        self.ki = ki
        self.trim = trim
        self.lowLimit = lowLimit
        self.highLimit = highLimit
        self.accumulator = 0.0
        self.prevError = 0.0

        return

    # Function to calculate output of PI control loop
    def Update(self, command=0.0, current=0.0):
        # Calculate error
        error = command - current

        # Update accumulator
        self.accumulator += ((1/2) * self.dT * (error + self.prevError))

        # Calculate output
        u = self.trim + (self.kp * error) + (self.ki * self.accumulator)

        # If output goes out of bounds
        if (u > self.highLimit):
            u = self.highLimit
            self.accumulator -= (1/2) * self.dT * (error + self.prevError)
        elif (u < self.lowLimit):
            u = self.lowLimit
            self.accumulator -= (1/2) * self.dT * (error + self.prevError)

        # Store current error for next loop
        self.prevError = error

        return u

    # Function to reset integration state to 0
    def resetIntegrator(self):
        self.accumulator = 0.0
        self.prevError = 0.0

        return

    # Function to set gains for PD control loop
    def setPIGains(self, dT=VPC.dT, kp=0.0, ki=0.0, trim=0.0, lowLimit=0.0, highLimit=0.0):
        self.dT = dT
        self.kp = kp
        self.ki = ki
        self.trim = trim
        self.lowLimit = lowLimit
        self.highLimit = highLimit

        return

# Class to implement PID control loop
class PIDControl():
    def __init__(self, dT=VPC.dT, kp=0.0, kd=0.0, ki=0.0, trim=0.0, lowLimit=0.0, highLimit=0.0):
        self.dT = dT
        self.kp = kp
        self.kd = kd
        self.ki = ki
        self.trim = trim
        self.lowLimit = lowLimit
        self.highLimit = highLimit
        self.accumulator = 0.0
        self.prevError = 0.0

        return

    # Function to calculate output of PID control loop
    def Update(self, command=0.0, current=0.0, derivative=0.0):
        # Calculate error
        error = command - current

        # Update accumulator
        self.accumulator += (1/2) * self.dT * (error + self.prevError)

        # Calculate output
        u = self.trim + (self.kp * error) - (self.kd * derivative) + (self.ki * self.accumulator)

        # If output goes out of bounds
        if (u > self.highLimit):
            u = self.highLimit
            self.accumulator -= (1/2) * self.dT * (error + self.prevError)
        elif (u < self.lowLimit):
            u = self.lowLimit
            self.accumulator -= (1/2) * self.dT * (error + self.prevError)
        self.prevError = error

        return u

    # Function to reset integration state to 0
    def resetIntegrator(self):
        self.accumulator = 0.0
        self.prevError = 0.0

        return

    # Function to set gains for PID control loop
    def setPIDGains(self, dT=VPC.dT, kp=0.0, kd=0.0, ki=0.0, trim=0.0, lowLimit=0.0, highLimit=0.0):
        self.dT = dT
        self.kp = kp
        self.kd = kd
        self.ki = ki
        self.trim = trim
        self.lowLimit = lowLimit
        self.highLimit = highLimit

        return

# Class to put together PI, PD, and PID control loops for vehicle
class VehicleClosedLoopControl():
    def __init__(self, dT=VPC.dT):
        self.VAM = VehicleAerodynamicsModule.VehicleAerodynamicsModel()
        self.dT = self.VAM.vehicleDynamics.dT
        self.VAM.vehicleDynamics.dT = dT
        self.controlGain = Controls.controlGains()
        self.trimInputs = Inputs.controlInputs()
        self.consurOutputs = Inputs.controlInputs()
        self.aileronFromRoll = PIDControl()
        self.rollFromCourse = PIControl()
        self.rudderFromSideslip = PIControl()
        self.elevatorFromPitch = PDControl()
        self.throttleFromAirspeed = PIControl()
        self.pitchFromAltitude = PIControl()
        self.pitchFromAirspeed = PIControl()
        self.climbMode = Controls.AltitudeStates.HOLDING

        return

    # Function to implement entire closed loop by combining PI, PD, and PID modules
    def Update(self, referenceCommands=Controls.referenceCommands()):
        # Get state variables
        pd = self.VAM.vehicleDynamics.state.pd
        pitch = self.VAM.vehicleDynamics.state.pitch
        roll = self.VAM.vehicleDynamics.state.roll
        p = self.VAM.vehicleDynamics.state.p
        q = self.VAM.vehicleDynamics.state.q

        Va = self.VAM.vehicleDynamics.state.Va
        beta = self.VAM.vehicleDynamics.state.beta
        chi = self.VAM.vehicleDynamics.state.chi
        altitude = -pd

        # Get course error
        courseError = referenceCommands.commandedCourse - chi

        # If error is outside of 180 degrees
        if courseError >= math.pi:
            chi += (2 * math.pi)
        elif courseError <= -math.pi:
            chi -= (2 * math.pi)

        # Get outputs for roll, aileron, or rudder commands by calling control loops
        rollCmd = self.rollFromCourse.Update(referenceCommands.commandedCourse, chi)
        aileronCmd = self.aileronFromRoll.Update(rollCmd, roll, p)
        rudderCmd = self.rudderFromSideslip.Update(0.0, beta)

        # Longitudinal controller
        # If supposed to descend, update climbMode and reset pitch from airspeed PI loop if needed and calculate throttle and pitch commands by calling control loops
        if altitude > (referenceCommands.commandedAltitude + VPC.altitudeHoldZone):
            if self.climbMode is not Controls.AltitudeStates.DESCENDING:
                self.climbMode = Controls.AltitudeStates.DESCENDING
                self.pitchFromAirspeed.resetIntegrator()

            throttleCmd = VPC.minControls.Throttle
            pitchCmd = self.pitchFromAirspeed.Update(referenceCommands.commandedAirspeed, Va)

        # Else if supposed to ascend, update climbMode and reset pitch from airspeed PI loop if needed and calculate throttle and pitch commands by calling control loops
        elif altitude < (referenceCommands.commandedAltitude - VPC.altitudeHoldZone):
            if self.climbMode is not Controls.AltitudeStates.CLIMBING:
                self.climbMode = Controls.AltitudeStates.CLIMBING
                self.pitchFromAirspeed.resetIntegrator()

            throttleCmd = VPC.maxControls.Throttle
            pitchCmd = self.pitchFromAirspeed.Update(referenceCommands.commandedAirspeed, Va)

        # Else if supposed to hold, update climbMode and reset pitch from airspeed PI loop if needed and calculate throttle and pitch commands by calling control loops
        else:
            if self.climbMode is not Controls.AltitudeStates.HOLDING:
                self.climbMode = Controls.AltitudeStates.HOLDING
                self.pitchFromAirspeed.resetIntegrator()

            throttleCmd = self.throttleFromAirspeed.Update(referenceCommands.commandedAirspeed, Va)
            pitchCmd = self.pitchFromAltitude.Update(referenceCommands.commandedAltitude, altitude)

        # Calculate elevator command using pitch command by calling control loop
        elevatorCmd = self.elevatorFromPitch.Update(pitchCmd, pitch, q)

        # Update internal control outputs
        self.consurOutputs.Throttle = throttleCmd
        self.consurOutputs.Rudder = rudderCmd
        self.consurOutputs.Elevator = elevatorCmd
        self.consurOutputs.Aileron = aileronCmd

        # Update reference roll and pitch commands
        referenceCommands.commandedRoll = rollCmd
        referenceCommands.commandedPitch = pitchCmd

        # Update internal Vehicle Aerodynamics Model with new control outputs
        self.VAM.Update(self.consurOutputs)

        return

    # Function to get control gains
    def getControlGains(self):
        return self.controlGain

    # Function to get trim inputs
    def getTrimInputs(self):
        return self.trimInputs

    # Function to get Vehicle AeroDynamics Model
    def getVehicleAerodynamicsModel(self):
        return self.VAM

    # Function to get controt outputs
    def getVehicleControlSurfaces(self):
        return self.consurOutputs

    # Function to get Vehicle Dynamics Model state
    def getVehicleState(self):
        return self.VAM.vehicleDynamics.state

    # Function to reset VehicleClosedLoopControl class except control gains
    def reset(self):
        self.VAM.reset()
        self.aileronFromRoll.resetIntegrator()
        self.rollFromCourse.resetIntegrator()
        self.rudderFromSideslip.resetIntegrator()
        self.throttleFromAirspeed.resetIntegrator()
        self.pitchFromAltitude.resetIntegrator()
        self.pitchFromAirspeed.resetIntegrator()

        return

    # Function to set control gains for each PI, PD, PID loop
    def setControlGains(self, controlGains=Controls.controlGains()):
        self.controlGain = controlGains
        self.aileronFromRoll.setPIDGains(self.dT, self.controlGain.kp_roll, self.controlGain.kd_roll, self.controlGain.ki_roll, self.trimInputs.Aileron, VPC.minControls.Aileron, VPC.maxControls.Aileron)
        self.rollFromCourse.setPIGains(self.dT, self.controlGain.kp_course, self.controlGain.ki_course, 0.0, -math.radians(VPC.bankAngleLimit), math.radians(VPC.bankAngleLimit))
        self.rudderFromSideslip.setPIGains(self.dT, self.controlGain.kp_sideslip, self.controlGain.ki_sideslip, self.trimInputs.Rudder, VPC.minControls.Rudder, VPC.maxControls.Rudder)
        self.elevatorFromPitch.setPDGains(self.controlGain.kp_pitch, self.controlGain.kd_pitch, self.trimInputs.Elevator, VPC.minControls.Elevator, VPC.maxControls.Elevator)
        self.throttleFromAirspeed.setPIGains(self.dT, self.controlGain.kp_SpeedfromThrottle, self.controlGain.ki_SpeedfromThrottle, self.trimInputs.Throttle, VPC.minControls.Throttle, VPC.maxControls.Throttle)
        self.pitchFromAltitude.setPIGains(self.dT, self.controlGain.kp_altitude, self.controlGain.ki_altitude, 0.0, -math.radians(VPC.pitchAngleLimit), math.radians(VPC.pitchAngleLimit))
        self.pitchFromAirspeed.setPIGains(self.dT, self.controlGain.kp_SpeedfromElevator, self.controlGain.ki_SpeedfromElevator, 0.0, -math.radians(VPC.pitchAngleLimit), math.radians(VPC.pitchAngleLimit))

        return

    # Function to set trim inputs
    def setTrimInputs(self, trimInputs= Inputs.controlInputs()):
        self.trimInputs = trimInputs

        return

    # Function to set Vehicle Dynamics Model state
    def setVehicleState(self,state):
        self.VAM.vehicleDynamics.state = state

        return
