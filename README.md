# geiger
Geiger counter with Huzzah32 and OLED Feather

List of material

 * GGreg20_V3 Ionizing Radiation Detector https://www.tindie.com/products/iotdev/ggreg20_v3-ionizing-radiation-detector/
 * Arduino Huzzah32
 * Arduino OLED Feather 128x32

Measured values (CPM - counts per minute and μSv) are shown on the display and the CPM value is also logged to MQTT server and Radmon.org (both currently hardcoded).

# TODOs

 * use buttons on the display (Wifi on/off, logging on/off, switch measurement units, switch the display on/off)
 * powering on/off
 * measure and show battery level

# How to interpret the measurements
The danger when being exposed to a radiation source depends mainly on two factors. The radiation level (μSv) and the period of exposition (t). For this reason, when the authorities stablish the maximum levels use the mark radiation absorbed in an entire year. This limit is set in 50000μSv.

The thing is in how much time takes us to we receive this maximum level. In normal conditions even after one year we will be far away from this point with levels close to 3650μSv. Even if we have to receive medical scans we will get 3000μSv for a mammogram or 20μSv for a chest X-ray. Far away from the limits unless you have to be continuously under tests.

Common background radiation is around 0.081μSv/h so in 1 year we will receive 3650μSv.

Anyhow being able to measure levels above 0.1μSv/h can give us valuable information of anormal values when analysing people skin, tools or even food.

If we calculate the average (per hour) radiation that we obtain from dividing the maximum level between the number of hours in a year (50000μSv)/(24*365) = 5.70μSv/h of maximum radioactivity per hour. In Fukushima on 21th March the level was 7.47μSv/h .

Some other reference levels (just the nominal value not the exposed time!):

       10 μSv – The average radiation you received today (0.2uSv/h ~ 4,8uSv/day)
       40 μSv – The radiation you receive by taking a flight from New York to L.A.
      100 μSv – The radiation you receive during a dental x-ray
      800 μSv – Total radiation dose at Three-Mile Island for the duration of the accident
    3 000 μSv – Radiation dose from a mammogram
    3 600 μSv – Average radiation a US citizen receives in a year from all sources
   50 000 μSv – Maximum allowable yearly occupational dose (USA)
  100 000 μSv – Lowest yearly dose likely linked to increased cancer risk
2 000 000 μSv – Severe radiation poisoning (sometimes fatal)

Source: https://cooking-hacks.com/documentation/tutorials/geiger-counter-radiation-sensor-board-arduino-raspberry-pi-tutorial/index.html

# Comments

A human being receives on average 200 000μSv during the entire life. The more you receive, the bigger risk of cancer you are exposed to.
