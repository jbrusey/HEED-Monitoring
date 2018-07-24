library(tidyverse)
library(ndjson)

data <- read_csv("~/Desktop/SensorData/ML_8.csv", col_names=FALSE)
names(data) <- c("Time","NodeID","SolarBatt", "Usage", "Charging", "Interrupt", "Activity", "Inactivity", "Battery", "Seq")


data <- data %>%
  mutate(
    Time = as.POSIXct(Time, origin="1970-01-01 00:00:00")
  )

sb_plt <- ggplot(data, aes(x=Time, y=SolarBatt)) +
  geom_point()+
  ylab("Solar Battery (V)") +
  xlab("Sequence Number")
sb_plt
ggsave(plot=sb_plt,"~/Desktop/SensorData/SolarBattery.pdf", width=6, height=3.5, units="in")


usage_plt <- ggplot(data, ) +
  geom_line(aes(x=Time, y=Usage, colour = "Usage")) +
  geom_line(aes(x=Time, y=Charging, colour = "Charging")) +
  ylab("Current (mA)") +
  xlab("Sequence Number")
usage_plt
ggsave(plot=usage_plt,"~/Desktop/SensorData/State.pdf", width=6, height=3.5, units="in")



cc_data <- ndjson::stream_in("~/Desktop/SensorData/StreetEnergy_BlueSolar.log") %>%
  mutate(
    date = as.POSIXct(time, origin = "1970-01-01 00:00:00"),
    I = as.numeric(I) /1000.0,
    V = as.numeric(V) / 1000.0,
    Power = -(I * V) #Invert so comsumption is positive
  ) %>% select(date, I, V, Power)


plt <- ggplot(cc_data, aes(x=date, y=Power)) +
  geom_point()+
  ylab("Power (W)") +
  xlab("Date") +
  ylim(0, 25)
plt
ggsave(plot=plt,"~/Desktop/SensorData/Streetlight_load.pdf", width=6, height=3.5, units="in")


bm_data <- ndjson::stream_in("~/Desktop/SensorData/StreetEnergy_BMV-700.log") %>%
  mutate(
    date = as.POSIXct(time, origin="1970-01-01 00:00:00"),
    SOC = as.numeric(SOC) /10.0,
    V = as.numeric(V) / 1000.0
  )


plt <- ggplot(bm_data, aes(x=date, y=SOC)) +
  geom_point()+
  ylab("Statge of Charge (%)") +
  xlab("Date") +
  ylim(0, 100)
plt
ggsave(plot=plt,"~/Desktop/SensorData/Streetlight_SOC.pdf", width=6, height=3.5, units="in")

plt <- ggplot(bm_data, aes(x=date, y=V)) +
  geom_point()+
  ylab("Voltage (V)") +
  xlab("Date") +
  ylim(10, 13)
plt
ggsave(plot=plt,"~/Desktop/SensorData/Streetlight_BatteryVoltage.pdf", width=6, height=3.5, units="in")
  
  