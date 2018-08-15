
library(tidyverse)

sensor_switch <- read_csv("SensorSwitch.CSV", col_names=FALSE) %>%
  rename(
    Time = X4,
    Voltage = X5
  ) %>%
  mutate(
    Voltage = ifelse(Voltage==0.04, 0, Voltage)
  ) %>%
  filter(Voltage >= 0)

plt <- ggplot(sensor_switch, aes(x=Time, y=Voltage)) +
  geom_step() +
  ylim(0,3.5) +
  xlab("Time (s)") +
  ylab("Voltage (V)")
ggsave("./SensorSwitch.pdf", plt, width = 6.5, height = 3)

