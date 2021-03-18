# Project Title
Remote Patient Monitoring System using IoT

# Project Abstract
The Sensory data collected from the wearable devices connected with patients will be uploaded to MySQL Database. From the database, the data will be fed to a Machine Learning Models deployed on AWS EC2 Instance. Using Flask framework and the API, the level of severity will be predicted. 

And also, based on the symptoms along with sensory data the machine learning model keep predicting the treatment and precautions.

# Procedure
ESP32 connected with Pulse Rate Sensor will be collecting the Systolic, Diastolic, Pulse Rate, Age, Gender will be uploaded to MySQL Database through the madblocks cloud REST API.
