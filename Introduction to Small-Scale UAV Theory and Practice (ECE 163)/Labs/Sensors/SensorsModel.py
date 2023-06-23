"""
Name: Leonid Shuster
Lab: 5
Role: Class to implement sensor outputs
NOTE: Functions were conceptually talked about with Jimmy Chen, Xingyu Zhou, and Christian Sabile
"""

import math
import random
from ece163.Modeling import VehicleAerodynamicsModel
from ece163.Utilities import MatrixMath
from ..Containers import Sensors
from ..Constants import VehiclePhysicalConstants as VPC
from ..Constants import VehicleSensorConstants as VSC
from ..Modeling import VehicleAerodynamicsModel

# Class to implement noise model by first order Gauss-Markov process
class GaussMarkov():
    def __init__(self, dT=VPC.dT, tau=1000000.0, eta=0.0):
        self.dT = dT

        self.tau = tau
        self.eta = eta

        self.v = 0

        return

    # Function to reset Gauss-Markov model
    def reset(self):
        self.v = 0

        return

    # Function to update Gauss-Markov process
    def update(self, vnoise=None):
        # If noise is not given, generate random noise
        if (vnoise == None):
            vnoise = random.gauss(0, self.eta)

        self.v = (math.exp(-self.dT / self.tau) * self.v) + vnoise

        return self.v

# Class to implement three Gauss-Markov models for X, Y, and Z axes of time-varying drift
class GaussMarkovXYZ():
    def __init__(self, dT=VPC.dT, tauX=1000000.0, etaX=0.0, tauY=1000000.0, etaY=0.0, tauZ=1000000.0, etaZ=0.0):
        self.dT = dT

        self.tauX = tauX
        self.etaX = etaX

        self.tauY = tauY
        self.etaY = etaY

        self.tauZ = tauZ
        self.etaZ = etaZ

        self.gmX = GaussMarkov(dT, tauX, etaX)
        self.gmY = GaussMarkov(dT, tauY, etaY)
        self.gmZ = GaussMarkov(dT, tauZ, etaZ)

        self.vX = 0
        self.vY = 0
        self.vZ = 0

        return

    # Function to reset all three Gauss-Markov models
    def reset(self):
        self.gmX.reset()
        self.gmY.reset()
        self.gmZ.reset()

        self.vX = 0
        self.vY = 0
        self.vZ = 0

        return

    # Function to update all three Gauss-Markov processes
    def update(self, vXnoise=None, vYnoise=None, vZnoise=None):
        # If noise for any axis is not given, generate random noise
        if (vXnoise == None):
            vXnoise = random.gauss(0, self.etaX)
        if (vYnoise == None):
            vYnoise = random.gauss(0, self.etaY)
        if (vZnoise == None):
            vZnoise = random.gauss(0, self.etaZ)

        self.vX = self.gmX.update(vXnoise)
        self.vY = self.gmY.update(vYnoise)
        self.vZ = self.gmZ.update(vZnoise)

        return self.vX, self.vY, self.vZ

# Class to contain true sensor and noisy sensor outputs
class SensorsModel():
    def __init__(self, aeroModel=VehicleAerodynamicsModel.VehicleAerodynamicsModel(), taugyro=VSC.gyro_tau, etagyro=VSC.gyro_eta, tauGPS=VSC.GPS_tau, etaGPSHorizontal=VSC.GPS_etaHorizontal, etaGPSVertical=VSC.GPS_etaVertical, gpsUpdateHz=VSC.GPS_rate):
        self.aeroModel = aeroModel
        self.dT = aeroModel.vehicleDynamics.dT

        self.taugyro = taugyro
        self.etagyro = etagyro

        self.tauGPS = tauGPS
        self.etaGPSHorizontal = etaGPSHorizontal
        self.etaGPSVertical = etaGPSVertical

        self.gpsUpdateHz = gpsUpdateHz

        self.sensorTrue = Sensors.vehicleSensors()
        self.sensorNoisy = Sensors.vehicleSensors()

        self.sensorBiases = self.initializeBiases()
        self.sensorSigmas = self.initializeSigmas()

        self.gyroGM = GaussMarkovXYZ(self.dT, taugyro, etagyro)
        self.gpsGM = GaussMarkovXYZ(1 / gpsUpdateHz, tauGPS, etaGPSHorizontal, tauGPS, etaGPSHorizontal, tauGPS, etaGPSVertical)

        self.updateTicks = 0

        self.gpsTickUpdate = 1 / (self.dT * gpsUpdateHz)

        return

    # Function to get noisy sensor outputs
    def getSensorsNoisy(self):
        return self.sensorNoisy

    # Function to get true sensor outputs
    def getSensorsTrue(self):
        return self.sensorTrue

    # Function to generate biases for each sensor
    def initializeBiases(self, gyroBias=VSC.gyro_bias, accelBias=VSC.accel_bias, magBias=VSC.mag_bias, baroBias=VSC.baro_bias, pitotBias=VSC.pitot_bias):
        sensorBiases = Sensors.vehicleSensors()

        # Generate random noise for each sensor
        sensorBiases.gyro_x = gyroBias * random.uniform(-1, 1)
        sensorBiases.gyro_y = gyroBias * random.uniform(-1, 1)
        sensorBiases.gyro_z = gyroBias * random.uniform(-1, 1)

        sensorBiases.accel_x = accelBias * random.uniform(-1, 1)
        sensorBiases.accel_y = accelBias * random.uniform(-1, 1)
        sensorBiases.accel_z = accelBias * random.uniform(-1, 1)

        sensorBiases.mag_x = magBias * random.uniform(-1, 1)
        sensorBiases.mag_y = magBias * random.uniform(-1, 1)
        sensorBiases.mag_z = magBias * random.uniform(-1, 1)

        sensorBiases.baro = baroBias * random.uniform(-1, 1)

        sensorBiases.pitot = pitotBias * random.uniform(-1, 1)

        sensorBiases.gps_n = 0
        sensorBiases.gps_e = 0
        sensorBiases.gps_alt = 0
        sensorBiases.gps_sog = 0
        sensorBiases.gps_cog = 0

        return sensorBiases

    # Function to generate white noise standard deviations
    def initializeSigmas(self, gyroSigma=VSC.gyro_sigma, accelSigma=VSC.accel_sigma, magSigma=VSC.mag_sigma, baroSigma=VSC.baro_sigma, pitotSigma=VSC.pitot_sigma, gpsSigmaHorizontal=VSC.GPS_sigmaHorizontal, gpsSigmaVertical=VSC.GPS_sigmaVertical, gpsSigmaSOG=VSC.GPS_sigmaSOG, gpsSigmaCOG=VSC.GPS_sigmaCOG):
        sensorSigmas = Sensors.vehicleSensors()

        sensorSigmas.gyro_x = gyroSigma
        sensorSigmas.gyro_y = gyroSigma
        sensorSigmas.gyro_z = gyroSigma

        sensorSigmas.accel_x = accelSigma
        sensorSigmas.accel_y = accelSigma
        sensorSigmas.accel_z = accelSigma

        sensorSigmas.mag_x = magSigma
        sensorSigmas.mag_y = magSigma
        sensorSigmas.mag_z = magSigma

        sensorSigmas.baro = baroSigma

        sensorSigmas.pitot = pitotSigma

        sensorSigmas.gps_n = gpsSigmaHorizontal
        sensorSigmas.gps_e = gpsSigmaHorizontal
        sensorSigmas.gps_alt = gpsSigmaVertical
        sensorSigmas.gps_sog = gpsSigmaSOG
        sensorSigmas.gps_cog = gpsSigmaCOG

        return sensorSigmas

    # Function to reset true sensor and noisy sensor outputs, Gauss-Markov models, and biases
    def reset(self):
        self.sensorTrue = Sensors.vehicleSensors()
        self.sensorNoisy = Sensors.vehicleSensors()

        self.sensorBiases = self.initializeBiases()
        self.sensorSigmas = self.initializeSigmas()

        self.gpsGM.reset()
        self.gyroGM.reset()

        return

    # Function to update true sensor and noisy sensor outputs
    def update(self):
        state = self.aeroModel.vehicleDynamics.state
        dot = self.aeroModel.vehicleDynamics.dot

        self.sensorTrue = self.updateSensorsTrue(self.sensorTrue, state, dot)
        self.sensorNoisy = self.updateSensorsNoisy(self.sensorTrue, self.sensorNoisy, self.sensorBiases, self.sensorSigmas)

        self.updateTicks += 1

        return

    # Function to update accelerometer sensor
    def updateAccelsTrue(self, state, dot):
        # Equations for acceleration from Page 122 of textbook
        accel_x = dot.u + (state.q * state.w) - (state.r * state.v) + (VPC.g0 * math.sin(state.pitch))
        accel_y = dot.v + (state.r * state.u) - (state.p * state.w) - (VPC.g0 * math.cos(state.pitch) * math.sin(state.roll))
        accel_z = dot.w + (state.p * state.v) - (state.q * state.u) - (VPC.g0 * math.cos(state.pitch) * math.cos(state.roll))

        return accel_x, accel_y, accel_z

    # Function to update GPS sensor
    def updateGPSTrue(self, state, dot):
        gps_n = state.pn
        gps_e = state.pe
        gps_alt = -state.pd
        gps_sog = math.hypot(state.u, state.v, state.w)
        gps_cog = math.atan2(dot.pe, dot.pn)

        return gps_n, gps_e, gps_alt, gps_sog, gps_cog

    # Function to update gyro sensor
    def updateGyrosTrue(self, state):
        return state.p, state.q, state.r

    # Function to update magnetometer sensor
    def updateMagsTrue(self, state):
        mags = MatrixMath.matrixMultiply(state.R, VSC.magfield)

        return mags[0][0], mags[1][0], mags[2][0]

    # Function to update pressure sensor
    def updatePressureSensorsTrue(self, state):
        baro = -(VPC.rho * VPC.g0 * -state.pd) + VSC.Pground
        pitot = (VPC.rho * (state.Va ** 2)) / 2

        return baro, pitot

    # Function to update noisy sensor outputs using true sensor outputs, biases, and sigmas for each sensor
    def updateSensorsNoisy(self, sensorTrue=Sensors.vehicleSensors(), sensorNoisyPrevious=Sensors.vehicleSensors(), sensorBiases=Sensors.vehicleSensors(), sensorSigmas=Sensors.vehicleSensors()):
        sensorNoisyCurrent = Sensors.vehicleSensors()

        # Update Gauss-Markov models
        gmX, gmY, gmZ = self.gyroGM.update()

        # Update noisy sensor outputs (gyro, accelerometer, magnetometer, pressure)
        sensorNoisyCurrent.gyro_x = sensorTrue.gyro_x + sensorBiases.gyro_x + gmX + random.gauss(0, sensorSigmas.gyro_x)
        sensorNoisyCurrent.gyro_y = sensorTrue.gyro_y + sensorBiases.gyro_y + gmY + random.gauss(0, sensorSigmas.gyro_y)
        sensorNoisyCurrent.gyro_z = sensorTrue.gyro_z + sensorBiases.gyro_z + gmZ + random.gauss(0, sensorSigmas.gyro_z)

        sensorNoisyCurrent.accel_x = sensorTrue.accel_x + sensorBiases.accel_x + random.gauss(0, sensorSigmas.accel_x)
        sensorNoisyCurrent.accel_y = sensorTrue.accel_y + sensorBiases.accel_y + random.gauss(0, sensorSigmas.accel_y)
        sensorNoisyCurrent.accel_z = sensorTrue.accel_z + sensorBiases.accel_z + random.gauss(0, sensorSigmas.accel_z)

        sensorNoisyCurrent.mag_x = sensorTrue.mag_x + sensorBiases.mag_x + random.gauss(0, sensorSigmas.mag_x)
        sensorNoisyCurrent.mag_y = sensorTrue.mag_y + sensorBiases.mag_y + random.gauss(0, sensorSigmas.mag_y)
        sensorNoisyCurrent.mag_z = sensorTrue.mag_z + sensorBiases.mag_z + random.gauss(0, sensorSigmas.mag_z)

        sensorNoisyCurrent.baro = sensorTrue.baro + sensorBiases.baro
        sensorNoisyCurrent.pitot = sensorTrue.pitot + sensorBiases.pitot

        # If correct number of ticks, update GPS
        if (self.updateTicks % self.gpsTickUpdate == 0):
            gpsBN, gpsBE, gpsALT = self.gpsGM.update()

            sensorNoisyCurrent.gps_n = sensorTrue.gps_n + gpsBN + random.gauss(0, sensorSigmas.gps_n)
            sensorNoisyCurrent.gps_e = sensorTrue.gps_e + gpsBE + random.gauss(0, sensorSigmas.gps_e)
            sensorNoisyCurrent.gps_alt = sensorTrue.gps_alt + gpsALT + random.gauss(0, sensorSigmas.gps_alt)
            sensorNoisyCurrent.gps_sog = sensorTrue.gps_sog + random.gauss(0, sensorSigmas.gps_sog)

            # Special case to avoid divide by 0
            if math.isclose(sensorSigmas.gps_sog, 0):
                sensorNoisyCurrent.gps_cog = sensorTrue.gps_cog + random.gauss(0, sensorSigmas.gps_cog * 100)
            else:
                sensorNoisyCurrent.gps_cog = sensorTrue.gps_cog + random.gauss(0, (sensorSigmas.gps_cog / VPC.InitialSpeed) / sensorSigmas.gps_sog)

            # Bounding gps_cog to +/- pi
            if (sensorNoisyCurrent.gps_cog > math.pi):
                sensorNoisyCurrent.gps_cog = math.pi
            elif (sensorNoisyCurrent.gps_cog < math.pi):
                sensorNoisyCurrent.gps_cog = -math.pi

        # Else if not, use previous noisy sensor outputs
        else:
            sensorNoisyCurrent.gps_n = sensorNoisyPrevious.gps_n
            sensorNoisyCurrent.gps_e = sensorNoisyPrevious.gps_e
            sensorNoisyCurrent.gps_alt = sensorNoisyPrevious.gps_alt
            sensorNoisyCurrent.gps_sog = sensorNoisyPrevious.gps_sog
            sensorNoisyCurrent.gps_cog = sensorNoisyPrevious.gps_cog

        return sensorNoisyCurrent

    # Function to update true sensor outputs from state and state derivative
    def updateSensorsTrue(self, prevTrueSensors, state, dot):
        sensorTrue = Sensors.vehicleSensors()

        # Update true sensor outputs (gyro, accelerometer, magnetometer, pressure)
        sensorTrue.gyro_x, sensorTrue.gyro_y, sensorTrue.gyro_z = self.updateGyrosTrue(state)
        sensorTrue.accel_x, sensorTrue.accel_y, sensorTrue.accel_z = self.updateAccelsTrue(state, dot)
        sensorTrue.mag_x, sensorTrue.mag_y, sensorTrue.mag_z = self.updateMagsTrue(state)
        sensorTrue.baro, sensorTrue.pitot = self.updatePressureSensorsTrue(state)

        # If correct number of ticks, update GPS
        if (self.updateTicks % self.gpsTickUpdate == 0):
            sensorTrue.gps_n, sensorTrue.gps_e, sensorTrue.gps_alt, sensorTrue.gps_sog, sensorTrue.gps_cog = self.updateGPSTrue(state, dot)

        # Else if not, use previous true sensor outputs
        else:
            sensorTrue.gps_n = prevTrueSensors.gps_n
            sensorTrue.gps_e = prevTrueSensors.gps_e
            sensorTrue.gps_alt = prevTrueSensors.gps_alt
            sensorTrue.gps_sog = prevTrueSensors.gps_sog
            sensorTrue.gps_cog = prevTrueSensors.gps_cog

        return sensorTrue
