# IoT-Application

This repository contains an Arduino code to run DHT11 and FC28 sensors and then connect to a REST API. 

The Arduino code will sent (POST) three kinds of data such as soil **temperature**, **moisture**, and **condition** to the REST API. The condition of the soil comes in a string format based on the temperature and moisture value. There will be three condition such as **Good**, **Average**, and **Bad**. 

Reference to the code can be seen in [this video](https://www.youtube.com/watch?v=0FZ7BOXXF1k).

Reference to the value used for the value used in determining the soil condition can be seen [here](https://docs.google.com/document/d/1p9a5d5ROAJR27xsEI_-cppt6xU05UKVb/edit?usp=sharing&ouid=108276866598815225524&rtpof=true&sd=true).
