#pragma once

bool connectMQTT(void);
void disconnectMQTT(void);
bool transmit(String topic, String dataString);

bool connectGSM(void);
void getGSMTime(Data* readings);
void disconnectGSM(void);

String constructJSON(Data* reading);
