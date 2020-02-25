import serial
import threading
#from threading import Thread
import time
import logging
import sys

class comObj:
  def __init__(self):
    self.rxBuffer = ''
  
class TreadReadSerial (threading.Thread):
  def __init__(self, comObj, port):
    super(TreadReadSerial, self).__init__()
    self.port = port
    self.comObj = comObj

  def run (self):
    while True:
      myChar = self.port.read(1)
      if myChar!='':
        sys.stdout.write(myChar)
        sys.stdout.flush()
        self.comObj.rxBuffer = self.comObj.rxBuffer + myChar

class TreadTestSerial (threading.Thread):
  def __init__(self, comObj, port):
    super(TreadTestSerial, self).__init__()
    self.port = port
    self.comObj = comObj
    self.minDelay = 0.02
    self.maxDelay = 0.080
    self.incDelay = 0.0001
    self.cycleTime = 0.3

  def run(self):
    self.outputDelay = self.minDelay
    while True:
      logging.info("Start Cycle, delay = %f", self.outputDelay)
      self.port.write("12345678")
      time.sleep(self.outputDelay)
      self.port.write("ABCDEFGH\n")
      
      time.sleep(self.cycleTime - self.outputDelay)
      
      self.outputDelay = self.outputDelay + self.incDelay
      if( self.outputDelay > self.maxDelay ):
        self.outputDelay = self.minDelay
        
      if self.comObj.rxBuffer != "12345678ABCDEFGH\n":
        print("No match")
        print self.comObj.rxBuffer
        sys.exit()
      
      self.comObj.rxBuffer = ''


	  

if __name__ == "__main__":
  
  print("I started all by myself!")
  
  port = serial.Serial("/dev/ttyS38", baudrate=115200, timeout=3.0)

  format = "%(asctime)s: %(message)s"
  logging.basicConfig(format=format, level=logging.INFO, datefmt="%H:%M:%S")
  logging.info("Main    : before creating thread")
  
  myComObj = comObj()
  
  rxT = TreadReadSerial(myComObj, port)
  tT = TreadTestSerial(myComObj, port)
  logging.info("Main    : before running threads")
  rxT.daemon = True
  tT.daemon = True
  
  rxT.start()
  tT.start()
	
  logging.info("Main    : wait for the thread to finish")
  
  # x.join()
  while(1):
    pass	
  logging.info("Main    : all done")

