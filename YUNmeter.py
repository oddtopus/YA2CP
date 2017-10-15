#!/usr/bin/env python
# -*- coding: utf-8 -*-
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

__title__="YUNmeter"
__author__="oddtopus"
__url__="github.com/oddtopus/..."
__license__="LGPL 3"
__doc__='''
Client frontend to get measures of distance from an ArduinoYUN.
Materials requested:
  - Arduino YUN running the script "srf02overWiFy.ino" and connected
    to the same WiFi network of the PC.
  - SRF02 (or similar) ultrasonic range finder connected to Arduino
    as one I2C device.
'''  


from PySide import QtCore, QtGui
from YUNmeterDialog import Ui_Dialog
import arduini

class myMW(Ui_Dialog):
  def __init__(self):
    self.MainWindow = QtGui.QMainWindow()
    self.setupUi(self.MainWindow)
    self.board=arduini.YUN()
    self.btnOK.clicked.connect(self.measure)
    self.btnCancel.clicked.connect(lambda: self.MainWindow.close())
  def measure(self):
    print 'Sending "http://'+self.edit1.text()+'/arduino/srf'
    try:
        risp=self.board.request(command='srf',address=self.edit1.text())
        if risp: 
          print risp
          if self.radio2.isChecked():
            self.lcd.display(int(float(risp.strip())/2.54))
          else:
            self.lcd.display(int(risp.strip()))
    except: pass
  def reject(self):
    self.close()

if __name__ == "__main__":
  import sys
  app = QtGui.QApplication(sys.argv)
  ui = myMW()
  ui.MainWindow.show()
  sys.exit(app.exec_())

