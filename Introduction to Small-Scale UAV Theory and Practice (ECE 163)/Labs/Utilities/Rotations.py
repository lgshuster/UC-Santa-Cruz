"""
Name: Leonid Shuster
Lab: 0
Role: Various functions to extract euler angles from dcm, create dcm from euler angles, or change coordinates from NED to ENU
NOTE: Functions were conceptually talked about with Jimmy Chen
"""

import math
from . import MatrixMath

# Extracts euler angles from DCM
def dcm2Euler(DCM):
    yaw = math.atan2(DCM[0][1], DCM[0][0])

    if (DCM[0][2] > 1): # If element is greater than 1, bound it to 1
        pitch = -math.asin(1)
    elif (DCM[0][2] < -1): # Else if element is less than 1, bound it to -1
        pitch = -math.asin(-1)
    else:
        pitch = -math.asin(DCM[0][2])

    roll = math.atan2(DCM[1][2], DCM[2][2])

    eulerAngles = [yaw, pitch, roll]

    return eulerAngles

# Creates DCM from euler angles
def euler2DCM(yaw, pitch, roll):
    # Get rotation matrices along every axis
    rYaw = [[math.cos(yaw), math.sin(yaw), 0], [-math.sin(yaw), math.cos(yaw), 0], [0, 0, 1]]
    rPitch = [[math.cos(pitch), 0, -math.sin(pitch)], [0, 1, 0], [math.sin(pitch), 0, math.cos(pitch)]]
    rRoll = [[1, 0, 0], [0, math.cos(roll), math.sin(roll)], [0, -math.sin(roll), math.cos(roll)]]

    dcm = MatrixMath.matrixMultiply(rRoll, MatrixMath.matrixMultiply(rPitch, rYaw)) # Multiply all rotation matrices to get DCM; rRoll * rPitch * rYaw

    return dcm

# Convert NED coordinates to ENU
def ned2enu(points):
    """
    Function changes coordinates from North-East-Down (NED) to East-North-Up (ENU). 
    This is required because while all of the dynamics and rotations are defined in NED, 
    the graphics functions use ENU

    :param points: matrix of point in NED [n x 3]
    :return: same set of [n x 3] points in ENU coordinates
    """

    rNedToEnu = [[0, 1, 0], [1, 0, 0], [0, 0, -1]]
    nedTranspose = MatrixMath.matrixTranspose(points) # Transpose points matrix from n x 3 to 3 x n to match rotation matrix 3 x 3

    enu = MatrixMath.matrixMultiply(rNedToEnu, nedTranspose) # Multiply matrices to change to ENU
    enuTranspose = MatrixMath.matrixTranspose(enu) #Tranpose ENU back to n x 3

    return enuTranspose
