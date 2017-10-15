# -*- coding: utf-8 -*-
#
#  arduini.py
#  
#  Copyright 2017 (oddtopus)
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#(c) 2016 R. T. LGPL3

__title__="arduini"
__author__="oddtopus"
__url__="github.com/oddtopus/..."
__license__="LGPL 3"
__doc__='''
A module to create UNO and YUN instances in a Python script.
Depending on the model of Arduino and Script uploaded in it,
allows to "open / close / query" the boards via serial connection
or Wi-Fi.
It needs that "pyserial" module is available in the Python's env.
'''  

try:  from urllib.request import urlopen # for pyhton3
except: from urllib2 import urlopen # for pyhton2
try: import serial
except: print("Module pyserial not found.\nInstall pyserial first.")
import time

class UNO(object):
  '''
  Class to comunicate with an Arduino over serial ports.
  For example with the sample sketch PORTs.ino
  '''
  def __init__(self):
    self.board=None
    self.port=None
    self.DDRD=[False for i in range(8)] # port D are pins from 0 (LSB) to 7 (MSB)
    self.PORTD=[False for i in range(8)]
    self.PIND=[False for i in range(8)]
  def open(self, port='/dev/ttyACM0'):
    if type(port)==str:
      self.port=port
      self.board = serial.Serial()
      self.board.port=self.port
      self.board.baudrate=9600
      self.board.timeout=3
      self.board.dsrdtr=True
      self.board.rtscts=False  
      try: 
        self.board.open()
      except: 
        print('-LOCAL- '+self.port+" not available.")
  def close(self):
    if self.board: 
      self.board.close()
      print('-LOCAL- '+self.port+" closed.")
      self.port=None
      self.board=None
  def request(self,command,delay=0.1):
    if self.board and self.board.isOpen():
      self.board.writelines([command.encode()])
      time.sleep(delay)
      data=self.board.read(self.board.inWaiting())
      return data
    else:
      print('-LOCAL- '+self.port+' is not open')    

class YUN(object):
  '''
  Class to comunicate with an Arduino over Wi-Fi.
  For example with the sample sketch srf02overWiFi.ino.
  '''
  def __init__(self):
    self.answer=None
  def request(self,command, address='192.168.240.1'):
    if address:
      self.answer=urlopen('http://'+address+"/arduino/"+command)
      risp=self.answer.read()
      if risp: return risp
      else: return None
