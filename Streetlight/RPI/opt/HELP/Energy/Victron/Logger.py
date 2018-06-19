# Logger.py  20/01/2018  R.Wilkins
#
# Logger for reading from Victron device

#Ve direct class taken from: https://github.com/karioja/vedirect and modified to fit purpose
# The code will log all messages from the device and log
# to a CSV log file.

import time
import datetime
import argparse
import sys
import os
import json
import serial
import logging
import paho.mqtt.client as mqtt


MQTT_SERVER = "159.65.25.153"

HOST_NAME = os.uname()[1]

class vedirect:
    def __init__(self, serialport, out_dir, timeout=60, write_rate=3600, sample_period=60):
        #VE.Direct declatations
        self.serialport = serialport
        self.ser = serial.Serial(serialport, 19200, timeout=timeout)
        self.header1 = '\r'
        self.header2 = '\n'
        self.hexmarker = ':'
        self.delimiter = '\t'
        self.key = ''
        self.value = ''
        self.bytes_sum = 0;
        self.state = self.WAIT_HEADER
        self.dict = {}

        #Logger declarations
        self.running = True #We want to run straight away so set to True
        self.sample_period = sample_period
        self.write_rate = write_rate #How many seconds of data to store before writing to file
        self.timeout_period = 300 #if no data exit and have supervisor and restart
        
        self.last_sense = 0 #last sensing time
        self.last_write = 0 #last time data was written

        self.data = [] #Array to store data in memory
        self.usb_pending_data = [] #Store data that couldn't be wrote to usb
        
        self.out_dir = out_dir #Set output directory
        self.bak_dir = "/opt/STAR/Data" #Set output directory
       
        self.log = logging.getLogger("VE Direct logger") #setup logging
        
        count = 15
        self.dName=	None
        while count > 0:
            pkt = self.read_data_single();
            if pkt != None:
                self.dName = pkt['PID']
                break
            else:
                count = count - 1
                time.sleep(1)

        if self.dName == None:
            exit()

        self.fname = self.out_dir + "/"+HOST_NAME+"_"+self.dName+"_VEDirect.log" #Set output file name base on host name, and the type of device
        self.bname = self.bak_dir + "/"+HOST_NAME+"_"+self.dName+"_VEDirect.log" #Set output file name base on host name, and the type of device


    (HEX, WAIT_HEADER, IN_KEY, IN_VALUE, IN_CHECKSUM) = range(5)

    def read_data_single(self):
        while True:
            byte = self.ser.read(1)
            packet = self.input(byte)
            if (packet != None):
                return packet
            
    def input(self, byte):
        if byte == self.hexmarker and self.state != self.IN_CHECKSUM:
            self.state = self.HEX
        if self.state == self.WAIT_HEADER:
            self.bytes_sum += ord(byte)
            if byte == self.header1:
                self.state = self.WAIT_HEADER
            elif byte == self.header2:
                self.state = self.IN_KEY
            return None
        elif self.state == self.IN_KEY:
            self.bytes_sum += ord(byte)
            if byte == self.delimiter:
                if (self.key == 'Checksum'):
                    self.state = self.IN_CHECKSUM
                else:
                    self.state = self.IN_VALUE
            else:
                self.key += byte
            return None
        elif self.state == self.IN_VALUE:
            self.bytes_sum += ord(byte)
            if byte == self.header1:
                self.state = self.WAIT_HEADER
                self.dict[self.key] = self.value;
                self.key = '';
                self.value = '';
            else:
                self.value += byte
            return None
        elif self.state == self.IN_CHECKSUM:
            self.bytes_sum += ord(byte)
            self.key = ''
            self.value = ''
            self.state = self.WAIT_HEADER
            if (self.bytes_sum % 256 == 0):
                self.bytes_sum = 0
                return self.dict
            else:
                #print 'Malformed packet'
                self.bytes_sum = 0
        elif self.state == self.HEX:
            self.bytes_sum = 0
            if byte == self.header2:
                self.state = self.WAIT_HEADER
            else:
               raise AssertionError()
           
    #Start the sensing process by setting the run flag to true
    def start(self):
        self.running = True

    #Stop the sensing process by setting the run flag to true
    def stop(self):
        self.running = False

    #Read incoming data and log to memory                                                                                                                                
    def data_logger(self):
        timestamp = time.time()
        if ((timestamp - self.last_sense) > self.sample_period):
            try:
                pkt = self.read_data_single();
                if pkt != None:
                    pkt["time"]=timestamp
                    #Turn packet into a JSON string
                    json_str = json.dumps(pkt) + "\n"
                    if '\\' in json_str: #serial out of sync break fast
                        self._writecsv()
                        exit(1)
                    #store in memory
                    self.data.append(json_str)

                    #transmit the data
                    mqtt.single("Street/"+HOST_NAME+"/Energy", json_str, hostname=MQTT_SERVER)

                    self.transmit(json_str)
                    self.last_sense = timestamp #And update the timestamp
            except:
                pass
        if ((timestamp - self.last_sense) > self.timeout_period):
            self._writecsv()
            exit(1)


    def transmit(self, pkt):
        return False
    
    def setFName(self):
        now = datetime.datetime.now()
        doy = now.strftime("%Y-%m-%d")
        #Set output file name base on date, host name, and the type of device
        self.fname = self.out_dir + "/" + doy + "_" + HOST_NAME + "_" + self.dName + "_VEDirect.log"

    def setBName(self):
        now = datetime.datetime.now()
        doy = now.strftime("%Y-%m-%d")
        self.bname = self.bak_dir + "/" + doy + "_" + HOST_NAME + "_" + self.dName + "_VEDirect.log" #Set output file name base on host name, and the type of device

        
    def _writecsv(self):
        
        #Check USB is mounted
        if (os.path.ismount(self.out_dir)):
            #write data
            self.setFName();
            data_file = open(self.fname, 'a+')
            bkup_file = open(self.fname+".bak", 'a+')
            for json in self.usb_pending_data:
                data_file.write(json)
                bkup_file.write(json)
            for json in self.data:
                data_file.write(json)
                bkup_file.write(json)
            data_file.close()
            bkup_file.close()
            
            #Reset data store and last write time
            self.usb_pending_data = []
            self.data = []
            return True
        
        else:
            #if usb is not mounted write to /opt/STAR/Data
            self.log.debug("USB not mounted")
            #write data
            self.setBName();
            data_file = open(self.bname, 'a+') #write to /opt/STAR
            bkup_file = open(self.bname+".bak", 'a+')
                        
            for json in self.data:
                data_file.write(json)
                bkup_file.write(json)
            data_file.close()
            bkup_file.close()
            
            #Reset data store and last write time
            self.usb_pending_data.extend(self.data)
            self.data = [] #Data has been written so we can clear data
            return False #We may want to remount?

        
    def writecsv(self):
        timestamp = time.time()
        if ((timestamp - self.last_write) > self.write_rate): #If it is time to write
            self.running=False
            self._writecsv()
            self.last_write = timestamp #And update the timestamp
            self.running=True
        else:
            self.running=True

    #Method to run the main loop, whilst the system is running keep taking readings
    def run(self):
        while self.running:
            self.data_logger()
            self.writecsv()


if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(description="Get data from VE.Direct devices")
    parser.add_argument("-o", "--out-dir", metavar="DIR",
                            default=".",
                            help="Output directory for files (default .)")
    parser.add_argument("-s", "--sample-period",
                            default=60,
                            help="Sample period in seconds")
    parser.add_argument("-p", "--port", metavar="DIR",
                            default="/dev/ttyUSB0",
                            help="Output directory for files (default .)")
    parser.add_argument("-l", "--log-level", default="info", metavar="LEVEL",
                            help="Set log level to LEVEL: debug,info,warning,error")
    parser.add_argument("-f", "--log-file", default="VELogger.log", metavar="LogFile",
                            help="Set log file name")
    args = parser.parse_args()
    
    LVLMAP = {"debug": logging.DEBUG,
                  "info": logging.INFO,
                  "warning": logging.WARNING,
                  "error": logging.ERROR,
                  "critical": logging.CRITICAL}

    logging.basicConfig(filename="/var/log/" + args.log_file,
                            filemode="a+",
                            format="%(asctime)s %(levelname)s %(message)s",
                            level=LVLMAP[args.log_level])

    logging.info("Starting VicrtronLogger with log-level %s" % (args.log_level))

    
    args = parser.parse_args()

    VE = vedirect(
        serialport = args.port,
        timeout=10,
        out_dir = args.out_dir,
        write_rate=3600,
        sample_period=float(args.sample_period))
    VE.run()
        


