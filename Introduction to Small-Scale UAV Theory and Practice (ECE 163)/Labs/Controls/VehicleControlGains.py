"""
Name: Leonid Shuster
Lab: 4
Role: Class to compute gains from tuning parameters or tuning parameters from gains
NOTE: Functions were conceptually talked about with Jimmy Chen, Xingyu Zhou, and Christian Sabile
"""

import math
import pickle
from ece163.Modeling import VehicleAerodynamicsModel
from ece163.Constants import VehiclePhysicalConstants as VPC
from ece163.Containers import States
from ece163.Containers import Inputs
from ece163.Containers import Controls
from ece163.Containers import Linearized
from ece163.Utilities import MatrixMath
from ece163.Utilities import Rotations

# Function to compute gains from tuning parameters
def computeGains(tuningParameters=Controls.controlTuning(), linearizedModel=Linearized.transferFunctions()):
    controlGains = Controls.controlGains()

    # Tuning Parameter Variables
    Wn_roll = tuningParameters.Wn_roll
    Zeta_roll = tuningParameters.Zeta_roll
    Wn_course = tuningParameters.Wn_course
    Zeta_course = tuningParameters.Zeta_course
    Wn_sideslip = tuningParameters.Wn_sideslip
    Zeta_sideslip = tuningParameters.Zeta_sideslip
    Wn_pitch = tuningParameters.Wn_pitch
    Zeta_pitch = tuningParameters.Zeta_pitch
    Wn_altitude = tuningParameters.Wn_altitude
    Zeta_altitude = tuningParameters.Zeta_altitude
    Wn_SpeedfromThrottle = tuningParameters.Wn_SpeedfromThrottle
    Zeta_SpeedfromThrottle = tuningParameters.Zeta_SpeedfromThrottle
    Wn_SpeedfromElevator = tuningParameters.Wn_SpeedfromElevator
    Zeta_SpeedfromElevator = tuningParameters.Zeta_SpeedfromElevator

    # Trim Variables
    Va_trim = linearizedModel.Va_trim
    a_phi1 = linearizedModel.a_phi1
    a_phi2 = linearizedModel.a_phi2
    a_beta1 = linearizedModel.a_beta1
    a_beta2 = linearizedModel.a_beta2
    a_theta1 = linearizedModel.a_theta1
    a_theta2 = linearizedModel.a_theta2
    a_theta3 = linearizedModel.a_theta3
    a_V1 = linearizedModel.a_V1
    a_V2 = linearizedModel.a_V2

    # Equations from UAV Supplemental
    controlGains.kp_roll = (Wn_roll ** 2) / a_phi2 # Page 36
    controlGains.ki_roll = 0.001 # Given by TA
    controlGains.kd_roll = ((2 * Zeta_roll * Wn_roll) - a_phi1) / a_phi2 # Page 36

    # Equations from textbook
    controlGains.kp_sideslip = ((2 * Zeta_sideslip * Wn_sideslip) - a_beta1) / a_beta2 # Page 105 (6.15) solving for kp_sideslip
    controlGains.ki_sideslip = (Wn_sideslip ** 2) / a_beta2 # Page 105 (6.14) solving for ki_sideslip

    controlGains.kp_course = (2 * Zeta_course * Wn_course) * (Va_trim / VPC.g0) # Page 103 (6.12)
    controlGains.ki_course = (Wn_course ** 2) * (Va_trim / VPC.g0) # Page 103 (6.13)

    # Equations from UAV Supplemental
    controlGains.kp_pitch = ((Wn_pitch ** 2) - a_theta2) / a_theta3 # Page 44
    controlGains.kd_pitch = ((2 * Zeta_pitch * Wn_pitch) - a_theta1) / a_theta3 # Page 44

    kp_pitch = controlGains.kp_pitch

    Kpitch_DC = (kp_pitch * a_theta3) / (Wn_pitch ** 2) # Page 44

    # Equations from the textbook
    controlGains.ki_altitude = (Wn_altitude ** 2) / (Kpitch_DC * Va_trim) # Page 109 (6.24)
    controlGains.kp_altitude = (2 * Zeta_altitude * Wn_altitude) / (Kpitch_DC * Va_trim) # Page 109 (6.25)

    controlGains.kp_SpeedfromThrottle = ((2 * Zeta_SpeedfromThrottle * Wn_SpeedfromThrottle) - a_V1) / a_V2 # Page 112 (6.30)
    controlGains.ki_SpeedfromThrottle = (Wn_SpeedfromThrottle ** 2) / a_V2 # Page 112 (6.29)

    controlGains.kp_SpeedfromElevator = (a_V1 - (2 * Zeta_SpeedfromElevator * Wn_SpeedfromElevator)) / (Kpitch_DC * VPC.g0) # Page 111 (6.28)
    controlGains.ki_SpeedfromElevator = -((Wn_SpeedfromElevator ** 2) / (Kpitch_DC * VPC.g0)) # Page 111 (6.27)

    return controlGains

# Function to compute tuning parameters from gains
def computeTuningParameters(controlGains=Controls.controlGains(), linearizedModel=Linearized.transferFunctions()):
    controlTuning = Controls.controlTuning()

    # Control Gain Variables
    kp_roll = controlGains.kp_roll
    kd_roll = controlGains.kd_roll
    kp_sideslip = controlGains.kp_sideslip
    ki_sideslip = controlGains.ki_sideslip
    kp_course = controlGains.kp_course
    ki_course = controlGains.ki_course
    kp_pitch = controlGains.kp_pitch
    kd_pitch = controlGains.kd_pitch
    kp_altitude = controlGains.kp_altitude
    ki_altitude = controlGains.ki_altitude
    kp_SpeedfromThrottle = controlGains.kp_SpeedfromThrottle
    ki_SpeedfromThrottle = controlGains.ki_SpeedfromThrottle
    kp_SpeedfromElevator = controlGains.kp_SpeedfromElevator
    ki_SpeedfromElevator = controlGains.ki_SpeedfromElevator

    # Trim Variables
    Va_trim = linearizedModel.Va_trim
    a_phi1 = linearizedModel.a_phi1
    a_phi2 = linearizedModel.a_phi2
    a_beta1 = linearizedModel.a_beta1
    a_beta2 = linearizedModel.a_beta2
    a_theta1 = linearizedModel.a_theta1
    a_theta2 = linearizedModel.a_theta2
    a_theta3 = linearizedModel.a_theta3
    a_V1 = linearizedModel.a_V1
    a_V2 = linearizedModel.a_V2

    try:
        # Equations from UAV Supplemental
        controlTuning.Wn_roll = math.sqrt(kp_roll * a_phi2) # Page 36 solving for Wn_roll
        Wn_roll = controlTuning.Wn_roll
        controlTuning.Zeta_roll = ((kd_roll * a_phi2) + a_phi1) / (2 * Wn_roll) # Page 36 solving for Zeta_roll

        # Equations from textbook
        controlTuning.Wn_course = math.sqrt(ki_course / (Va_trim / VPC.g0)) # Page 103 (6.13) solving for Wn_course
        Wn_course = controlTuning.Wn_course
        controlTuning.Zeta_course = (kp_course / (Va_trim / VPC.g0)) / (2 * Wn_course) # Page 103 (6.12) solving for Zeta_course

        controlTuning.Wn_sideslip = math.sqrt(a_beta2 * ki_sideslip) # Page 105 (6.14) solving for Wn_sideslip
        Wn_sideslip = controlTuning.Wn_sideslip
        controlTuning.Zeta_sideslip = (a_beta1 + (a_beta2 * kp_sideslip)) / (2 * Wn_sideslip) # Page 105 (6.15) solving for kp_sideslip

        # Equations from UAV Supplemental
        controlTuning.Wn_pitch = math.sqrt((kp_pitch * a_theta3) + a_theta2) # Page 44 solving for Wn_pitch
        Wn_pitch = controlTuning.Wn_pitch
        controlTuning.Zeta_pitch = ((kd_pitch * a_theta3) + a_theta1) / (2 * Wn_pitch) # Page 44 solving for Zeta_pitch

        Kpitch_DC = (kp_pitch * a_theta3) / (Wn_pitch ** 2) # Page 44

        # Equations from the textbook
        controlTuning.Wn_altitude = math.sqrt(Kpitch_DC * Va_trim * ki_altitude) # Page 109 (6.24) solving for Wn_altitude
        Wn_altitude = controlTuning.Wn_altitude
        controlTuning.Zeta_altitude = (kp_altitude * Kpitch_DC * Va_trim) / (2 * Wn_altitude) # Page 109 (6.25) solving for Zeta_altitude

        controlTuning.Wn_SpeedfromThrottle = math.sqrt(ki_SpeedfromThrottle * a_V2) # Page 112 (6.29) solving for Wn_SpeedfromThrottle
        Wn_SpeedfromThrottle = controlTuning.Wn_SpeedfromThrottle
        controlTuning.Zeta_SpeedfromThrottle = ((kp_SpeedfromThrottle * a_V2) + a_V1) / (2 * Wn_SpeedfromThrottle) # Page 112 (6.30) solving for Zeta_SpeedfromThrottle

        controlTuning.Wn_SpeedfromElevator = math.sqrt(ki_SpeedfromElevator * -Kpitch_DC * VPC.g0) # Page 111 (6.27) solving for Wn_SpeedfromElevator
        Wn_SpeedfromElevator = controlTuning.Wn_SpeedfromElevator
        controlTuning.Zeta_SpeedfromElevator = ((kp_SpeedfromElevator * Kpitch_DC * VPC.g0) - a_V1) / -(2 * Wn_SpeedfromElevator) # Page 111 (6.28) solving for Zeta_SpeedfromElevator

    except ValueError:
        return Controls.controlTuning()

    return controlTuning
