library(tidyverse)



data <- read_csv("./Threshold_Acceleration_Test_Final.csv", col_names=FALSE) %>%
  filter(X1 < 1531499953,
         X1 > 1531496269)

plt <- ggplot(data) +
  geom_path(aes(x=X1, y=X9)) +
  ylab("Activity") +
  xlab("Time")
plt
ggsave(plot=plt,"~/Desktop/MovementPlots/ExperimentalRun.pdf", width=6, height=3.5, units="in")
ggsave(plot=plt,"~/Desktop/MovementPlots/ExperimentalRun.png", width=6, height=3.5, units="in")


data <- gather(data, axis, value, x:z, factor_key=TRUE)


data <- read_csv("./Second_Acceleration_Profile.csv", col_names=FALSE)
names(data) <- c ("seq", "x", "y", "z")
data <- gather(data, axis, value, x:z, factor_key=TRUE)


mg_LSB = 15.6
data$value_msg <- data$value * mg_LSB

plt <- ggplot(data, aes(x=seq, y=value, group=axis, colour=axis)) +
         geom_point() +
         facet_grid(axis~.) +
  ylab("Acceleration (LSB)") +
  xlab("Sequence Number")
plt
ggsave(plot=plt,"~/Desktop/MovementPlots/RawData.pdf", width=6, height=3.5, units="in")
ggsave(plot=plt,"~/Desktop/MovementPlots/RawData.png", width=6, height=3.5, units="in")


x_data <- data %>% filter(axis=="x")


x_data$diff <- c(NA, abs(diff(x_data$value)))


plt <- ggplot(x_data, aes(x=seq, y=value)) +
  geom_point(colour="#F8766D") +
  facet_grid(axis~.) +
  geom_hline(aes(yintercept=10)) +
  ylab("Accelleration (LSB)") +
  xlab("Sequence Number")
plt
ggsave(plot=plt,"~/Desktop/MovementPlots/XAxisThreshold.pdf", width=6, height=3.5, units="in")
ggsave(plot=plt,"~/Desktop/MovementPlots/XAxisThreshold.png", width=6, height=3.5, units="in")



plt <- ggplot(x_data, aes(x=seq, y=diff)) +
  geom_point(colour="#F8766D") +
  geom_hline(aes(yintercept=10))  +
  ylab("Accelleration Difference (LSB)") +
  xlab("Sequence Number")
plt
ggsave(plot=plt,"~/Desktop/MovementPlots/XAxisDiffThreshold.pdf", width=6, height=3.5, units="in")
ggsave(plot=plt,"~/Desktop/MovementPlots/XAxisDiffThreshold.png", width=6, height=3.5, units="in")





